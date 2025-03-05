/******************************************************************************
 * @file    app_config.h
 * @author
 * @version V_NEW
 * @date    2021-06-25
 * @maintainer
 * @brief
 ******************************************************************************
 * @attention
 * <h2><center>&copy; COPYRIGHT 2013 MVSilicon </center></h2>
 */
#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

#include "type.h"
//************************************************************************************************************
//    本系统默认开启2个系统全局宏，在IDE工程配置(Build Settings-Compiler-Symbols)，此处用于提醒
//*CFG_APP_CONFIG 和 FUNC_OS_EN*/
//************************************************************************************************************

//************************************************************************************************************
//    功能开关说明：
// *CFG_APP_*  : 软件系统应用模式开启，比如USB U盘播放歌曲应用模式的选择
// *CFG_FUNC_* : 软件功能开关
// *CFG_PARA_* : 系统软件参数设置
// *CFG_RES_*  : 系统硬件资源配置
// ************************************************************************************************************


//24bit 音效开关宏控制
//24bit SDK RAM开销会大幅度增加
#define  CFG_AUDIO_WIDTH_24BIT


//****************************************************************************************
//       芯片型号选择配置
// 应用开发时，一定要选对型号,防止上位机显示的mic和linein输入和硬件接口不一致
//****************************************************************************************
//#define  CFG_CHIP_BP10128
//#define  CFG_CHIP_BP1064A2	// flash容量 2M
//#define  CFG_CHIP_BP1064L2	// flash容量 2M
//#define  CFG_CHIP_BP1048A2	// flash容量 2M
#define  CFG_CHIP_BP1048B2   	// flash容量 2M
//#define  CFG_CHIP_BP1048B1   	// flash容量 1M  客户需自行把代码控制在 1M 以内
//#define  CFG_CHIP_MT166      	// flash容量 1M  客户需自行把代码控制在 1M 以内
//#define  CFG_CHIP_BP1032A1   	// flash容量 1M  客户需自行把代码控制在 1M 以内
//#define  CFG_CHIP_BP1032A2   	// flash容量 2M
//#define  CFG_CHIP_BP1048P2    // flash容量 2M
//#define  CFG_CHIP_BP1048P4    // flash容量 4M

//****************************************************************************************
//       针对1MB FLASH芯片配置
//****************************************************************************************
/**音频SDK版本号：V1.0.0**/
/*0xB1：芯片B1X，01：大版本号， 00：小版本号， 00：用户修订号（由用户设定，可结合补丁号）；实际存储字节序：1A 01 00 00 ，略区别于sdk版本*/
/*开启flash_boot时，用flashboot升级usercode后，boot明码区和code明码(如0xB8和0xB8+0x10000)两个值会不同，前者是burner烧录时版本，后者是mva版本需关注*/
#define	 CFG_SDK_VER_CHIPID			(0xB1)
#define  CFG_SDK_MAJOR_VERSION		(0)
#define  CFG_SDK_MINOR_VERSION		(5)
#define  CFG_SDK_PATCH_VERSION	    (2)

//****************************************************************************************
//       B0,B1口作为DEBUG仿真功能选择配置
// 说明:
//    1.若用到linein4(B0,B1),为了防止转到linein4通道后无法仿真，需要开启此宏；
//    2.若用到linein4(B0,B1),量产时需要关闭此宏，否则转到此通道时会有POP声；
//****************************************************************************************
#define CFG_FUNC_SW_DEBUG_EN

//****************************************************************************************
//	低优先级任务，用于擦写flash，避免系统阻塞
//****************************************************************************************
#define CFG_FUNC_OPEN_SLOW_DEVICE_TASK

//****************************************************************************************
// 系统App功能模式选择
//****************************************************************************************
#define CFG_APP_IDLE_MODE_EN			//-------开关机模式--此模式不可关闭-//
#define CFG_APP_BT_MODE_EN				//-------蓝牙功能-----------------//
#define CFG_APP_USB_PLAY_MODE_EN		//-------UDisk播放模式------------//
#define CFG_APP_CARD_PLAY_MODE_EN		//-------SD Card播放模式----------//
#define CFG_APP_RADIOIN_MODE_EN			//-------收音机功能---------------//

//****************************************************************************************
// 说明:
// 		1.LINE IN输入跟LINE IN MIX只能二选一，可以都不用
// 		2.要是需要两路line in同时输入，另一路请选用MIC通路
//		3.LINE IN MIX 默认经过系统音效处理
//****************************************************************************************
#define CFG_APP_LINEIN_MODE_EN			//-------LINE输入模式-------------//
//#define CFG_FUNC_LINE_MIX_MODE		//-------LINE混合输入功能----------//

//****************************************************************************************
// 说明:
// 		1.I2s IN输入跟I2s MIX可同时开也可单独使用，配置不冲突即可  也可以都不用
//		2.i2s mix in是否经过系统音效处理在后面的参数中可自行配置
//****************************************************************************************
#define CFG_APP_I2SIN_MODE_EN			//-------I2S IN输入模式-----------//
//#define CFG_FUNC_I2S_MIX_MODE			//-------I2S MIX模式--------------//

//****************************************************************************************
// 说明:
// 		1.光纤/同轴 输入跟SPDIF MIX只能二选一，可以都不用
//		2.SPDIF MIX 可用于光纤或者同轴，二选一，不能同时存在
//		3.SPDIF MIX 默认经过系统音效处理
//****************************************************************************************
#define CFG_APP_OPTICAL_MODE_EN			//-------SPDIF 光纤模式------------//
#define CFG_APP_COAXIAL_MODE_EN			//-------SPDIF 同轴模式------------//
//#define CFG_FUNC_SPDIF_MIX_MODE		//-------SPDIF 混合输入功能---------//


//#define CFG_APP_HDMIIN_MODE_EN 		//-------HDMI IN模式---------------//
#define CFG_APP_USB_AUDIO_MODE_EN		//-------USB声卡，读卡器，一线通功能---//


