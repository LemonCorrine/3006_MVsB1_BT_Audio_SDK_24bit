#include <string.h>
#include "type.h"
#include "dma.h"
#include "adc_interface.h"
#include "dac_interface.h"
#include "app_config.h"
#include "mode_task_api.h"
#include "main_task.h"
#include "debug.h"
#include "i2s_interface.h"
//service
#include "audio_core_api.h"
#include "audio_core_service.h"
#include "mode_task_api.h"

/*
	//I2S0_MODULE Port0
	#if CFG_RES_I2S_MODE == 0
	GPIO_PortAModeSet(GPIOA0, 9);// mclk 9:out  做master
	#else
	GPIO_PortAModeSet(GPIOA0, 3);// mclk 3:in   做slave
	#endif
	GPIO_PortAModeSet(GPIOA1, 6);// lrclk
	GPIO_PortAModeSet(GPIOA2, 5);// bclk
	GPIO_PortAModeSet(GPIOA3, 7);// out
	GPIO_PortAModeSet(GPIOA4, 1);// din

	//I2S0_MODULE Port1
	#if CFG_RES_I2S_MODE == 0
	GPIO_PortAModeSet(GPIOA24, 9);// mclk 9:out  做master
	#else
	GPIO_PortAModeSet(GPIOA24, 3);// mclk 3:in   做slave
	#endif
	GPIO_PortAModeSet(GPIOA20, 6);//lrclk
	GPIO_PortAModeSet(GPIOA21, 5);//bclk
	GPIO_PortAModeSet(GPIOA22, 10);//do
	GPIO_PortAModeSet(GPIOA23, 3);//di


	//I2S1_MODULE Port0
	#if CFG_RES_I2S_MODE == 0
	GPIO_PortAModeSet(GPIOA27, 6);//mclk 6:out  做master
	#else
	GPIO_PortAModeSet(GPIOA27, 1);//mclk 1:in   做slave
	#endif
	GPIO_PortAModeSet(GPIOA28, 1);//lrclk
	GPIO_PortAModeSet(GPIOA29, 1);//bclk
	GPIO_PortAModeSet(GPIOA30, 6);//do
	GPIO_PortAModeSet(GPIOA31, 2);//di

	//I2S1_MODULE Port1
	#if CFG_RES_I2S_MODE == 0
	GPIO_PortAModeSet(GPIOA7, 5);//mclk 5:out  做master
	#else
	GPIO_PortAModeSet(GPIOA7, 3);//mclk 3:in   做slave
	#endif
	GPIO_PortAModeSet(GPIOA8, 1);//lrclk
	GPIO_PortAModeSet(GPIOA9, 2);//bclk
	GPIO_PortAModeSet(GPIOA10, 4);//do
	GPIO_PortAModeSet(GPIOA11, 2);//di

	//I2S1_MODULE Port2
	GPIO_PortAModeSet(GPIOA1, 7);//lrclk
	GPIO_PortAModeSet(GPIOA2, 6);//bclk
	GPIO_PortAModeSet(GPIOA31, 5);//do
	GPIO_PortAModeSet(GPIOA30, 2);//di

	//I2S1_MODULE Port3
	GPIO_PortAModeSet(GPIOA20, 7);//lrclk
	GPIO_PortAModeSet(GPIOA21, 6);//bclk
	GPIO_PortAModeSet(GPIOA11, 4);//do
	GPIO_PortAModeSet(GPIOA10, 1);//di
*/
//I2S IO口配置见如上注释，根据需求自己搭配，可自由搭配，不一定要使用每组固定的IO
void I2S_GPIO_Port_ModeSet(I2S_MODULE I2SModuleIndex, uint8_t i2s_mode)
{
	if (I2SModuleIndex == I2S0_MODULE)
	{
		if (i2s_mode == 0){
			GPIO_PortAModeSet(GPIOA0, 9); // mclk 9:out  做master
		}else{
			GPIO_PortAModeSet(GPIOA0, 3); // mclk 3:in   做slave
		}
		GPIO_PortAModeSet(GPIOA1, 6);	  // lrclk
		GPIO_PortAModeSet(GPIOA2, 5);	  // bclk

		#if (defined(CFG_RES_AUDIO_I2SOUT_EN) && (CFG_RES_I2S == 0)) || defined(CFG_RES_AUDIO_I2S0OUT_EN)
		GPIO_PortAModeSet(GPIOA3, 7);	  // out
		#endif
		
		#if (defined(CFG_APP_I2SIN_MODE_EN) && (CFG_RES_I2S == 0)) || defined(CFG_RES_AUDIO_I2S0IN_EN)
		GPIO_PortAModeSet(GPIOA4, 1);	  // din
		#endif
	}

	if (I2SModuleIndex == I2S1_MODULE)
	{
		if (i2s_mode == 0){
			GPIO_PortAModeSet(GPIOA7, 5); // mclk 5:out  做master
		}else{
			GPIO_PortAModeSet(GPIOA7, 3); // mclk 3:in   做slave
		}
		GPIO_PortAModeSet(GPIOA8, 1);	  //lrclk
		GPIO_PortAModeSet(GPIOA9, 2);	  //bclk
		
		#if (defined(CFG_RES_AUDIO_I2SOUT_EN) && (CFG_RES_I2S == 1)) || defined(CFG_RES_AUDIO_I2S1OUT_EN)
		GPIO_PortAModeSet(GPIOA10, 4);	   //do
		#endif

		#if (defined(CFG_APP_I2SIN_MODE_EN) && (CFG_RES_I2S == 1)) || defined(CFG_RES_AUDIO_I2S1IN_EN)
		GPIO_PortAModeSet(GPIOA11, 2);	   //di
		#endif
	}	
}

