/*
 * audio_effect_class.c
 *
 *  Created on: Jun 8, 2023
 *      Author: szsj-1
 */
#include <string.h>
#include <stdint.h>
#include "debug.h"
#include "app_config.h"
#include "audio_effect_library.h"
#include "ctrlvars.h"
#include "audio_effect_class.h"
#include "audio_effect_api.h"
#include "nds32_intrinsic.h"
#include "communication.h"
#include "main_task.h"
//-------------//
extern uint8_t  tx_buf[256];
extern bool IsEffectChange;
extern void* effect_list_param[AUDIO_EFFECT_SUM];
void Communication_Effect_Send(uint8_t *buf, uint32_t len);
uint8_t AudioEffectClassDescParser(uint8_t Control,const void *pdata);
void Comm_Effect_Send(void *pEffect, uint8_t Enable, void* param, uint16_t Len, uint8_t Control);
//--------------//
/*
 * 具体协议在调音工具目录下：固件与用户应用程序通信协议V2.39.3.pdf----> 5.20.46 用户自定义音效格式
|----------------------------------------------------------------------------------------|
|                |  parameter total |
|----------------------------------------------------------------------------------------|
|   parameter n  |  data type       | data .......     | default value    | name,unit
|----------------------------------------------------------------------------------------|
|   parameter n  |  data type       | data .......     | default value    | name,unit
|----------------------------------------------------------------------------------------|
|   parameter n  |  data type       | data .......     | default value    | name,uni
|----------------------------------------------------------------------------------------|
|   parameter n  |  data type       | data .......     | default value    | name,unit
|----------------------------------------------------------------------------------------|
|   parameter n  |  data type       | data .......     | default value    | name,unit
|----------------------------------------------------------------------------------------|
*/
/*data type
  boolType
  0x00: 逻辑类型
                 2 Bytes: 默认值
  enumType
  0x01: 枚举类型
                 1 Byte：参数内容长度
                 N Bytes: 参数内容， 字符串格式上传，并用分号（；）间隔开不同的枚举值
                 2 Bytes: 默认值

  NumberType
  0x02: 连续类型    2 Bytes: 最小值
                 2 Bytes: 最大值
                 2 Bytes: 步进值
                 2 Bytes： 显示与传输值关系, 范围 1~65536 1： 显示值=传输值2： 显示值=传输值/2…1024： 显示值 = 传输值/1024
                 2 Bytes： 保留小数位                 0： 整数1： 1 位小数 2： 2 位小数
                 2 Bytes: 默认值
                 1 Byte:  参数的单位长度
                 N Bytes: 参数的单位内容
  DispType
  0x03: 显示类型（只读）
                2 Bytes: 最小值
                2 Bytes: 最大值
                2 Bytes：显示值与传输值关系           1： 显示值=传输值           2： 显示值=传输值/2   1024： 显示值 = 传输值/1024
                2 Bytes：保留小数位 0： 整数 1： 1 位小数 2： 2 位小数
                2 Bytes: 默认值
                1 Byte:  参数的单位长度
                N Bytes: 参数的单位内容
*/
/*********************************************
 *
 *
 * ButterWorth audio class,describe tab
 *
 *
 *********************************************/