#if defined(CFG_APP_LINEIN_MODE_EN) || defined(CFG_FUNC_LINE_MIX_MODE)
	#if defined(CFG_CHIP_BP1064A2) || defined(CFG_CHIP_BP1064L2) || defined(CFG_CHIP_BP10128)
	#define LINEIN_INPUT_CHANNEL				ANA_INPUT_CH_LINEIN1
	#elif defined(CFG_CHIP_BP1032)
	#define LINEIN_INPUT_CHANNEL				ANA_INPUT_CH_LINEIN4
	#else
	#define LINEIN_INPUT_CHANNEL				ANA_INPUT_CH_LINEIN5
	#endif
#endif

#ifdef CFG_APP_RADIOIN_MODE_EN
    #define FUNC_RADIO_RDA5807_EN
    //#define FUNC_RADIO_QN8035_EN  //芯片io时钟只支持12M，qn8035必须外挂晶振

	#if defined(FUNC_RADIO_RDA5807_EN) && defined(FUNC_RADIO_QN8035_EN)
	#error Conflict: radio type error //不能同时选择两种外挂芯片
	#endif
#endif

#ifdef  CFG_APP_USB_AUDIO_MODE_EN
	#define CFG_PARA_USB_MODE	AUDIO_MIC
	#define CFG_RES_AUDIO_USB_IN_EN		//缺省 作为模式通路使能
	#define CFG_PARA_AUDIO_USB_IN_SYNC	//时钟偏差引起的 采样点同步
	#define CFG_PARA_AUDIO_USB_IN_SRC	//转采样准备

	#define CFG_RES_AUDIO_USB_OUT_EN
	#define CFG_PARA_AUDIO_USB_OUT_SYNC	//时钟偏差引起的 采样点同步
	#define CFG_PARA_AUDIO_USB_OUT_SRC	//转采样准备
	#define CFG_RES_AUDIO_USB_VOL_SET_EN
	//#define USB_READER_EN
#endif

//IDLE模式(假待机),powerkey/deepsleep可以同时选中也可以单独配置
//通过消息进入不同的模式
//MSG_DEEPSLEEP/MSG_POWER/MSG_POWERDOWN --> 进入IDLE模式(假待机)
//MSG_DEEPSLEEP --> 进入IDLE模式以后如果CFG_IDLE_MODE_DEEP_SLEEP打开进入deepsleep
//MSG_POWERDOWN --> 进入IDLE模式以后如果CFG_IDLE_MODE_POWER_KEY打开进入powerdown
#ifdef  CFG_APP_IDLE_MODE_EN
	#define CFG_IDLE_MODE_POWER_KEY	//power key
	#define CFG_IDLE_MODE_DEEP_SLEEP //deepsleep
	#ifdef CFG_IDLE_MODE_POWER_KEY
		#define BAKEUP_FIRST_ENTER_POWERDOWN		//第一次上电需要按下PowerKey
		#define POWERKEY_MODE		POWERKEY_MODE_SLIDE_SWITCH_LPD//POWERKEY_MODE_PUSH_BUTTON
		#if (POWERKEY_MODE == POWERKEY_MODE_SLIDE_SWITCH_LPD) || (POWERKEY_MODE == POWERKEY_MODE_SLIDE_SWITCH_HPD)
			#define POWERKEY_CNT					20
		#else
			#define POWERKEY_CNT					2000
		#endif

		#if (POWERKEY_MODE == POWERKEY_MODE_PUSH_BUTTON)
			//powerkey复用短按键功能。
			#define USE_POWERKEY_PUSH_BUTTON_MSG_SP		MSG_PLAY_PAUSE
		#endif	
	#endif
	#ifdef CFG_IDLE_MODE_DEEP_SLEEP
//		#define  CFG_FUNC_MAIN_DEEPSLEEP_EN		//系统开机先睡眠，等唤醒。

		/*红外按键唤醒,注意CFG_PARA_WAKEUP_GPIO_IR和 唤醒键IR_KEY_POWER设置*/
		#define CFG_PARA_WAKEUP_SOURCE_IR		SYSWAKEUP_SOURCE9_IR//固定source9
		/*ADCKey唤醒 配合CFG_PARA_WAKEUP_GPIO_ADCKEY 唤醒键ADCKEYWAKEUP设置及其电平*/
		#define CFG_PARA_WAKEUP_SOURCE_ADCKEY	SYSWAKEUP_SOURCE1_GPIO//如使用ADC唤醒，必须打开CFG_RES_ADC_KEY_SCAN 和CFG_RES_ADC_KEY_USE
		#define CFG_PARA_WAKEUP_SOURCE_CEC		SYSWAKEUP_SOURCE2_GPIO//HDMI专用，CFG_PARA_WAKEUP_GPIO_CEC
		#define CFG_PARA_WAKEUP_SOURCE_POWERKEY SYSWAKEUP_SOURCE6_POWERKEY
		#define CFG_PARA_WAKEUP_SOURCE_IOKEY1	SYSWAKEUP_SOURCE3_GPIO
		#define CFG_PARA_WAKEUP_SOURCE_IOKEY2	SYSWAKEUP_SOURCE4_GPIO
	#endif
#endif

#if  defined(CFG_CHIP_MT166) || defined(CFG_CHIP_BP1032A1) || defined(CFG_CHIP_BP1032A2)
	#define CFG_RES_MIC_SELECT      (1)//0=NO MIC, 1= MIC1, 2= MIC2, 3 = MCI1+MIC2
#else
	#define CFG_RES_MIC_SELECT      (3)//0=NO MIC, 1= MIC1, 2= MIC2, 3 = MCI1+MIC2
#endif

//****************************************************************************************
//                 音频输出通道相关配置
//说明:
//    如下输出源可同时输出；
//****************************************************************************************
/**DAC-0通道配置选择**/
#define CFG_RES_AUDIO_DAC0_EN