#ifdef CFG_RES_AUDIO_I2SOUT_EN
void AudioI2sOutParamsSet(void)
{
	I2SParamCt i2s_set;
	i2s_set.IsMasterMode = CFG_RES_I2S_MODE;// 0:master 1:slave
	i2s_set.SampleRate = CFG_PARA_I2S_SAMPLERATE; //外设采样率
	i2s_set.I2sFormat = I2S_FORMAT_I2S;
#ifdef	CFG_AUDIO_WIDTH_24BIT
	i2s_set.I2sBits = I2S_LENGTH_24BITS;
#else
	i2s_set.I2sBits = I2S_LENGTH_16BITS;
#endif
	i2s_set.I2sTxRxEnable = 1;
#if (CFG_RES_I2S == 0)
	i2s_set.TxPeripheralID = PERIPHERAL_ID_I2S0_TX;
#else
	i2s_set.TxPeripheralID = PERIPHERAL_ID_I2S1_TX;
#endif
	i2s_set.TxBuf = (void*)mainAppCt.I2SFIFO;

	i2s_set.TxLen = mainAppCt.I2SFIFO_LEN;

	I2S_GPIO_Port_ModeSet(CFG_RES_I2S_MODULE, CFG_RES_I2S_MODE);

	I2S_AlignModeSet(CFG_RES_I2S_MODULE, I2S_LOW_BITS_ACTIVE);
	AudioI2S_Init(CFG_RES_I2S_MODULE, &i2s_set);//
}
#endif


#ifdef CFG_FUNC_I2S_MIX_MODE

void AudioI2s0ParamsSet(void)
{
#ifdef CFG_RES_I2S0_EN
	I2SParamCt i2s0_set;
	i2s0_set.IsMasterMode = CFG_RES_I2S0_MIX_MODE;// 0:master 1:slave
	i2s0_set.SampleRate = CFG_PARA_I2S0_SAMPLE; //外设采样率
	i2s0_set.I2sFormat = I2S_FORMAT_I2S;
	
#ifdef	CFG_AUDIO_WIDTH_24BIT
	i2s0_set.I2sBits = I2S_LENGTH_24BITS;
#else
	i2s0_set.I2sBits = I2S_LENGTH_16BITS;
#endif

	#if defined (CFG_RES_AUDIO_I2S0IN_EN) && defined (CFG_RES_AUDIO_I2S0OUT_EN)
	i2s0_set.I2sTxRxEnable = 3;
	#elif defined (CFG_RES_AUDIO_I2S0IN_EN)
	i2s0_set.I2sTxRxEnable = 2;
	#elif defined (CFG_RES_AUDIO_I2S0OUT_EN)
	i2s0_set.I2sTxRxEnable = 1;
	#else
	i2s0_set.I2sTxRxEnable = 0;
	#endif

	i2s0_set.TxPeripheralID = PERIPHERAL_ID_I2S0_TX;
	i2s0_set.RxPeripheralID = PERIPHERAL_ID_I2S0_RX;
#ifdef CFG_RES_AUDIO_I2S0OUT_EN
	if (mainAppCt.I2S0_TX_FIFO == NULL){
		mainAppCt.I2S0_TX_FIFO = (uint32_t*)osPortMalloc(mainAppCt.I2S0_TX_FIFO_LEN);
	}
	if(mainAppCt.I2S0_TX_FIFO != NULL){
		memset(mainAppCt.I2S0_TX_FIFO, 0, mainAppCt.I2S0_TX_FIFO_LEN);
	}else{
		APP_DBG("malloc I2S0_TX_FIFO error\n");
		return;
	}

	i2s0_set.TxBuf = (void*)mainAppCt.I2S0_TX_FIFO;
	i2s0_set.TxLen = mainAppCt.I2S0_TX_FIFO_LEN;
#endif

#ifdef CFG_RES_AUDIO_I2S0IN_EN
	if (mainAppCt.I2S0_RX_FIFO == NULL){
		mainAppCt.I2S0_RX_FIFO = (uint32_t*)osPortMalloc(mainAppCt.I2S0_RX_FIFO_LEN);
	}
	if(mainAppCt.I2S0_RX_FIFO != NULL){
		memset(mainAppCt.I2S0_RX_FIFO, 0, mainAppCt.I2S0_RX_FIFO_LEN);
	}else{
		APP_DBG("malloc I2S0_RX_FIFO error\n");
		return;
	}	

	i2s0_set.RxBuf = (void*)mainAppCt.I2S0_RX_FIFO;
	i2s0_set.RxLen = mainAppCt.I2S0_RX_FIFO_LEN;
#endif

	I2S_GPIO_Port_ModeSet(I2S0_MODULE, CFG_RES_I2S0_MIX_MODE);

	I2S_ModuleDisable(I2S0_MODULE);
	I2S_AlignModeSet(I2S0_MODULE, I2S_LOW_BITS_ACTIVE);
	AudioI2S_Init(I2S0_MODULE, &i2s0_set);
#endif
}