const ButterWorthDescribe ButterWorthTab=
{
  {3},
#if CFG_AUDIO_EFFECT_BUTTERWORTH
  {enumType,     "Low Pass;Hight Pass",0x0000,             "filter_type"         },//0:Low-pass, 1:High-pass
  {continueType, 0x0001,0x000A,0x0001,0x0001,0x0000,0x0001,"filter_order",   " "},//1~10
  {continueType, 0x0014,0x5DC0,0x0001,0x0001,0x0000,0x0032,"Cut Freq",       "HZ"},//
#endif
};
void Comm_Effect_ButterWorth(EffectNode *pNode, uint8_t * buf, uint8_t index)
{
#if CFG_AUDIO_EFFECT_DYNAMIC_EQ
	int16_t TmpData16;
	uint8_t Control = index + 0x81;
	ButterWorthUnit *p = (ButterWorthUnit *)pNode->EffectUnit;

	switch(buf[0])//
	{
		case 0:
			memcpy(&TmpData16, &buf[1], 2);
			if(p == NULL)
			{
				if(TmpData16 == 1)
				{
					pNode->Enable = 1;
					IsEffectChange = 1;
				}
			}
			else
			{
				if(p->enable != TmpData16)
				{
					p->enable = TmpData16;
					pNode->Enable = p->enable;
					IsEffectChange = 1;
				}
			}
#ifdef CFG_EFFECT_PARAM_UPDATA_BY_ACPWORKBENCH
			effect_enable_list[index] = pNode->Enable;
#endif
			break;
		case 1:
			memcpy(&TmpData16, &buf[1], 2);
			if(p->param.filter_type == TmpData16)
			{
				break;
			}
			p->param.filter_type = TmpData16 &0x01;

			if(p->enable)
			{
				AudioEffectButterWorthInit(p,p->channel,gCtrlVars.sample_rate);
			}
			break;
		case 2:
			memcpy(&TmpData16, &buf[1], 2);
			if(p->param.filter_order == TmpData16)
			{
				break;
			}
			if(TmpData16 < 1)  TmpData16 = 1;
			if(TmpData16 > 10)  TmpData16 = 10;

			p->param.filter_order = TmpData16;

			if(p->enable)
			{
				AudioEffectButterWorthInit(p,p->channel,gCtrlVars.sample_rate);
			}
			break;
		case 3:
			memcpy(&TmpData16, &buf[1], 2);
			if(p->param.fc == TmpData16)
			{
				break;
			}
			if(TmpData16 < 20)   TmpData16 = 20;//ButterWorthTab
			if(TmpData16 > (gCtrlVars.sample_rate/2))  TmpData16 = gCtrlVars.sample_rate/2;
			p->param.fc = TmpData16;
			if(p->enable)
			{
				AudioEffectButterWorthInit(p,p->channel,gCtrlVars.sample_rate);
			}
			break;

		case 0xFF:
			memcpy(&TmpData16, &buf[1], 2);
			if(p == NULL)
			{
				if(TmpData16 == 1)
				{
					pNode->Enable = 1;
					IsEffectChange = 1;
				}
#ifdef CFG_EFFECT_PARAM_UPDATA_BY_ACPWORKBENCH
				effect_enable_list[index] = pNode->Enable;
#endif
				memcpy(effect_list_param[index], &buf[3], sizeof(ButterWorthParam));//导入ini文件时，如果音效默认开启和ini文件不同，默认未开启的音效会变回默认参数
				break;
			}
			if(p->enable != TmpData16)
			{
				p->enable = TmpData16;
				pNode->Enable = p->enable;
			}
			memcpy(&p->param, &buf[3], sizeof(ButterWorthParam));
			AudioEffectButterWorthInit(p,p->channel,gCtrlVars.sample_rate);

			break;
		case 0xf0:
			AudioEffectClassDescParser(Control,&ButterWorthTab);
			break;
		default:
			break;
	}
#endif
}
void Communication_Effect_ButterWorth(uint8_t Control, EffectNode* addr, uint8_t *buf, uint32_t len)
{
#if CFG_AUDIO_EFFECT_BUTTERWORTH
	EffectNode       *pNode = addr;
	ButterWorthUnit    *p = (ButterWorthUnit *)pNode->EffectUnit;
	uint16_t LenParam = sizeof(ButterWorthParam);
	void* pParam = effect_list_param[Control - 0x81];
	uint8_t Enable = 0;

	if(len == 0)//ask
	{
		if(p != NULL)
		{
			Enable = p->enable;
			pParam = (void *)&p->param;
		}
		Comm_Effect_Send(p, Enable, pParam, LenParam, Control);
	}
	else
	{

		Comm_Effect_ButterWorth(pNode, buf, Control - 0x81);
		if(p != NULL)
		{
			memcpy(effect_list_param[Control - 0x81], &p->param, sizeof(ButterWorthParam));
		}
	}
#endif //CFG_AUDIO_EFFECT_BUTTERWORTH
}