/**DAC-X通道配置选择**/
#define CFG_RES_AUDIO_DACX_EN

/**I2S音频输出通道配置选择**/
//#define CFG_RES_AUDIO_I2SOUT_EN

/**光纤同轴音频输出通道配置选择**/
//#define CFG_RES_AUDIO_SPDIFOUT_EN   // 此功能必须关闭TWS

#ifdef CFG_RES_AUDIO_SPDIFOUT_EN
	#ifdef CFG_AUDIO_WIDTH_24BIT
		#define CFG_AUDIO_SPDIFOUT_24BIT	//打开SPDIF_TX 24bit输出，关闭SPDIF_TX 16bit输出
	#endif

	#define CFG_AUDIO_SPDIFOUT_MEDIA_NO_SRC
	#ifdef CFG_AUDIO_SPDIFOUT_MEDIA_NO_SRC
	//usb/tf卡模式 SPDIFOUT跟随解码后的采样率，不做SRC处理
	//DAC0/DACX/MIC不支持 48K以上的采样率，需要关闭DAC0/DACX/MIC
		#undef 	CFG_RES_AUDIO_DAC0_EN
		#undef 	CFG_RES_AUDIO_DACX_EN
		#undef 	CFG_RES_MIC_SELECT
		#define CFG_RES_MIC_SELECT      (0)

		#define LOSSLESS_DECODER_HIGH_RESOLUTION
	#endif
#endif


/*
 * 时钟配置选择；可选择使用外部时钟
 * 注意:
 * 		1. 此配置针对i2s配置, 且必须i2s做slave才能使用; dac输出不可使用外部时钟
 *		2. 配置为 0 ,即使用内部时钟，系统默认配置，跟平常并无区别
 *		3. 配置为 1 ,即使用i2s0的mclk作为系统时钟
 *		4. 配置为 2 ,即使用i2s1的mclk作为系统时钟
 */
#define	USE_MCLK_IN_MODE					0

//#define CFG_AUDIO_OUT_AUTO_SAMPLE_RATE_44100_48000
//****************************************************************************************
//     I2S相关配置选择
//说明:
//    1.I2S输出也使能时端口选择和模式需要注意保持一致;
//	  2、缺省配置为Master，I2S外设音频使用Master的MCLK,否则对方不微调时钟就会有pop音。
//    3.谨慎开启I2S slave，I2S外设必须提供稳定时钟，如果外设连线工作不稳定，请设置AudioIOSet.Sync = FALSE；自行增加DetectLock调整Sync
//****************************************************************************************
#if defined(CFG_APP_I2SIN_MODE_EN) || defined(CFG_RES_AUDIO_I2SOUT_EN)
	#include"i2s.h"
	#define	CFG_RES_I2S 				1 // I2S0_MODULE:I2S0总线   I2S1_MODULE:I2S1总线;
	#define	CFG_RES_I2S_MODE			0 // 0: master mode;		1: slave mode 外设未接不要配slave

	#if (CFG_RES_I2S == 0)				  // MCLK-LRCLK-BCLK-DOUT-DIN Port 2/3缺端口和功能，谨慎配置
		#define	CFG_RES_I2S_MODULE		I2S0_MODULE
	// 	I2S1 GPIO具体配置在 i2s_mix_mode_set.c 中
	//	#define	CFG_RES_I2S_IO_PORT		0 // 0:i2S0_0-A0-1-2-3-4; 		1:I2S0_1-A24-A20-21-22-23; 	2:I2S0_2-X-X-X-A4-3;	3:I2S0_3-X-X-X-A23-22 //2和3 缺clk 代码中未配置
	#elif (CFG_RES_I2S == 1)
		#define	CFG_RES_I2S_MODULE		I2S1_MODULE
	// 	I2S1 GPIO具体配置在 i2s_mix_mode_set.c 中
	//	#define	CFG_RES_I2S_IO_PORT		1 // 0:i2S1_0-A27-28-29-30-31; 	1:I2S1_1-A7-8-9-10-11; 		2:I2S1_2-X-A1-2-31-30;	3:I2S1_3-X-A20-21-11-10
	#endif

	#define CFG_PARA_I2S_SAMPLERATE		44100 	// 采样率设置
	#define CFG_FUNC_I2S_IN_SYNC_EN				// I2S输入采样率微调,内部时钟,slaver时开启
	#define CFG_FUNC_I2S_OUT_SYNC_EN			// I2S输出采样率微调,内部时钟,slaver时开启
#endif

#ifdef CFG_FUNC_I2S_MIX_MODE
	#define	CFG_RES_I2S0_EN	 // 0:i2S0_0-A0-1-2-3-4; 	 1:I2S0_1-A24-A20-21-22-23;  2:I2S0_2-X-X-X-A4-3;	   3:I2S0_3-X-X-X-A23-22 //2和3 缺clk 代码中未配置
	#define	CFG_RES_I2S1_EN	 // 0:i2S1_0-A27-28-29-30-31;  1:I2S1_1-A7-8-9-10-11;      2:I2S1_2-X-A1-2-31-30;  3:I2S1_3-X-A20-21-11-10
	
	#ifdef CFG_RES_I2S0_EN
	#define CFG_RES_AUDIO_I2S0IN_EN             // I2S0 输入 使能
	#define CFG_RES_AUDIO_I2S0OUT_EN            // I2S0 输出 使能
	
	#define CFG_FUNC_I2S0IN_SRA_EN  			// I2S输入采样率微调,内部时钟,slaver时开启
	#define CFG_FUNC_I2S0OUT_SRA_EN  			// I2S输出采样率微调,内部时钟,slaver时开启

	#define	CFG_RES_I2S0_MIX_MODE		0 		// 0: master mode;		1: slave mode 外设未接不要配slave
	#define CFG_PARA_I2S0_SAMPLE  		44100 	// I2S0采样率
    #define CFG_FUNC_I2S0_MIX_EN        0		// 0 = 独立音效处理      	1 = 与music_pcm混合后一起处理音

	// I2S0 GPIO具体配置在 i2s_mix_mode_set.c 中