void AudioI2s1ParamsSet(void)
{
#ifdef CFG_RES_I2S1_EN
	I2SParamCt i2s1_set;
	i2s1_set.IsMasterMode = CFG_RES_I2S1_MIX_MODE;// 0:master 1:slave
	i2s1_set.SampleRate = CFG_PARA_I2S1_SAMPLE; //外设采样率
	i2s1_set.I2sFormat = I2S_FORMAT_I2S;
	
#ifdef	CFG_AUDIO_WIDTH_24BIT
	i2s1_set.I2sBits = I2S_LENGTH_24BITS;
#else
	i2s1_set.I2sBits = I2S_LENGTH_16BITS;
#endif

	#if defined (CFG_RES_AUDIO_I2S1IN_EN) && defined (CFG_RES_AUDIO_I2S1OUT_EN)
	i2s1_set.I2sTxRxEnable = 3;
	#elif defined (CFG_RES_AUDIO_I2S1IN_EN)
	i2s1_set.I2sTxRxEnable = 2;
	#elif defined (CFG_RES_AUDIO_I2S1OUT_EN)
	i2s1_set.I2sTxRxEnable = 1;
	#else
	i2s1_set.I2sTxRxEnable = 0;
	#endif

	i2s1_set.TxPeripheralID = PERIPHERAL_ID_I2S1_TX;
	i2s1_set.RxPeripheralID = PERIPHERAL_ID_I2S1_RX;

#ifdef CFG_RES_AUDIO_I2S1OUT_EN
	if (mainAppCt.I2S1_TX_FIFO == NULL){
		mainAppCt.I2S1_TX_FIFO = (uint32_t*)osPortMalloc(mainAppCt.I2S1_TX_FIFO_LEN);
	}
	if(mainAppCt.I2S1_TX_FIFO != NULL){
		memset(mainAppCt.I2S1_TX_FIFO, 0, mainAppCt.I2S1_TX_FIFO_LEN);
	}else{
		APP_DBG("malloc I2S1_TX_FIFO error\n");
		return;
	}

	i2s1_set.TxBuf = (void*)mainAppCt.I2S1_TX_FIFO;
	i2s1_set.TxLen = mainAppCt.I2S1_TX_FIFO_LEN;
#endif

#ifdef CFG_RES_AUDIO_I2S1IN_EN
	if (mainAppCt.I2S1_RX_FIFO == NULL){
		mainAppCt.I2S1_RX_FIFO = (uint32_t*)osPortMalloc(mainAppCt.I2S1_RX_FIFO_LEN);
	}
	if(mainAppCt.I2S1_RX_FIFO != NULL){
		memset(mainAppCt.I2S1_RX_FIFO, 0, mainAppCt.I2S1_RX_FIFO_LEN);
	}else{
		APP_DBG("malloc I2S1_RX_FIFO error\n");
		return;
	}	
	i2s1_set.RxBuf = (void*)mainAppCt.I2S1_RX_FIFO;
	i2s1_set.RxLen = mainAppCt.I2S1_RX_FIFO_LEN;
#endif

	I2S_GPIO_Port_ModeSet(I2S1_MODULE, CFG_RES_I2S1_MIX_MODE);

	I2S_ModuleDisable(I2S1_MODULE);
	I2S_AlignModeSet(I2S1_MODULE, I2S_LOW_BITS_ACTIVE);
	AudioI2S_Init(I2S1_MODULE, &i2s1_set);
#endif
}