/*********************************************
 *
 *
 * Dynamic Eq  audio class,describe tab
 *
 *
 *********************************************/
const DynamicEqDescribe DynamicEqDescribeTab=
{
  {5},
#if CFG_AUDIO_EFFECT_DYNAMIC_EQ
  // ID           Min     Max   Step   ratio fraction    default   name                  unit
  {continueType, 0xDCD8,0x0000,0x0001,0x0064,0x0002,     0x0000,   "Low_threshold",      "DB"},// -9000 ~ 0 to cover -90.00dB ~ 0.00dB
  {continueType, 0xDCD8,0x0000,0x0001,0x0064,0x0002,     0x0000,   "Normal_threshold",   "DB"},// -9000 ~ 0 to cover -90.00dB ~ 0.00dB
  {continueType, 0xDCD8,0x0000,0x0001,0x0064,0x0002,     0x0000,   "Hight_threshold",    "DB"},// -9000 ~ 0 to cover -90.00dB ~ 0.00dB
  {continueType, 0x0000,0x07D0,0x0001,0x0001,0x0000,     0x0064,   "Attack",             "ms"},//1~10
  {continueType, 0x0000,0x07D0,0x0001,0x0001,0x0000,     0x03E8,   "Release",            "ms"},//
#endif
};
void Comm_Effect_HarmonicDynamicEQ(EffectNode *pNode, uint8_t * buf, uint8_t index)
{
#if CFG_AUDIO_EFFECT_DYNAMIC_EQ
	int16_t TmpData16;
	uint8_t Control = index + 0x81;
	DynamicEqUnit *p = (DynamicEqUnit *)pNode->EffectUnit;

	switch(buf[0])//
	{
		case 0:
			memcpy(&TmpData16, &buf[1], 2);
			if(p == NULL)
			{
				if(TmpData16 == 1)
				{
					pNode->Enable = 1;
					IsEffectChange = 1;
				}
			}
			else
			{
				if(p->enable != TmpData16)
				{
					p->enable = TmpData16;
					pNode->Enable = p->enable;
					IsEffectChange = 1;
				}
			}
#ifdef CFG_EFFECT_PARAM_UPDATA_BY_ACPWORKBENCH
			effect_enable_list[index] = pNode->Enable;
#endif
			break;
		case 1:
			memcpy(&TmpData16, &buf[1], 2);

			if(p->param.low_energy_threshold == TmpData16)
			{
				break;
			}
			if(TmpData16 > p->param.normal_energy_threshold)
			{
				break;
			}
			p->param.low_energy_threshold = TmpData16;

			if(p->enable)
			{
				AudioEffectDynamicEqInit(p,p->channel,gCtrlVars.sample_rate);
			}
			break;
		case 2:
			memcpy(&TmpData16, &buf[1], 2);
			if(p->param.normal_energy_threshold == TmpData16)
			{
				break;
			}
			if(TmpData16 > p->param.high_energy_threshold)
			{
				break;
			}
			if(TmpData16 < p->param.low_energy_threshold)
			{
				break;
			}
			p->param.normal_energy_threshold = TmpData16;

			if(p->enable)
			{
				AudioEffectDynamicEqInit(p,p->channel,gCtrlVars.sample_rate);
			}
			break;
		case 3:
			memcpy(&TmpData16, &buf[1], 2);
			if(p->param.high_energy_threshold == TmpData16)
			{
				break;
			}
			if(TmpData16 < p->param.normal_energy_threshold)
			{
				break;
			}
			p->param.high_energy_threshold = TmpData16;

			if(p->enable)
			{
				AudioEffectDynamicEqInit(p,p->channel,gCtrlVars.sample_rate);
			}
			break;
		case 4:
			memcpy(&TmpData16, &buf[1], 2);
			if(p->param.attack_time == TmpData16)
			{
				break;
			}
			p->param.attack_time = TmpData16;

			if(p->enable)
			{
				AudioEffectDynamicEqInit(p,p->channel,gCtrlVars.sample_rate);
			}
			break;
		case 5:
			memcpy(&TmpData16, &buf[1], 2);
			if(p->param.release_time == TmpData16)
			{
				break;
			}
			p->param.release_time = TmpData16;

			if(p->enable)
			{
				AudioEffectDynamicEqInit(p,p->channel,gCtrlVars.sample_rate);
			}
			break;
		case 0xFF:
			memcpy(&TmpData16, &buf[1], 2);
			if(p == NULL)
			{
				if(TmpData16 == 1)
				{
					pNode->Enable = 1;
					IsEffectChange = 1;
				}
#ifdef CFG_EFFECT_PARAM_UPDATA_BY_ACPWORKBENCH
				effect_enable_list[index] = pNode->Enable;
#endif
				memcpy(effect_list_param[index], &buf[3], sizeof(DynamicEqParam));//导入ini文件时，如果音效默认开启和ini文件不同，默认未开启的音效会变回默认参数
				break;
			}
			if(p->enable != TmpData16)
			{
				p->enable = TmpData16;
				pNode->Enable = p->enable;
			}
			memcpy(&p->param, &buf[3], sizeof(DynamicEqParam));
			AudioEffectDynamicEqInit(p, p->channel, gCtrlVars.sample_rate);
			break;
		case 0xf0:
			AudioEffectClassDescParser(Control,&DynamicEqDescribeTab);
			break;
		default:
			break;
	}
#endif
}

