
/**
 **************************************************************************************
 * @file    audio_effect_param.h
 * @brief
 *
 * @author  Zheng
 * @version V2.0.0
 *
 * &copy; Shanghai Mountain View Silicon Technology Co.,Ltd. All rights reserved.
 **************************************************************************************
*/
        
#ifndef __AUDIO_EFFECT_PARAM_H__
#define __AUDIO_EFFECT_PARAM_H__

#ifdef  __cplusplus
extern "C" {
#endif//__cplusplus

#include "type.h"
#include "audio_effect_api.h"

typedef struct _AudioEffectCommParamBuffContext
{
	GainControlParam		paramGainControl_0;
	GainControlParam		paramGainControl_1;
	GainControlParam		paramGainControl_2;
	GainControlParam		paramGainControl_3;
	GainControlParam		paramGainControl_4;
	GainControlParam		paramGainControl_5;
	GainControlParam		paramGainControl_6;
	ExpanderParam		paramExpander_7;
	SilenceDetectorParam		paramSilenceDetector_8;
	CompanderParam		paramCompander_9;
	LLCompressorParam		paramLLCompressor_10;
	EQParam		paramEQ_11;
	VBParam		paramVB_12;
	VBClassicParam		paramVBClassic_13;
	ExciterParam		paramExciter_14;
	EQParam		paramEQ_15;
	EQParam		paramEQ_16;
	EQDRCParam		paramEQDRC_17;
	EQParam		paramEQ_18;
	EQDRCParam		paramEQDRC_19;
	LLCompressorParam		paramLLCompressor_20;
	PhaseControlParam		paramPhaseControl_21;
	VBParam		paramVB_22;
	VBClassicParam		paramVBClassic_23;
	EQDRCParam		paramEQDRC_24;
	EQParam		paramEQ_25;
}AudioEffectCommParamBuffContext;

typedef struct _AudioEffectAECParam_HFPBuffContext
{
	ExpanderParam		paramExpander_0;
	EQParam		paramEQ_1;
	AECParam		paramAEC_2;
	GainControlParam		paramGainControl_3;
	DRCParam		paramDRC_4;
}AudioEffectAECParam_HFPBuffContext;


#ifdef  __cplusplus
}
#endif//__cplusplus

#endif//__AUDIO_EFFECT_PARAM_H__