void I2s0MixModeAudioCoreInit(void)
{
	AudioCoreIO	AudioIOSet;
	
#ifdef CFG_RES_AUDIO_I2S0IN_EN
	if(!AudioCoreSourceIsInit(I2S0_SOURCE_NUM))
	{
		AudioI2s0ParamsSet();

		memset(&AudioIOSet, 0, sizeof(AudioCoreIO));

#if (CFG_RES_I2S0_MIX_MODE == 0) || !defined(CFG_FUNC_I2S0IN_SRA_EN)//master 或者关微调
		#if CFG_PARA_I2S0_SAMPLE == CFG_PARA_SAMPLE_RATE
		AudioIOSet.Adapt = STD;
		#else
		AudioIOSet.Adapt = SRC_ONLY;
		#endif
#else //slave
		#if CFG_PARA_I2S0_SAMPLE == CFG_PARA_SAMPLE_RATE
		AudioIOSet.Adapt = SRA_ONLY;//CLK_ADJUST_ONLY;//
		#else
		AudioIOSet.Adapt = SRC_SRA;//SRC_ADJUST;//
		#endif
#endif
		AudioIOSet.Sync = (!CFG_RES_I2S0_MIX_MODE); //TRUE;//FALSE;//
		AudioIOSet.Channels = 2;
		AudioIOSet.Net = DefaultNet;
		AudioIOSet.Depth = AudioCoreFrameSizeGet(DefaultNet) * 2;//sI2SInPlayCt->I2SFIFO1 采样点深度
		AudioIOSet.HighLevelCent = 60;
		AudioIOSet.LowLevelCent = 40;
		AudioIOSet.SampleRate = CFG_PARA_I2S0_SAMPLE;//根据实际外设选择

		AudioIOSet.DataIOFunc = AudioI2S0_DataGet ;
		AudioIOSet.LenGetFunc = AudioI2S0_DataLenGet;

		#if (defined(CFG_AUDIO_WIDTH_24BIT) && (CFG_FUNC_I2S1_MIX_EN == 0)) || defined(CFG_FUNC_I2S0_MIX_RECOD_EN) || !defined(BT_TWS_SUPPORT)
		AudioIOSet.IOBitWidth = 1;//0,16bit,1:24bit
		#else
		AudioIOSet.IOBitWidth = 0;//0,16bit,1:24bit
		#endif
		AudioIOSet.IOBitWidthConvFlag = 0;//tws不需要数据进行位宽扩展，会在TWS_SOURCE_NUM以后统一转成24bit

		if(!AudioCoreSourceInit(&AudioIOSet, I2S0_SOURCE_NUM))
		{
			DBG("I2S0_SOURCE_NUM source error!\n");
			return;
		}
	}
	AudioCoreSourceEnable(I2S0_SOURCE_NUM);
#endif

#ifdef CFG_RES_AUDIO_I2S0OUT_EN
	if(!AudioCoreSinkIsInit(AUDIO_I2S0_OUT_SINK_NUM))
	{
		memset(&AudioIOSet, 0, sizeof(AudioCoreIO));

#if (CFG_RES_I2S0_MIX_MODE == 0) || !defined(CFG_FUNC_I2S0OUT_SRA_EN)//master 或者关微调
		#if CFG_PARA_I2S0_SAMPLE == CFG_PARA_SAMPLE_RATE
		AudioIOSet.Adapt = STD;
		#else
		AudioIOSet.Adapt = SRC_ONLY;
		#endif
#else //slave
		#if CFG_PARA_I2S0_SAMPLE == CFG_PARA_SAMPLE_RATE
		AudioIOSet.Adapt = SRA_ONLY;//CLK_ADJUST_ONLY;//
		#else
		AudioIOSet.Adapt = SRC_SRA;//SRC_ADJUST;//
		#endif
#endif

		AudioIOSet.Sync = (!CFG_RES_I2S0_MIX_MODE); //TRUE;//I2S slave 时候如果master没有接，有可能会导致DAC也不出声音。
		AudioIOSet.Channels = 2;
		AudioIOSet.Net = DefaultNet;
		AudioIOSet.HighLevelCent = 60;
		AudioIOSet.LowLevelCent = 40;
		AudioIOSet.SampleRate = CFG_PARA_I2S0_SAMPLE;//根据实际外设选择

		#ifdef TWS_IIS0_OUT
		AudioIOSet.Resident = TRUE;
		AudioIOSet.Depth = TWS_SINK_DEV_FIFO_SAMPLES;
		#else
		AudioIOSet.Depth = AudioCoreFrameSizeGet(DefaultNet) * 2 ;
		#endif

		AudioIOSet.DataIOFunc = AudioI2S0_DataSet;
		AudioIOSet.LenGetFunc = AudioI2S0_DataSpaceLenGet;

		#ifdef CFG_AUDIO_WIDTH_24BIT
		AudioIOSet.IOBitWidth = PCM_DATA_24BIT_WIDTH;//0,16bit,1:24bit
		AudioIOSet.IOBitWidthConvFlag = 0;//不需要做位宽转换处理
		#endif
		
		if(!AudioCoreSinkInit(&AudioIOSet, AUDIO_I2S0_OUT_SINK_NUM))
		{
			DBG("I2S0 out init error");
			return;
		}
		
		AudioI2s0ParamsSet();
		AudioCoreSinkEnable(AUDIO_I2S0_OUT_SINK_NUM);
		AudioCoreSinkAdjust(AUDIO_I2S0_OUT_SINK_NUM, TRUE);

		mainAppCt.tws_device_init_flag = TRUE;
	}
	else//sam add,20230221
	{
		I2S_SampleRateSet(I2S0_MODULE, CFG_PARA_SAMPLE_RATE);
		#ifdef	CFG_AUDIO_WIDTH_24BIT
		AudioIOSet.IOBitWidth = PCM_DATA_24BIT_WIDTH;//0,16bit,1:24bit
		AudioIOSet.IOBitWidthConvFlag = 0;//DAC 24bit ,sink最后一级输出时需要转变为24bi
		AudioCore.AudioSink[AUDIO_I2S0_OUT_SINK_NUM].BitWidth = AudioIOSet.IOBitWidth;
		AudioCore.AudioSink[AUDIO_I2S0_OUT_SINK_NUM].BitWidthConvFlag = AudioIOSet.IOBitWidthConvFlag;
		AudioCore.AudioSink[AUDIO_I2S0_OUT_SINK_NUM].Sync = (!CFG_RES_I2S0_MIX_MODE);
		#endif
	}	
#endif
}