//  #define	CFG_RES_I2S0_IO_PORT		0		// 0:i2S0_0-A0-1-2-3-4;  1:I2S0_1-A24-A20-21-22-23;  2:I2S0_2-X-X-X-A4-3;  3:I2S0_3-X-X-X-A23-22 //2和3 缺clk 代码中未配置
	#endif
	
	#ifdef CFG_RES_I2S1_EN
	#define CFG_RES_AUDIO_I2S1IN_EN             // I2S1 输入 使能
	#define CFG_RES_AUDIO_I2S1OUT_EN            // I2S1 输出 使能

	#define CFG_FUNC_I2S1IN_SRA_EN  			// I2S输入采样率微调,内部时钟,slaver时开启
	#define CFG_FUNC_I2S1OUT_SRA_EN  			// I2S输出采样率微调,内部时钟,slaver时开启

	#define	CFG_RES_I2S1_MIX_MODE		0 		// 0: master mode;  	1: slave mode 外设未接不要配slave
	#define CFG_PARA_I2S1_SAMPLE  		44100 	// I2S1采样率
    #define CFG_FUNC_I2S1_MIX_EN        0		// 0 = 独立音效处理      	1 = 与music_pcm混合后一起处理音

	// I2S1 GPIO具体配置在 i2s_mix_mode_set.c 中
//  #define	CFG_RES_I2S1_IO_PORT		1		// 0:i2S1_0-A27-28-29-30-31; 1:I2S1_1-A7-8-9-10-11;  2:I2S1_2-X-A1-2-31-30;	3:I2S1_3-X-A20-21-11-10
	#endif
#endif

//****************************************************************************************
//                 解码器类型选择
//说明:
//    如下解码器类型选择会影响code size;
//****************************************************************************************
//#define LOSSLESS_DECODER_HIGH_RESOLUTION//打开后支持高采样率解码，资源消耗较大请自行评估 (不关闭音效可能出现卡顿现象)

#define USE_MP3_DECODER
#define USE_WMA_DECODER
#define USE_SBC_DECODER
#define USE_WAV_DECODER
//#define USE_DTS_DECODER
#define USE_FLAC_DECODER	//24bit 1.5Mbps高码率时，需要扩大DECODER_FIFO_SIZE_FOR_PLAYER 输出fifo，或扩大输入：FLAC_INPUT_BUFFER_CAPACITY
//#define USE_AAC_DECODER
//#define USE_AIF_DECODER
//#define USE_AMR_DECODER
#define USE_APE_DECODER

//****************************************************************************************
//                 总音效功能配置
//****************************************************************************************
//高低音调节功能配置说明:
//    1.此功能是基于MIC OUT EQ进行手动设置的，需要在调音参数中使能此EQ；
//    2.默认f5对应bass gain,f6对应treb gain,若调音界面上修改此EQ filter数目，需要对应修改BassTrebAjust()中对应序号；
//EQ模式功能配置说明:
//    1.此功能是基于MUSIC EQ进行手动设置的，需要在调音参数中使能此EQ；
//    2.可在flat/classic/pop/jazz/pop/vocal boost之间通过KEY来切换   
//    3.如果需要切换EQ_DRC音效，请打开 MUSIC_EQ_DRC
#define CFG_FUNC_AUDIO_EFFECT_EN //总音效使能开关
#ifdef CFG_FUNC_AUDIO_EFFECT_EN

    //#define CFG_FUNC_ECHO_DENOISE					//消除快速调节delay时的杂音
	//#define CFG_FUNC_MUSIC_EQ_MODE_EN				//Music EQ模式功能配置
	#ifdef CFG_FUNC_MUSIC_EQ_MODE_EN
	//	#define MUSIC_EQ_DRC  						//打开此宏EQ将调节EQ_DRC音效, 注意音效中要打开EQ_DRC音效
		#define CFG_FUNC_EQMODE_FADIN_FADOUT_EN		//EQ模式切换时fade in/fade out处理功能选择,调节EQ模式中有POP声时，建议打开 		
    #endif

	#define CFG_FUNC_MUSIC_TREB_BASS_EN				//Music高低音调节功能配置 

    //#define CFG_FUNC_SILENCE_AUTO_POWER_OFF_EN	//无信号自动关机功能，
    #ifdef CFG_FUNC_SILENCE_AUTO_POWER_OFF_EN      
		#define  SILENCE_THRESHOLD                 120           //设置信号检测门限，小于这个值认为无信号
		#define  SILENCE_POWER_OFF_DELAY_TIME      10*60*100     //无信号关机延时时间，单位：ms
    #endif

	#define CFG_FUNC_AUDIO_EFFECT_ONLINE_TUNING_EN//在线调音
	#ifdef CFG_FUNC_AUDIO_EFFECT_ONLINE_TUNING_EN

		//**音频SDK版本号,不要修改**/
		#define  CFG_EFFECT_MAJOR_VERSION						(CFG_SDK_MAJOR_VERSION)
		#define  CFG_EFFECT_MINOR_VERSION						(CFG_SDK_MINOR_VERSION)
		#define  CFG_EFFECT_USER_VERSION						(CFG_SDK_PATCH_VERSION)
	
		/**在线调音硬件接口有USB HID接口，或者UART接口*/
		/**建议使用USB HID接口，收发buf共512Byte*/
		/**UART接口占用2路DMA，收发Buf共2k Byte*/
		/**使用USB HID作为调音接口，DMA资源紧张*/
		#define  CFG_COMMUNICATION_BY_USB		// usb or uart