void Communication_Effect_DynamicEQ(uint8_t Control, EffectNode* addr, uint8_t *buf, uint32_t len)
{
#if CFG_AUDIO_EFFECT_DYNAMIC_EQ

	EffectNode       *pNode = addr;
	DynamicEqUnit    *p = (DynamicEqUnit *)pNode->EffectUnit;
	uint16_t LenParam = sizeof(DynamicEqParam);
	void* pParam = effect_list_param[Control - 0x81];
	uint8_t Enable = 0;

	if(len == 0)//ask
	{
		if(p != NULL)
		{
			Enable = p->enable;
			pParam = (void *)&p->param;
		}
		Comm_Effect_Send(p, Enable, pParam, LenParam, Control);
	}
	else
	{

		Comm_Effect_HarmonicDynamicEQ(pNode, buf, Control - 0x81);
		if(p != NULL)
		{
			memcpy(effect_list_param[Control - 0x81], &p->param, sizeof(DynamicEqParam));
		}
	}
#endif //CFG_AUDIO_EFFECT_DYNAMIC_EQ
}
/*********************************************
 *
 *
 * VBSurround audio class,describe tab
 *
 *
 *********************************************/
const VirtualSurroundDescribe VBSurroundTab=
{
  {0},
#if CFG_AUDIO_EFFECT_VIRTUAL_SURROUND_EN
//	{continueType, 0x0000,0x001E,0x0001,0x0001,0x0000,0x0010,"reserve",   "xx"},
#endif
};