void I2s1MixModeAudioCoreInit(void)
{
	AudioCoreIO	AudioIOSet;
	
#ifdef CFG_RES_AUDIO_I2S1IN_EN
	if(!AudioCoreSourceIsInit(I2S1_SOURCE_NUM))
	{
		AudioI2s1ParamsSet();
		memset(&AudioIOSet, 0, sizeof(AudioCoreIO));

#if CFG_RES_I2S1_MIX_MODE == 0 || !defined(CFG_FUNC_I2S1IN_SRA_EN)//master 或者关微调
		#if CFG_PARA_I2S1_SAMPLE == CFG_PARA_SAMPLE_RATE
		AudioIOSet.Adapt = STD;
		#else
		AudioIOSet.Adapt = SRC_ONLY;
		#endif
#else //slave
		#if CFG_PARA_I2S1_SAMPLE == CFG_PARA_SAMPLE_RATE
		AudioIOSet.Adapt = SRA_ONLY;//CLK_ADJUST_ONLY;//
		#else
		AudioIOSet.Adapt = SRC_SRA;//SRC_ADJUST;//
		#endif
#endif

		AudioIOSet.Sync = (!CFG_RES_I2S1_MIX_MODE); //TRUE;//FALSE;//
		AudioIOSet.Channels = 2;
		AudioIOSet.Net = DefaultNet;
		AudioIOSet.Depth = AudioCoreFrameSizeGet(DefaultNet) * 2;//sI2SInPlayCt->I2SFIFO1 采样点深度
		AudioIOSet.HighLevelCent = 60;
		AudioIOSet.LowLevelCent = 40;
		AudioIOSet.SampleRate = CFG_PARA_I2S1_SAMPLE;//根据实际外设选择

		AudioIOSet.DataIOFunc = AudioI2S1_DataGet ;
		AudioIOSet.LenGetFunc = AudioI2S1_DataLenGet;

		#if (defined(CFG_AUDIO_WIDTH_24BIT) && (CFG_FUNC_I2S1_MIX_EN == 0)) || defined(CFG_FUNC_I2S1_MIX_RECOD_EN) || !defined(BT_TWS_SUPPORT)
		AudioIOSet.IOBitWidth = PCM_DATA_24BIT_WIDTH;//0,16bit,1:24bit
		#else
		AudioIOSet.IOBitWidth = PCM_DATA_16BIT_WIDTH;//0,16bit,1:24bit
		#endif
		AudioIOSet.IOBitWidthConvFlag = 0;//tws不需要数据进行位宽扩展，会在TWS_SOURCE_NUM以后统一转成24bit

		if(!AudioCoreSourceInit(&AudioIOSet, I2S1_SOURCE_NUM))
		{
			DBG("I2S1_SOURCE_NUM source error!\n");
			return;
		}
	}
	AudioCoreSourceEnable(I2S1_SOURCE_NUM);
#endif

#ifdef CFG_RES_AUDIO_I2S1OUT_EN
	if(!AudioCoreSinkIsInit(AUDIO_I2S1_OUT_SINK_NUM))
	{
		memset(&AudioIOSet, 0, sizeof(AudioCoreIO));

#if CFG_RES_I2S1_MIX_MODE == 0 || !defined(CFG_FUNC_I2S1OUT_SRA_EN)//master 或者关微调
		#if CFG_PARA_I2S1_SAMPLE == CFG_PARA_SAMPLE_RATE
		AudioIOSet.Adapt = STD;
		#else
		AudioIOSet.Adapt = SRC_ONLY;
		#endif
#else //slave
		#if CFG_PARA_I2S1_SAMPLE == CFG_PARA_SAMPLE_RATE
		AudioIOSet.Adapt = SRA_ONLY;//CLK_ADJUST_ONLY;//
		#else
		AudioIOSet.Adapt = SRC_SRA;//SRC_ADJUST;//
		#endif
#endif

		AudioIOSet.Sync = (!CFG_RES_I2S1_MIX_MODE);	//I2S slave 时候如果master没有接，有可能会导致DAC也不出声音。
		AudioIOSet.Channels = 2;
		AudioIOSet.Net = DefaultNet;
		AudioIOSet.HighLevelCent = 60;
		AudioIOSet.LowLevelCent = 40;
		AudioIOSet.SampleRate = CFG_PARA_I2S1_SAMPLE;//根据实际外设选择

		#ifdef TWS_IIS1_OUT
		AudioIOSet.Depth = TWS_SINK_DEV_FIFO_SAMPLES;
		AudioIOSet.Resident = TRUE;
		#else
		AudioIOSet.Depth = AudioCoreFrameSizeGet(DefaultNet) * 2 ;
		#endif
		AudioIOSet.DataIOFunc = AudioI2S1_DataSet;
		AudioIOSet.LenGetFunc = AudioI2S1_DataSpaceLenGet;

#ifdef	CFG_AUDIO_WIDTH_24BIT
		AudioIOSet.IOBitWidth = PCM_DATA_24BIT_WIDTH;//0,16bit,1:24bit
		AudioIOSet.IOBitWidthConvFlag = 0;//不需要做位宽转换处理
#endif
		if(!AudioCoreSinkInit(&AudioIOSet, AUDIO_I2S1_OUT_SINK_NUM))
		{
			DBG("I2S1 out init error");
			return;
		}
		
		AudioI2s1ParamsSet();
		AudioCoreSinkEnable(AUDIO_I2S1_OUT_SINK_NUM);
		AudioCoreSinkAdjust(AUDIO_I2S1_OUT_SINK_NUM, TRUE);

		mainAppCt.tws_device_init_flag = TRUE;
	}
	else//sam add,20230221
	{
		I2S_SampleRateSet(I2S1_MODULE, CFG_PARA_SAMPLE_RATE);
		#ifdef	CFG_AUDIO_WIDTH_24BIT
		AudioIOSet.IOBitWidth = PCM_DATA_24BIT_WIDTH;//0,16bit,1:24bit
		AudioIOSet.IOBitWidthConvFlag = 0;//DAC 24bit ,sink最后一级输出时需要转变为24bi
		AudioCore.AudioSink[AUDIO_I2S1_OUT_SINK_NUM].BitWidth = AudioIOSet.IOBitWidth;
		AudioCore.AudioSink[AUDIO_I2S1_OUT_SINK_NUM].BitWidthConvFlag = AudioIOSet.IOBitWidthConvFlag;
		AudioCore.AudioSink[AUDIO_I2S1_OUT_SINK_NUM].Sync = (!CFG_RES_I2S1_MIX_MODE);
		#endif
	}	
#endif
}