//		#define	 CFG_UART_COMMUNICATION_TX_PIN					GPIOA10
//		#define  CFG_UART_COMMUNICATION_TX_PIN_MUX_SEL			(3)
//		#define  CFG_UART_COMMUNICATION_RX_PIN					GPIOA9
//		#define  CFG_UART_COMMUNICATION_RX_PIN_MUX_SEL			(1)

		#define  CFG_COMMUNICATION_CRYPTO						(0)////调音通讯加密=1 调音通讯不加密=0
		#define  CFG_COMMUNICATION_PASSWORD                     0x11223344//////四字节的长度密码
	#endif

	//使用flash存好的调音参数，这个需要升级完整的MVA包，不能只下bin文件
	//音效参数存储于flash固定区域中
	//#define CFG_EFFECT_PARAM_IN_FLASH_EN
	#ifdef CFG_EFFECT_PARAM_IN_FLASH_EN
		#define CFG_EFFECT_PARAM_IN_FLASH_MAX_NUM				(10)//支持flash参数最大组数，flash空间紧张时建议按需配置
		#define	CFG_EFFECT_PARAM_IN_FLASH_ACTIVCE_NUM			(6)
		#ifdef CFG_FUNC_AUDIO_EFFECT_ONLINE_TUNING_EN
			#define CFG_EFFECT_PARAM_UPDATA_BY_ACPWORKBENCH			//打开该宏可以通过ACPWorkBench，多消耗4156 Byte RAM，量产时如果RAM紧张可以关闭这个宏
		#endif
	#endif

#endif

//****************************************************************************************
//     音频相关配置参数
//****************************************************************************************
#define	CFG_PARA_SAMPLE_RATE				(44100)//(48000)//
#define CFG_BTHF_PARA_SAMPLE_RATE			(16000)//蓝牙HFP模式下统一采样率为16K
#define	CFG_PARA_SAMPLES_PER_FRAME          (256)//(512)	//系统默认的帧数大小 //注:关闭总音效开关时,必须配置为256,和通话sample配置参数保持一致
#define	CFG_BTHF_PARA_SAMPLES_PER_FRAME     (256)			//蓝牙通话模式下帧数大小
#define CFG_PARA_MIN_SAMPLES_PER_FRAME		(256)//         //系统帧最小值，保证mic delay最小,注意:改为128时 tws带音效播U盘mips余量不足。
#define CFG_PARA_MAX_SAMPLES_PER_FRAME		(512)//(512)

#if (BT_AVRCP_VOLUME_SYNC == ENABLE) && defined(CFG_APP_BT_MODE_EN)
#define CFG_PARA_MAX_VOLUME_NUM		        (16)	//SDK 16 级音量,针对iphone手机蓝牙音量同步功能定制，音量表16级能一一对应手机端音量级别
#define CFG_PARA_SYS_VOLUME_DEFAULT			(12)	//SDK默认音量
#else
#define CFG_PARA_MAX_VOLUME_NUM		        (32)	//SDK 32 级音量
#define CFG_PARA_SYS_VOLUME_DEFAULT			(25)	//SDK默认音量
#endif

//****************************************************************************************
//     转采样功能选择
//说明:
//    1.使能该宏表示系统会将会以统一的采样率输出，默认使用44.1KHz;
//    2.此版本默认打开，请勿关闭!!!!!!!!!!
//****************************************************************************************	
#define	CFG_FUNC_MIXER_SRC_EN

//     采样率硬件微调功能选择
//说明:
//	     硬件微调同一时刻只可使能开启一个微调。使系统AUPLL时钟跟随音源
//****************************************************************************************	
//#define	CFG_FUNC_FREQ_ADJUST
#ifdef CFG_FUNC_FREQ_ADJUST
	#define CFG_PARA_BT_FREQ_ADJUST		//Btplay 模式续存期间 硬件微调，与CFG_PARA_BT_SYNC宏配合
 	#define CFG_PARA_HFP_FREQ_ADJUST	//通话模式 续存期间 硬件微调  使用上行微调，下行跟随。 与CFG_PARA_HFP_SYNC配合
#endif

//****************************************************************************************
//                 录音功能配置
//****************************************************************************************
//#define CFG_FUNC_RECORDER_EN
#ifdef CFG_FUNC_RECORDER_EN

	#define CFG_FUNC_RECORD_SD_UDISK			//录音到SD卡或者U盘
	//#define CFG_FUNC_RECORD_FLASHFS 			//录音到flash  暂不支持
	
	//以下只能三选一，一般用于双芯片录另外一颗芯片的MIC声音
	//#define CFG_FUNC_MIC_RECOD_EN             //通过MIC IN源录音
	//#define CFG_FUNC_I2S0_MIX_RECOD_EN     	//通过I2s0 MIX IN源录音，需要使能 CFG_RES_AUDIO_I2S0IN_EN
	//#define CFG_FUNC_I2S1_MIX_RECOD_EN     	//通过I2s1 MIX IN源录音，需要使能 CFG_RES_AUDIO_I2S1IN_EN

	#if defined(CFG_FUNC_MIC_RECOD_EN) || defined(CFG_FUNC_I2S0_MIX_RECOD_EN) || defined(CFG_FUNC_I2S1_MIX_RECOD_EN)
	#define CFG_FUNC_RECORDER_SILENCE_DECTOR  	//能量检测
	#endif
	
	#ifdef CFG_FUNC_RECORD_SD_UDISK
		#define CFG_FUNC_RECORD_UDISK_FIRST				//U盘和卡同时存在时，录音设备优先选择U盘，否则优先选择录音到SD卡。
		#define CFG_PARA_RECORDS_FOLDER 		"REC"	//录卡录U盘时根目录文件夹。注意ffpresearch_init 使用回调函数过滤字符串。
		#define CFG_FUNC_RECORDS_MIN_TIME		1000	//单位ms，开启此宏后，小于这个长度的自动删除。
		#define CFG_PARA_REC_MAX_FILE_NUM       256     //录音文件最大数目

		#define MEDIAPLAYER_SUPPORT_REC_FILE            // U盘或TF卡模式下，打开此功能，则支持播放录音文件；否则，只能在录音回放模式下播放录音文件
        //#define AUTO_DEL_REC_FILE_FUNCTION            //录音文件达到最大数后，自动删除全部录音文件的功能选项
	#endif

	#define CFG_PARA_REC_GAIN		        (8191)	    //输入录音增益   8191:+6db;7284:+5db;6492:+4db;5786:+3db;5157:+2db;4596:+1db;4095:0db;

	#define DEL_REC_FILE_EN


	/*注意flash空间，避免冲突   middleware/flashfs/file.h FLASH_BASE*/
	#ifdef CFG_FUNC_RECORD_FLASHFS
		#define CFG_PARA_FLASHFS_FILE_NAME		"REC.MP3"//RECORDER_FILE_NAME
	#endif

	//N >= 2 ；考虑128系统帧以及加音效MIPS较高，优先级为3的编码进程处理数据较慢，推荐值为 6。提高系统帧，mips低时可以调小N,节约ram。
	#define MEDIA_RECORDER_FIFO_N				6
	#define MEDIA_RECORDER_FIFO_LEN				(CFG_PARA_MAX_SAMPLES_PER_FRAME * MEDIA_RECORDER_CHANNEL * MEDIA_RECORDER_FIFO_N)
	//调整下列参数后，录音介质可能需要重做兼容性测试 适配FILE_WRITE_FIFO_LEN。
	#define MEDIA_RECORDER_CHANNEL				2
	#define MEDIA_RECORDER_BITRATE				96 //Kbps
	#define MEDIA_RECODER_IO_BLOCK_TIME			1000//ms
	//FIFO_Len=(码率(96) / 8 * 缓冲时间ms(1000) （码率单位Kbps,等效毫秒）
	//根据SDIO协议，写卡阻塞存在250*2ms阻塞 可能，实测部分U盘存在785ms周期性写入阻塞，要求编码数据fifo空间 确保超过这个长度的两倍(含同步)。
	#define FILE_WRITE_FIFO_LEN					((((MEDIA_RECORDER_BITRATE / 8) * MEDIA_RECODER_IO_BLOCK_TIME ) / 512) * 512)//(依据U盘/Card兼容性需求和RAM资源可选400~1500ms。按扇区512对齐