#if CFG_AUDIO_EFFECT_VIRTUAL_SURROUND_EN
void Comm_Effect_VirtualSurround(EffectNode *pNode, uint8_t * buf, uint8_t index)
{
	int16_t TmpData16;
	VirtualSurroundUnit *p = (VirtualSurroundUnit *)pNode->EffectUnit;
	uint8_t Control = index + 0x81;
	//int16_t parameter_len = (int16_t)VBSurroundTab.numbers.paramete_totals + 1;//+ 1 = enable

	memset(tx_buf, 0, sizeof(tx_buf));

	switch(buf[0])//
	{
		case 0:
			memcpy(&TmpData16, &buf[1], 2);
			if(p == NULL)
			{
				if(TmpData16 == 1)
				{
					pNode->Enable = 1;
					IsEffectChange = 1;
				}
			}
			else
			{
				if(p->enable != TmpData16)
				{
					p->enable = TmpData16;
					pNode->Enable = p->enable;
					if(p->enable)
					{
						//AudioEffectPingPongInit(p, p->channel, gCtrlVars.sample_rate);
						IsEffectChange = 1;//
					}
				}
			}
#ifdef CFG_EFFECT_PARAM_UPDATA_BY_ACPWORKBENCH
			effect_enable_list[index] = pNode->Enable;
#endif
			break;
//		case 1:
//			memcpy(&TmpData16, &buf[1], 2);
//			if(TmpData16 > 100)
//			{
//				TmpData16 = 100;
//			}
//			if(p->param.intensity != TmpData16)
//			{
//				p->param.intensity = TmpData16;
//				AudioEffectThreeDInit(p, p->channel, gCtrlVars.sample_rate);
//			}
//			break;
		case 0xf0:
			AudioEffectClassDescParser(Control,&VBSurroundTab);
			break;
		case 0xFF:
			memcpy(&TmpData16, &buf[1], 2);
			if(p == NULL)
			{
				if(TmpData16 == 1)
				{
					pNode->Enable = 1;
					IsEffectChange = 1;
				}
#ifdef CFG_EFFECT_PARAM_UPDATA_BY_ACPWORKBENCH
				effect_enable_list[index] = pNode->Enable;
#endif
				memcpy(effect_list_param[index], &buf[3], sizeof(VirtualSurroundParam));//导入ini文件时，如果音效默认开启和ini文件不同，默认未开启的音效会变回默认参数
				break;
			}
			if(p->enable != TmpData16)
			{
				p->enable = TmpData16;
				pNode->Enable = p->enable;
			}
			memcpy(&p->param, &buf[3], sizeof(VirtualSurroundParam));
			AudioEffectVirtualSurroundInit(p, p->channel, gCtrlVars.sample_rate);
			break;
		default:
			break;
	}
}
void Communication_Effect_VirtualSurround(uint8_t Control, EffectNode* addr, uint8_t * buf, uint32_t len)
{
	EffectNode *pNode = addr;
	VirtualSurroundUnit *p = (VirtualSurroundUnit *)pNode->EffectUnit;

	if(len == 0)//ask
	{
		uint16_t LenParam = sizeof(VirtualSurroundParam);
		uint8_t Enable = 0;
		void* pParam = effect_list_param[Control - 0x81];
		if(p != NULL)
		{
			Enable = p->enable;
			pParam = (void *)&p->param;
		}
		Comm_Effect_Send(p, Enable, pParam, LenParam, Control);
	}
	else
	{
		Comm_Effect_VirtualSurround(pNode, buf, Control - 0x81);
		if(p != NULL)
		{
			memcpy(effect_list_param[Control - 0x81], &p->param, sizeof(VirtualSurroundParam));
		}
	}
}
#endif //end of CFG_AUDIO_EFFECT_VIRTUAL_SURROUND_EN

/*
 *  AudioEffectTypeDescParser
 *  buf = tx_buf, pdata=audio effect describe
 *  return data_len
 */