void I2sMixModeInit_HFP(void)
{
	AudioCoreIO	AudioIOSet;
	
#ifdef CFG_RES_AUDIO_I2S0OUT_EN
	AudioCoreSinkDeinit(AUDIO_I2S0_OUT_SINK_NUM);
	if(!AudioCoreSinkIsInit(AUDIO_I2S0_OUT_SINK_NUM))
	{
		memset(&AudioIOSet, 0, sizeof(AudioCoreIO));

#if CFG_RES_I2S0_MIX_MODE == 0 || !defined(CFG_FUNC_I2S_OUT_SYNC_EN)// Master 或不开微调
		#if CFG_PARA_I2S0_SAMPLE == CFG_BTHF_PARA_SAMPLE_RATE
		AudioIOSet.Adapt = STD;//SRC_ONLY
		#else
		AudioIOSet.Adapt = SRC_ONLY;
		#endif
#else//slave
		#if CFG_PARA_I2S0_SAMPLE == CFG_BTHF_PARA_SAMPLE_RATE
		AudioIOSet.Adapt = SRA_ONLY;//CLK_ADJUST_ONLY;
		#else
		AudioIOSet.Adapt = SRC_SRA;//SRC_ADJUST;
		#endif
#endif

		AudioIOSet.Sync = (!CFG_RES_I2S0_MIX_MODE);	//I2S slave 时候如果master没有接，有可能会导致DAC也不出声音。
		AudioIOSet.Channels = 2;
		AudioIOSet.Net = DefaultNet;
		AudioIOSet.HighLevelCent = 60;
		AudioIOSet.LowLevelCent = 40;
		AudioIOSet.SampleRate = CFG_PARA_I2S0_SAMPLE;//根据实际外设选择

		#ifdef TWS_IIS0_OUT
		AudioIOSet.Resident = TRUE;
		AudioIOSet.Depth = TWS_SINK_DEV_FIFO_SAMPLES;
		#else
		AudioIOSet.Depth = AudioCoreFrameSizeGet(DefaultNet) * 2 ;
		#endif

		AudioIOSet.DataIOFunc = AudioI2S0_DataSet;
		AudioIOSet.LenGetFunc = AudioI2S0_DataSpaceLenGet;

		#ifdef CFG_AUDIO_WIDTH_24BIT
		AudioIOSet.IOBitWidth = PCM_DATA_16BIT_WIDTH;//0,16bit,1:24bit
		AudioIOSet.IOBitWidthConvFlag = 1;//不需要做位宽转换处理
		#endif
		
		if(!AudioCoreSinkInit(&AudioIOSet, AUDIO_I2S0_OUT_SINK_NUM))
		{
			DBG("I2S0 out init error");
			return;
		}
		
		AudioI2s0ParamsSet();
		AudioCoreSinkEnable(AUDIO_I2S0_OUT_SINK_NUM);
		AudioCoreSinkAdjust(AUDIO_I2S0_OUT_SINK_NUM, TRUE);
	}
	else//sam add,20230221
	{
		I2S_SampleRateSet(I2S0_MODULE, CFG_PARA_I2S0_SAMPLE);
		#ifdef	CFG_AUDIO_WIDTH_24BIT
		AudioIOSet.IOBitWidth = PCM_DATA_16BIT_WIDTH;//0,16bit,1:24bit
		AudioIOSet.IOBitWidthConvFlag = 1;//DAC 24bit ,sink最后一级输出时需要转变为24bi
		AudioCore.AudioSink[AUDIO_I2S0_OUT_SINK_NUM].BitWidth = AudioIOSet.IOBitWidth;
		AudioCore.AudioSink[AUDIO_I2S0_OUT_SINK_NUM].BitWidthConvFlag = AudioIOSet.IOBitWidthConvFlag;
		AudioCore.AudioSink[AUDIO_I2S0_OUT_SINK_NUM].Sync = (!CFG_RES_I2S0_MIX_MODE);
		#endif
	}	
#endif

#ifdef CFG_RES_AUDIO_I2S1OUT_EN
	AudioCoreSinkDeinit(AUDIO_I2S1_OUT_SINK_NUM);
	if(!AudioCoreSinkIsInit(AUDIO_I2S1_OUT_SINK_NUM))
	{
		memset(&AudioIOSet, 0, sizeof(AudioCoreIO));
		
#if CFG_RES_I2S1_MIX_MODE == 0 || !defined(CFG_FUNC_I2S_OUT_SYNC_EN)// Master 或不开微调
		#if CFG_PARA_I2S1_SAMPLE == CFG_BTHF_PARA_SAMPLE_RATE
		AudioIOSet.Adapt = STD;//SRC_ONLY
		#else
		AudioIOSet.Adapt = SRC_ONLY;
		#endif
#else//slave
		#if CFG_PARA_I2S1_SAMPLE == CFG_BTHF_PARA_SAMPLE_RATE
		AudioIOSet.Adapt = SRA_ONLY;//CLK_ADJUST_ONLY;
		#else
		AudioIOSet.Adapt = SRC_SRA;//SRC_ADJUST;
		#endif
#endif

		AudioIOSet.Sync = (!CFG_RES_I2S1_MIX_MODE);	//I2S slave 时候如果master没有接，有可能会导致DAC也不出声音。
		AudioIOSet.Channels = 2;
		AudioIOSet.Net = DefaultNet;
		AudioIOSet.HighLevelCent = 60;
		AudioIOSet.LowLevelCent = 40;
		AudioIOSet.SampleRate = CFG_PARA_I2S1_SAMPLE;//根据实际外设选择

		#ifdef TWS_IIS1_OUT
		AudioIOSet.Resident = TRUE;
		AudioIOSet.Depth = TWS_SINK_DEV_FIFO_SAMPLES;
		#else
		AudioIOSet.Depth = AudioCoreFrameSizeGet(DefaultNet) * 2 ;
		#endif

		AudioIOSet.DataIOFunc = AudioI2S1_DataSet;
		AudioIOSet.LenGetFunc = AudioI2S1_DataSpaceLenGet;

		#ifdef CFG_AUDIO_WIDTH_24BIT
		AudioIOSet.IOBitWidth = PCM_DATA_16BIT_WIDTH;//0,16bit,1:24bit
		AudioIOSet.IOBitWidthConvFlag = 1;//不需要做位宽转换处理
		#endif
		
		if(!AudioCoreSinkInit(&AudioIOSet, AUDIO_I2S1_OUT_SINK_NUM))
		{
			DBG("I2S1 out init error");
			return;
		}
		
		AudioI2s1ParamsSet();
		AudioCoreSinkEnable(AUDIO_I2S1_OUT_SINK_NUM);
		AudioCoreSinkAdjust(AUDIO_I2S1_OUT_SINK_NUM, TRUE);
	}
	else//sam add,20230221
	{
		I2S_SampleRateSet(I2S1_MODULE, CFG_PARA_I2S1_SAMPLE);
		#ifdef	CFG_AUDIO_WIDTH_24BIT
		AudioIOSet.IOBitWidth = PCM_DATA_16BIT_WIDTH;//0,16bit,1:24bit
		AudioIOSet.IOBitWidthConvFlag = 1;//DAC 24bit ,sink最后一级输出时需要转变为24bi
		AudioCore.AudioSink[AUDIO_I2S1_OUT_SINK_NUM].BitWidth = AudioIOSet.IOBitWidth;
		AudioCore.AudioSink[AUDIO_I2S1_OUT_SINK_NUM].BitWidthConvFlag = AudioIOSet.IOBitWidthConvFlag;
		AudioCore.AudioSink[AUDIO_I2S1_OUT_SINK_NUM].Sync = (!CFG_RES_I2S1_MIX_MODE);
		#endif
	}	
#endif
}