#endif //CFG_FUNC_RECORDER_EN


//****************************************************************************************
//                 U盘或SD卡模式相关功能配置
//    
//****************************************************************************************
#if(defined(CFG_APP_USB_PLAY_MODE_EN) || defined(CFG_APP_CARD_PLAY_MODE_EN) || BT_AVRCP_SONG_TRACK_INFOR)
/**LRC歌词功能 **/
//#define CFG_FUNC_LRC_EN			 	// LRC歌词文件解析

/*------browser function------*/
//#define FUNC_BROWSER_PARALLEL_EN  		//browser Parallel
//#define FUNC_BROWSER_TREE_EN  			//browser tree
#if	defined(FUNC_BROWSER_PARALLEL_EN)||defined(FUNC_BROWSER_TREE_EN)
#define FUNCTION_FILE_SYSTEM_REENTRY
#if defined(FUNC_BROWSER_TREE_EN)||defined(FUNC_BROWSER_PARALLEL_EN)
#define GUI_ROW_CNT_MAX		5		//最多显示多少行
#else
#define GUI_ROW_CNT_MAX		1		//最多显示多少行
#endif
#endif
/*------browser function------*/

/**字符编码类型转换 **/
/**目前支持Unicode     ==> Simplified Chinese (DBCS)**/
/**字符转换库由fatfs提供，故需要包含文件系统**/
/**如果支持转换其他语言，需要修改fatfs配置表**/
/**注意： 支持字库功能需要使用 flash容量>=2M的芯片**/
/**开启此功能，请参考文档《24bit sdk中文字库使用说明.pdf》进行配置的修改 **/
//#define CFG_FUNC_STRING_CONVERT_EN	// 支持字符编码转换

/**取消AA55判断**/
/*fatfs内磁盘系统MBR和DBR扇区结尾有此标记检测，为提高非标类型盘兼容性，可开启此项, 为有效鉴定无效盘，此项默认关闭*/
//#define	CANCEL_COMMON_SIGNATURE_JUDGMENT
//#define FUNC_UPDATE_CONTROL   //升级交互过程控制(通过按键确认升级)
#endif

/**USB Host检测功能**/
#if(defined(CFG_APP_USB_PLAY_MODE_EN))
#define CFG_RES_UDISK_USE
#define CFG_FUNC_UDISK_DETECT
#endif

/**USB Device检测功能**/
#if (defined (CFG_APP_USB_AUDIO_MODE_EN)) || (defined(CFG_COMMUNICATION_BY_USB))
	#define CFG_FUNC_USB_DEVICE_EN
#endif

//flash系统参数在线调整
#define CFG_FUNC_FLASH_PARAM_ONLINE_TUNING_EN

//****************************************************************************************
//                 BT功能配置
//说明:
//    1.蓝牙相关功能配置细节请在bt_config.h中定制!!!!!!!!!!
//    
//****************************************************************************************
#include "bt_config.h"
#if 1//def BT_TWS_SUPPORT
// sync device
#ifdef	CFG_RES_AUDIO_DAC0_EN
	#define TWS_DAC0_OUT	1
#endif
#ifdef CFG_RES_AUDIO_DACX_EN
	#define TWS_DACX_OUT	2
#endif

#if (defined(CFG_RES_AUDIO_I2SOUT_EN) && (CFG_RES_I2S == 0)) || defined(CFG_RES_AUDIO_I2S0OUT_EN)
	#define TWS_IIS0_OUT	3
#endif

#if (defined(CFG_RES_AUDIO_I2SOUT_EN) && (CFG_RES_I2S == 1)) || defined(CFG_RES_AUDIO_I2S1OUT_EN)
	#define TWS_IIS1_OUT	4
#endif

#ifdef BT_TWS_SUPPORT
	#define TWS_SINK_DEV_FIFO_SAMPLES	(CFG_PARA_MAX_SAMPLES_PER_FRAME * 2)