uint8_t AudioEffectClassDescParser(uint8_t Control,const void *pdata)
{
	    uint8_t len ,slen,i;
	    uint8_t pos = 0,m;
	    int32_t *dptr,data_type;
	    TypeBool      *p1;
	    TypeEnum      *p2;
	    TypeContinue  *p3;
	    TypeDisp      *p4;

	    uint8_t *buf = tx_buf;

	    buf[pos++] = 0xa5;
	    buf[pos++] = 0x5a;
	    buf[pos++] = Control;
	    buf[pos++] = 0;//len
	    buf[pos++] = 0xf0;

	    dptr =  (int32_t *)pdata;
	    data_type = *dptr;
        dptr += ParameterTotasSize;

        len = data_type&0xff;
        buf[pos++] = len;//parameter numbers;
        //-------------------------------//
        for(i = 0; i < len;i++)
        {
			//----------------------------------------//
			p1 = (TypeBool *)dptr;
			p2 = (TypeEnum *)dptr;
			p3 = (TypeContinue *)dptr;
			p4 = (TypeDisp *)dptr;
			data_type = p1->data_type;
			//----------------------------------------//
			if(data_type==boolType)//bool
			{
			   dptr += LogicSize;
			   slen=0;
			   m = pos;
			   pos++;
			   strcpy((char *)&buf[pos],(char *)p1->name);
			   slen = strlen(p1->name);
			   buf[m] = slen;
			   pos += slen;
			   memcpy(&buf[pos],&p1->data_type,1);
			   pos += 1;

			   memcpy(&buf[pos],&p1->defualt,2);
			   pos += 2;
			   //DBG("P1:%d->%08x  %08x\n",slen,p1->data_type,p1->enable);

			}

			if(data_type==enumType)//enum
			{
			   dptr += EnumSize;
			   //-----name--------------------//
			   slen=0;
			   m = pos;
			   pos++;
			   strcpy((char *)&buf[pos],(char *)p2->name);
			   slen = strlen(p2->name);
			   buf[m] = slen;
			   pos += slen;
			   memcpy(&buf[pos],&p2->data_type,1);
			   pos += 1;
			   //-----select name-----------------//
			   slen=0;
			   m = pos;
			   pos++;
			   strcpy((char *)&buf[pos],(char *)p2->enum_name);
			   slen = strlen(p2->enum_name);
			   buf[m] = slen;
			   pos += slen;
			   //------------------------------//
			   memcpy(&buf[pos],&p2->defualt,2);
			   pos += 2;
			   //DBG("P1:%d->%08x  %08x  %08x  %08x  %s  %s\n",slen,p2->data_type,p2->defualt,p2->name,p2->str);
			}

			if(data_type==continueType)//continue
			{
				 dptr += ContinueSize;
				 slen=0;
				 m = pos;
				 pos++;
				 strcpy((char *)&buf[pos],(char *)p3->name);
				 slen = strlen(p3->name);
				 buf[m] = slen;
				 pos += slen;
				 memcpy(&buf[pos],&p3->data_type,1);
				 pos += 1;

				 memcpy(&buf[pos],&p3->min,2);
				 pos += 2;

				 memcpy(&buf[pos],&p3->max,2);
				 pos += 2;

				 memcpy(&buf[pos],&p3->step,2);
				 pos += 2;

				 memcpy(&buf[pos],&p3->ratio,2);
				 pos += 2;

				 memcpy(&buf[pos],&p3->fraction,2);
				 pos += 2;

				 memcpy(&buf[pos],&p3->defualt,2);
				 pos += 2;
				 //--unit-------------//
				 if(p3->unit==0)
				 {
					 buf[pos++] = 0;//len = 0
				 }
				 else
				 {
				   m = pos;
				   pos++;
				   strcpy((char *)&buf[pos],(char *)p3->unit);
				   slen = strlen(p3->unit);
				   buf[m] = slen;
				   pos += slen;
				 }

				//DBG("P1:%d->%08x  %08x  %08x  %08x  %s\n",slen,p3->data_type,p3->min,p3->max,p3->defualt,p3->name);
			}

			if(data_type==dispType)//disp
			{
			   dptr += DispSize;
			   slen=0;
			   m = pos;
			   pos++;

			   strcpy((char *)&buf[pos],(char *)p4->name);
			   slen = strlen(p4->name);
			   buf[m] = slen;
			   pos += slen;
			   memcpy(&buf[pos],&p4->data_type,1);
			   pos += 1;

			   memcpy(&buf[pos],&p4->min,2);
			   pos += 2;

			   memcpy(&buf[pos],&p4->max,2);
			   pos += 2;

			   memcpy(&buf[pos],&p4->ratio,2);
			   pos += 2;

			   memcpy(&buf[pos],&p4->fraction,2);
			   pos += 2;

			   memcpy(&buf[pos],&p4->defualt,2);
			   pos += 2;
			   //--unit-------------//
			   m = pos;
			   pos++;
			   strcpy((char *)&buf[pos],(char *)p4->unit);
			   slen = strlen(p4->unit);
			   buf[m] = slen;
			   pos += slen;
			   //DBG("P1:%d->%08x  %08x\n",slen,p4->data_type,p4->defualt);
			}
			//----------------------------------//
			if(pos>254)
			{
				pos = 4;
			}
        }
        buf[pos] = 0x16;//end
        pos -= 4;
        buf[3] = pos;//len
        Communication_Effect_Send(buf, pos+1);
	    return pos;
}
//----------------//