void I2sMixModeInit(void)
{
	#ifdef CFG_RES_I2S0_EN
	I2s0MixModeAudioCoreInit();
	#endif

	#ifdef CFG_RES_I2S1_EN
	I2s1MixModeAudioCoreInit();
	#endif
}

void I2sMixModeDeInit(void)
{
#ifdef CFG_RES_AUDIO_I2S0IN_EN
	if(AudioCoreSourceIsInit(I2S0_SOURCE_NUM))
	{
		AudioCoreSourceDisable(I2S0_SOURCE_NUM);
		AudioCoreSourceDeinit(I2S0_SOURCE_NUM);

		if(mainAppCt.I2S0_RX_FIFO != NULL)
		{
			APP_DBG("I2S0_RX_FIFO\n");
			osPortFree(mainAppCt.I2S0_RX_FIFO);
			mainAppCt.I2S0_RX_FIFO = NULL;
		}		
	}
#endif

#if defined(CFG_RES_AUDIO_I2S0OUT_EN) && !defined(TWS_IIS0_OUT)
	I2S_ModuleDisable(I2S0_MODULE);
	RST_I2SModule(I2S0_MODULE);

	DMA_InterruptFlagClear(PERIPHERAL_ID_I2S0_TX, DMA_DONE_INT);
	DMA_InterruptFlagClear(PERIPHERAL_ID_I2S0_TX, DMA_THRESHOLD_INT);
	DMA_InterruptFlagClear(PERIPHERAL_ID_I2S0_TX, DMA_ERROR_INT);
	DMA_ChannelDisable(PERIPHERAL_ID_I2S0_TX);

	if(mainAppCt.I2S0_TX_FIFO != NULL)
	{
		APP_DBG("I2S0_TX_FIFO\n");
		osPortFree(mainAppCt.I2S0_TX_FIFO);
		mainAppCt.I2S0_TX_FIFO = NULL;
	}
	AudioCoreSinkDeinit(AUDIO_I2S0_OUT_SINK_NUM);
#endif


#ifdef CFG_RES_AUDIO_I2S1IN_EN
	if(AudioCoreSourceIsInit(I2S1_SOURCE_NUM))
	{
		AudioCoreSourceDisable(I2S1_SOURCE_NUM);
		AudioCoreSourceDeinit(I2S1_SOURCE_NUM);

		if(mainAppCt.I2S1_RX_FIFO != NULL)
		{
			APP_DBG("I2S1_RX_FIFO\n");
			osPortFree(mainAppCt.I2S1_RX_FIFO);
			mainAppCt.I2S1_RX_FIFO = NULL;
		}	
	}
#endif	

#if defined(CFG_RES_AUDIO_I2S1OUT_EN) && !defined(TWS_IIS1_OUT)
	I2S_ModuleDisable(I2S1_MODULE);
	RST_I2SModule(I2S1_MODULE);

	DMA_InterruptFlagClear(PERIPHERAL_ID_I2S1_TX, DMA_DONE_INT);
	DMA_InterruptFlagClear(PERIPHERAL_ID_I2S1_TX, DMA_THRESHOLD_INT);
	DMA_InterruptFlagClear(PERIPHERAL_ID_I2S1_TX, DMA_ERROR_INT);
	DMA_ChannelDisable(PERIPHERAL_ID_I2S1_TX);

	if(mainAppCt.I2S1_TX_FIFO != NULL)
	{
		APP_DBG("I2S1_TX_FIFO\n");
		osPortFree(mainAppCt.I2S1_TX_FIFO);
		mainAppCt.I2S1_TX_FIFO = NULL;
	}
	AudioCoreSinkDeinit(AUDIO_I2S1_OUT_SINK_NUM);
#endif
}

#endif