#else
	#define TWS_SINK_DEV_FIFO_SAMPLES	(CFG_PARA_SAMPLES_PER_FRAME * 2)
#endif
//Key device for sync
	#define TWS_AUDIO_OUT_PATH	TWS_DAC0_OUT//TWS_IIS0_OUT//TWS_IIS1_OUT

#else
	#define TWS_AUDIO_OUT_PATH	0xff//释放音频内存
#endif


#define DAC_RESET_SET           0      // 0 使用disable->FuncReset->enable方式；  1 使用pause->run方式


//****************************************************************************************
//****************************************************************************************
/**OS操作系统进入IDLE时经core进入休眠状态，以达到降低功耗目的**/
/*注意，这是OS调度的IDLE，并非应用层APPMODE，应用层无需关心*/
#define CFG_FUNC_IDLE_TASK_LOW_POWER
#ifdef	CFG_FUNC_IDLE_TASK_LOW_POWER
	#define	CFG_GOTO_SLEEP_USE
#endif

//****************************************************************************************
//                 SHELL功能配置
//说明:
//    1.shell功能启用以及配置请到shell.c中配置;
//    2.shell功能默认关闭，默认使用UART1;
//****************************************************************************************
//#define CFG_FUNC_SHELL_EN

//****************************************************************************************
//                 UART DEBUG功能配置
//注意：DEBUG打开后，会增大mic通路的delay，不需要DEBUG调试代码时，建议关闭掉！
//****************************************************************************************
#define CFG_FUNC_DEBUG_EN
#ifdef CFG_FUNC_DEBUG_EN
	//#define CFG_USE_SW_UART
	#ifdef CFG_USE_SW_UART
		#define SW_UART_IO_PORT				    SWUART_GPIO_PORT_A//SWUART_GPIO_PORT_B
		#define SW_UART_IO_PORT_PIN_INDEX	    1//bit num
		#define  CFG_SW_UART_BANDRATE   		512000//software uart baud rate select:38400 57600 115200 256000 512000 1000000 ,default 512000
	#else
		#define CFG_UART_TX_PORT 				(0)//tx port  0--A6，1--A10, 2--A25, 3--A0, 4--A1
		#define  CFG_UART_BANDRATE   			512000//hardware uart baud set
	#endif
#endif

//****************************************************************************************
//                提示音功能配置
//说明:
//    1.烧录工具参见MVs26_SDK\tools\script；
//    2.提示音功能开启，注意flash中const data提示音数据需要预先烧录，否则不会播放;
//    3.const data数据开机检查，影响开机速度，主要用于验证。
//****************************************************************************************
#define CFG_FUNC_REMIND_SOUND_EN
#ifdef CFG_FUNC_REMIND_SOUND_EN
	#define CFG_PARAM_FIXED_REMIND_VOL   	10		//固定提示音音量值,0表示受music vol同步控制
#endif

//****************************************************************************************
//                 断点记忆功能配置        
//****************************************************************************************
#define CFG_FUNC_BREAKPOINT_EN
#ifdef CFG_FUNC_BREAKPOINT_EN
	#define BP_PART_SAVE_TO_NVM			// 断点信息保存到NVM
	#define BP_SAVE_TO_FLASH			// 断电信息保存到Flash
	#define FUNC_MATCH_PLAYER_BP		// 获取FS扫描后与播放模式断点信息匹配的文件。文件夹和ID号
#endif

//****************************************************************************************
//                 Key 按键相关配置
//****************************************************************************************
/**按键beep音功能**/
//#define  CFG_FUNC_BEEP_EN
    #define CFG_PARA_BEEP_DEFAULT_VOLUME    15//注意:若蓝牙音量同步功能开启后，此值最大为16

/**按键双击功能**/
#define  CFG_FUNC_DBCLICK_MSG_EN
#ifdef CFG_FUNC_DBCLICK_MSG_EN
	#define  CFG_PARA_CLICK_MSG             MSG_PLAY_PAUSE //单击执行消息
	#define  CFG_PARA_DBCLICK_MSG           MSG_BT_HF_REDAIL_LAST_NUM   //双击执行消息
	#define  CFG_PARA_DBCLICK_DLY_TIME      35       //双击有效间隔时间:4ms*20=80ms
#endif

/**ADC按键**/
#define CFG_RES_ADC_KEY_SCAN				//在device service 中启用Key扫描ADCKEY
#if defined(CFG_RES_ADC_KEY_SCAN) || defined(CFG_PARA_WAKEUP_SOURCE_ADCKEY)
	#define CFG_RES_ADC_KEY_USE				//ADC按键功能 启用
#endif

/**IR按键**/
#define CFG_RES_IR_KEY_SCAN				//启用device service Key扫描IRKey

/**编码旋钮按键**/
//#define	CFG_RES_CODE_KEY_USE

/**GPIO按键**/
//#define CFG_RES_IO_KEY_SCAN	

/**电位器功能选择**/
//#define CFG_ADC_LEVEL_KEY_EN 

//***************************************************************************************
//					RTC/闹钟功能配置
//注意:
//   1.RTC时钟源选择32K晶振方案，只有BP1064L2型号支持;
//   2.其他芯片型号，RTC时钟源默认选择rc时钟（精度低）；
//   3.其他芯片型号，推荐12M晶振模式，deepsleep功耗有所抬高
//	 4.打开RTC功能
//		a.需要关闭TWS模式,打开CFG_FUNC_RTC_EN宏
//		b.手动删除BT_Audio_APP\middleware\bluetooth文件夹下libtws.a库，并且在工程配置中去掉tws库的引用
//***************************************************************************************
#ifndef BT_TWS_SUPPORT
#ifdef CFG_CHIP_BP1064L2
	#define CFG_FUNC_RTC_EN
#else
	#ifndef CFG_FUNC_BACKUP_EN
		//#define CFG_FUNC_RTC_EN
	#endif
#endif

#ifdef CFG_FUNC_RTC_EN
	#define CFG_RES_RTC_EN
	#ifdef CFG_RES_RTC_EN
		#define CFG_PARA_RTC_SRC_OSC32K   //RTC时钟源参数
		#ifdef CFG_PARA_RTC_SRC_OSC32K
			#define CFG_FUNC_OSC32K_STARTUP_DETECT
		#endif
	#endif
	
	#define CFG_FUNC_ALARM_EN  //闹钟功能,必须开时钟
	#define CFG_FUNC_LUNAR_EN  //万年历,必须开时钟
	#ifdef CFG_FUNC_ALARM_EN
		#define CFG_FUNC_SNOOZE_EN //闹钟贪睡功能
	#endif
#endif
#endif

//****************************************************************************************
//                Display 显示配置
//****************************************************************************************
//#define CFG_FUNC_DISPLAY_TASK_EN	//display task
//#define CFG_FUNC_DISPLAY_EN
#ifdef CFG_FUNC_DISPLAY_EN
//  #define  DISP_DEV_SLED
  #define  DISP_DEV_7_LED
/**8段LED显示操作**/
/*LED显存刷新需要在Timer1ms中断进行，读写flash操作时会关闭中断*/
/*所以需要做特殊处理，请关注该宏包含的代码段*/
/*注意timer中断服务函数和调用到的API必须进入TCM，含调用的所有api，库函数请咨询支持*/
/*开启此宏，要关注所有使用NVIC_SetPriority 设置为0的代码，避免对应中断调用非TCM代码引起死机复位*/
#ifdef DISP_DEV_7_LED
  #define	CFG_FUNC_LED_REFRESH
#endif

#if defined(DISP_DEV_SLED) && defined(DISP_DEV_7_LED) && defined(LED_IO_TOGGLE)
   #error Conflict: display setting error //不能同时选择两种显示模式
#endif
#endif

//****************************************************************************************
//				   iphone检测功能选择设置,用于充电设置
//****************************************************************************************
//#define    CFG_FUNC_DETECT_IPHONE

//****************************************************************************************
//				   耳机插拔检测功能选择设置
//****************************************************************************************
//#define  CFG_FUNC_DETECT_PHONE_EN                            

//****************************************************************************************
//				   3线，4线耳机类型检测功能选择设置
//****************************************************************************************
//#define  CFG_FUNC_DETECT_MIC_SEG_EN  

//****************************************************************************************
//                            配置冲突 编译警告
//****************************************************************************************

#if defined(CFG_FUNC_SHELL_EN) && defined(CFG_USE_SW_UART)
#error	Conflict: shell  X  SW UART No RX!
#endif

// 使能自动化测试
//#define AUTO_TEST_ENABLE

// 使能蓝牙OTA
// OTA打开自动开启蓝牙SPP
//#define  CFG_FUNC_BT_OTA_EN

#include "sys_gpio.h"


//************************************************************************************************************
//dump工具,可以将数据发送到dump工具,用于分析
//************************************************************************************************************

//#define CFG_DUMP_DEBUG_EN
#ifdef CFG_DUMP_DEBUG_EN

//UART配置,需要和串口日志打印为不同的UART组
//UART0: A0/A1/A6
//UART1: A10/A25/A19
	#define CFG_DUMP_UART_TX_PORT 				(4)//tx port  0--A6，1--A10, 2--A25, 3--A0, 4--A1, 5--A19
#if ((CFG_DUMP_UART_TX_PORT == 1) || (CFG_DUMP_UART_TX_PORT == 2) || (CFG_DUMP_UART_TX_PORT == 5))
	#define CFG_DUMP_UART_TX_PORT_GROUP			(1)
#else
	#define CFG_DUMP_UART_TX_PORT_GROUP			(0)
#endif
	#define CFG_DUMP_UART_BANDRATE 				(2000000)

	#define DUMP_UART_MODE_DAC					(0)
	#define DUMP_UART_MODE_LININ				(1)
	#define DUMP_UART_MODE_AEC					(2)
	#define DUMP_UART_MODE_SBC					(3)
	#define DUMP_UART_MODE_SCO					(4)

	#define CFG_DUMP_UART_MODE					(DUMP_UART_MODE_DAC)

	//配置冲突 编译警告
	//DUMP_DEBUG 和 UART DEBUG 不能使用同一组串口
#ifdef CFG_FUNC_DEBUG_EN
	#if(((CFG_UART_TX_PORT == 1) || (CFG_UART_TX_PORT == 2) || (CFG_UART_TX_PORT == 5)) && ((CFG_DUMP_UART_TX_PORT == 1) || (CFG_DUMP_UART_TX_PORT == 2) || (CFG_DUMP_UART_TX_PORT == 5)))
		#error "Different ports are required !!!"
	#endif
	#if(((CFG_UART_TX_PORT == 0) || (CFG_UART_TX_PORT == 3) || (CFG_UART_TX_PORT == 4)) && ((CFG_DUMP_UART_TX_PORT == 0) || (CFG_DUMP_UART_TX_PORT == 3) || (CFG_DUMP_UART_TX_PORT == 4)))
		#error "Different ports are required !!!"
	#endif
#endif

	//a19 是usb接口的dp，使用此io需要关闭所有usb功能
#if(CFG_DUMP_UART_TX_PORT == 5)
	#ifdef CFG_APP_USB_PLAY_MODE_EN
		#error "Need Close CFG_APP_USB_PLAY_MODE_EN !!!"
	#endif
	#ifdef CFG_APP_USB_AUDIO_MODE_EN
		#error "Need Close CFG_APP_USB_AUDIO_MODE_EN !!!"
	#endif
	#ifdef CFG_COMMUNICATION_BY_USB
		#error "Need Close CFG_COMMUNICATION_BY_USB !!!"
	#endif
#endif

#endif /* CFG_DUMP_DEBUG_EN */


#endif /* APP_CONFIG_H_ */
