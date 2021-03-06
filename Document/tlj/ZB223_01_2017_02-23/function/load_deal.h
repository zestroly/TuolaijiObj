#ifndef	__LOAD_DEAL_H_
#define __LOAD_DEAL_H_
#include "ap_S32K1xx_adc.h"
#include "typedef.h"
#include "common.h"
#include "configFile.h"
#include "LoadOutputCondition.h"
#include "LcmDisp.h"
#include "Led.h"
#include "configFile.h"
#include "LED.h"


/*****************PARAMETER MACROS*****************/
#define	NUM_CAT4016							1
#define	Num_PowerDevice					1
#define NUM_BUZZER_CHANNELS     16 

#define DASHBOARD_LOAD_START_ADDR       0
#define LCD_INDICATOR_START_ADDR        4
#define LED_INDICATOR_START_ADDR        28
#define BOARD_LED_START_ADDR            44
#define VIRTUAL_LOAD_START_ADDR         52

#define DASHBOARD_LOAD_NUM              2
#define LCD_INDICATOR_NUM               24
#define LED_INDICATOR_NUM               16
#define BOARD_LED_NUM                   8
#define VIRTUAL_LOAD_NUM                40


#define DETECT_LOAD_SHORT              ((stTemp == 0x00) && (inputTemp == 0x01))
#define DETECT_LOAD_OPEN               ((stTemp == 0x00) && (inputTemp == 0x00))
#define DETECT_LOAD_CLOSE              ((stTemp == 0x01) && (inputTemp == 0x00))
#define DETECT_LOAD_NORMAL             ((stTemp == 0x01) && (inputTemp == 0x01))
#define UNMARK_SHORT_DETECT     0x01
#define UNMARK_OPEN_DETECT      0x01
#define LOAD_RUNING             0x03
#define LOAD_SHORT              0x01
#define LOAD_OPEN               0x02
#define LOAD_CLOSE              0x00

#define SENSOR_ERROR            0x01

#define RUNING_CNT              5
#define SHORT_CNT               5
#define OPEN_CNT                5
#define CLOSE_CNT               5
#define	BTS724_ST_STATUS		    155	///3V 

#define LEVEL_1_WARN            0x01
#define LEVEL_2_WARN            0x02
#define LEVEL_3_WARN            0x03

#define LOAD_ON_OPEN_DETECT     0x00
#define LOAD_NORMAL_DETECT      0x01

/******funtion macros****************************/
//#pragma CONST_SEG __GPAGE_SEG CONFIG_DATA_SDO
//EXT const  UINT8 ShortMask;
//EXT const  UINT8 OpenMask;
//#pragma CODE_SEG  USER_ROM

EXT void driverDashboardLoad(PutLoadStruct data);
EXT void initCAT4016(void);
EXT void driverCat4016Led(void);
EXT void initDirectLedPin(void);
//EXT void driverDirectLed(void);
EXT void initBuzzerPin(void);
EXT void driverBuzzer(void);


EXT void 	SendMeterLedProcess(void);
EXT void	GetSTProcess(void);


EXT void togetherNewStatusAndAbnormalStatus(void);
EXT void detectLoadsStatus(UINT8 *pResult,UINT8 *pAbnormal,UINT8 *pSt,UINT8 *pInput1,UINT8 * pShortMask,UINT8 * pOpenMask,UINT8 len,UINT8 style);
EXT void confirmLoadStatus(UINT8 *pResult,UINT8 *pLoadStatus,UINT8 *pCnt,UINT8 len);
EXT void clearNewStatusCnt(UINT8 *pNewStatus,UINT8 *pOldStatus,UINT8 *pCnt,UINT8 len);
EXT void loadAbnormalDetect(UINT8 *pAbnormal,UINT8 *pLoadStatus,UINT8 amount);

EXT void updataLoadsStatus(UINT8 *pLoadsStatus,UINT8 *pNewStatus,UINT8 *pChangeLocation);
EXT void shutOffErrorLoads(UINT8 *pLoad,UINT8 *pLoadsStatus,UINT8 len);
EXT void detectOpen(void);
EXT void loadOnOpenDetect(UINT8 *pOffLoadStatus,UINT8 *pNormalStatus,UINT8 cnt);



EXT void gearStatus(void);
void startStatus(void);


//#pragma CODE_SEG DEFAULT
//================ram macros=======================
EXT PutLoadStruct LedDriverBuf;          //板上非CAT4016LED。
EXT PutLoadStruct VirtualLoad[5];


#define   ACC_LOAD                                      VirtualLoad[0].Loads.Load2    //66处理关ACC时琇CD屏的切换
#define   ON_LOAD                                       VirtualLoad[0].Loads.Load3    //67 处理ON时开关LCD屏
#define   FRONT_DOOR_OPENED_LOAD        VirtualLoad[0].Loads.Load4    //68处理前门未关时，LCD屏的切换
#define   MIDDLE_DOOR_OPENED_LOAD       VirtualLoad[0].Loads.Load5    //69处理中门未关时Ｇ宄涸毓收�
#define   SYS_SLEEP_LOAD                           VirtualLoad[0].Loads.Load6    //70休眠状态
#define   MIDDLE_VIDEO_LOAD                     VirtualLoad[0].Loads.Load7    //71处理开中门时，LCD切换到中门视频
#define   R_GEAR_VIDEO_LOAD                    VirtualLoad[1].Loads.Load0    //72处理倒档开时，LCD切换到倒档视频
#define   INTERFACE_SWITCH_LOAD            VirtualLoad[1].Loads.Load1    //73画面切换
//#define   FAST_BLINK_LOAD               VirtualLoad[1].Loads.Load2    //74处理逻辑中的模块丢失时，转向灯的快速闪烁
#define   START_TOO_LONGER_LOAD         VirtualLoad[1].Loads.Load3    //75处理启动状态
#define   N_GEAR_UNOPEN_LOAD               VirtualLoad[1].Loads.Load4    //76处理启动状态
#define   RESTART_LOAD                            VirtualLoad[1].Loads.Load5    //77处理启动状态
#define   STARTING_LOAD                          VirtualLoad[2].Loads.Load0    //77处理启动状态
#define   GEAR_1_SOUND_LOAD                 VirtualLoad[2].Loads.Load1    //78当需要声音报警时，而在图标指示、负载开短路、模块丢失无法实现时，用此即可实现
#define   GEAR_2_SOUND_LOAD                VirtualLoad[2].Loads.Load2    //79当需要声音报警时，而在图标指示、负载开短路、模块丢失无法实现时，用此即可实现
//#define   CHARGING_INDICATOR_GO_OUT_5S  VirtualLoad[2].Loads.Load3    //80作为虚拟开关输入。
#define   PRES1_WARN                              VirtualLoad[2].Loads.Load3    //83 前气压报警
#define   PRES2_WARN                              VirtualLoad[2].Loads.Load4    //84 后气压报警


//#define   WARM_LOAD                     VirtualLoad[2].Loads.Load4    //81处理系统休眠

#define   ACC_KEY                       USR_s_OV.DashboardKey[0].b.Key6
#define   ON_KEY                        USR_s_OV.DashboardKey[1].b.Key1

#define   PWMLOADST0            LogicOutPut[0].Loads.Load2
#define   PWMLOADST1            LogicOutPut[0].Loads.Load3	//ap: 0位短路
                                     
//EXT UINT8 ConfigVirtualLoad[5];
//EXT UINT8 Fault1Load[2];
//EXT UINT8 Fault2Load[2];
//EXT UINT8 Fault3Load[2];
//EXT unsigned int BuzzerData;
//EXT unsigned int BuzzerCondition;
EXT UINT8 StartStatus;
EXT PutLoadStruct LogicOutPut[20];
EXT PutLoadStruct AllLogicOutPut[20];
EXT PutLoadStruct LoadLogicResult[20];
EXT UINT8 PwmLoadShortCnt[2];
//EXT PutLoadStruct LeftRightLoadLogic[2];
//EXT	PutLoadStruct PutData[Num_PowerDevice];
//EXT PutLoadStruct OldPutDtata[Num_PowerDevice];
EXT	LoadStruct Cat4016Data;
EXT	LoadStruct Cat4016OldData;
EXT UINT8  DetectStyleCnt;

EXT DIAGNOSE_STATUS_STRUCT DashboardDiagnose[18];
//EXT DIAGNOSE_STATUS_STRUCT OldDashboardDiagnose[18];
EXT DIAGNOSE_STATUS_STRUCT FrontDriverDiagnose[13];
//EXT DIAGNOSE_STATUS_STRUCT OldFrontDriverDiagnose[13];
EXT DIAGNOSE_STATUS_STRUCT BackDriverDiagnose[13];
//EXT DIAGNOSE_STATUS_STRUCT OldBackDriverDiagnose[13];
EXT DIAGNOSE_STATUS_STRUCT MiddleDriverDiagnose[13];
//EXT DIAGNOSE_STATUS_STRUCT OldMiddleDriverDiagnose[13];
EXT DIAGNOSE_STATUS_STRUCT TopDriverDiagnose[13];
//EXT DIAGNOSE_STATUS_STRUCT OldTopDriverDiagnose[13];
EXT DIAGNOSE_STATUS_STRUCT PowerDiagnose[13];
//EXT DIAGNOSE_STATUS_STRUCT OldPowerDiagnose[13];
EXT DIAGNOSE_STATUS_STRUCT PowerContorolDiagnose[6];


EXT UINT8 LoadsAbnormal[1];
EXT UINT8 NewLoadsStatus0[1];
EXT UINT8 NewLoadsStatus[1];
EXT UINT8 OldLoadsStatus[1];
EXT UINT8 DashboardLoadsStatus[1];
EXT UINT8 LoadsCnt[8];
EXT UINT8 LoadsSt[1];
EXT UINT8 LoadsInput[1];

EXT LoadStStruct    PwmboardLoad;
extern PutLoadStruct LeftRightLoadLogic[2];



//extern void setVolumeBuzzer(unsigned char val);
extern const  uchar BuzzerDutyCONST[4]; 

#define	SetBuzzerDb                             SOUND_POW_DUTY = FTM_PWM_Duty(SOUND_FRQ,50)  // setVolumeBuzzer(USR_s_DeviceConfigData.BuzzerLevel) //  SOUND_POW_DUTY = FTM_PWM_Duty(SOUND_FRQ,BuzzerDutyCONST[USR_s_DeviceConfigData.BuzzerLevel])//PWMDTY1=BuzzerDutyCONST[USR_s_DeviceConfigData.BuzzerLevel]
#define	BuzzerOffDb                             SOUND_POW_DUTY = FTM_PWM_Duty(SOUND_FRQ,0)// setVolumeBuzzer(0)  // SOUND_POW_DUTY = FTM_PWM_Duty(SOUND_FRQ,BuzzerDutyCONST[0])//PWMDTY1=BuzzerDutyCONST[0]
#define	SetBuzzer                               BUZZER_FLAG = 1  
#define	BuzzerOff                               BUZZER_FLAG = 0  

void doLCDBKLight(void);
extern const  uchar TftDutyCONST[4];
#define	BOTTOM_TFT_LIGHT_LED_ON				 doLCDBKLight()//LCDBACKLIGHT_DUTY =FTM_PWM_Duty(TFT_LINGHT_FRQ,TftDutyCONST[USR_s_DeviceConfigData.LightLevel]+InsideData[10])	//PWMDTY0=  TftDutyCONST[USR_s_DeviceConfigData.LightLevel]//TftDutyCONST[2] ///PTP_PTP6
#define	BOTTOM_TFT_LIGHT_LED_OFF				 LCDBACKLIGHT_DUTY = FTM_PWM_Duty(TFT_LINGHT_FRQ,TftDutyCONST[0])// 	PWMDTY0=  TftDutyCONST[0]//TftDutyCONST[0] ///PTP_PTP6

//l:仪表背光灯
extern  const  uchar LedDutyCONST[4]; 
#define	BOTTOM_LIGHT_LED_ON				 	BACKGROUD_LED_DUTY = FTM_PWM_Duty(TFT_LINGHT_FRQ,LedDutyCONST[USR_s_DeviceConfigData.LightLevel])//PWMDTY7=LedDutyCONST[USR_s_DeviceConfigData.LightLevel] ///PTP_PTP6
#define	BOTTOM_LIGHT_LED_OFF			 		BACKGROUD_LED_DUTY = FTM_PWM_Duty(TFT_LINGHT_FRQ,LedDutyCONST[0])//PWMDTY7=LedDutyCONST[0] ///PTP_PTP6



EXT	UINT8  Bts724St[6];


	

///move to App_CANOpenData.h echosong 20110421 EXT	LoadStruct SendMeterLedArry[NUM_CAT4016];
EXT	unsigned char	WaterTempFlahTime;
///M show meter module;led1->dl201,led15->dl215         
/***********************************************************************************************
                         ___                  ____
       --|▔|   /|▔|   |   |                |   |      -{-|▔|    |▔|\
       --|__|   /|__|   | ! |      O         |!!!|      -{-|__|    |  |\}
       --|__|   /|__|   |___|                |___|      -{-|__|    |__|\}
        MLed9   MLed11  MLed13    MLed15     MLed16      MLed14    MLed12
       
        远光灯 近关灯   轻微故障 ACC档开关    严重故障     前雾灯   后雾灯
   ___                                                                         ___
  /   |                                                                       |   \
  \___|                                                                       |___/
  MLed7                                                                       MLed10
  
  左转向灯                                                                   右转向灯
                           ___     ___               _                 ___
          |_   (  )       |   |   |   |      / |     |      (    )    |___| |  
          |    (P )   ~   | 1 |   | 2 |     |/ |   |--- )   (abs )    |   |_|
         ---   (  )       |___|   |___|      |_|   |___ )   (    )    |___| 
        MLed5  MLed3 MLed1                 MLed2    MLed4    MLed6    MLed8
        
        
        水温   驻车  预热  前门开 后门开  后仓门   MIL报警  ASR故障  油量报警灯
                       
************************************************************************************************/


#define	MLed1	    	Cat4016Data.bits.bit7 //39     发动机预热
#define	MLed3	    	Cat4016Data.bits.bit6 //38     驻车符号片
#define	MLed5	    	Cat4016Data.bits.bit5 //37     水温报警
#define	MLed7	    	Cat4016Data.bits.bit4 //36     左转向灯   4
#define	MLed9	    	Cat4016Data.bits.bit3 //35     远光灯
#define	MLed11	    Cat4016Data.bits.bit2 //34     近关灯
#define	MLed13	    Cat4016Data.bits.bit1 //33     轻微故障
#define	MLed15	    Cat4016Data.bits.bit0 //32     ACC档开关

#define	MLed16	    Cat4016Data.bits.bit15//47      严重故障 
#define	MLed14	    Cat4016Data.bits.bit14//46      前雾灯 
#define	MLed12	    Cat4016Data.bits.bit13//45      后雾灯
#define	MLed10	    Cat4016Data.bits.bit12//44      右转向灯     5
#define	MLed8	    	Cat4016Data.bits.bit11//43      油量报警灯
#define	MLed6	    	Cat4016Data.bits.bit10//42      ASR故障
#define	MLed4	    	Cat4016Data.bits.bit9 //41      MIL报警 
#define	MLed2	    	Cat4016Data.bits.bit8 //40      后仓门 
///MLoade show meter load
#define		MLoad1			PutData[0].Loads.Load0             
#define		MLoad2			PutData[0].Loads.Load1
#define		MLoad3			PutData[0].Loads.Load2
#define		MLoad4			PutData[0].Loads.Load3
#define		MLoad5			PutData[0].Loads.Load4
#define		MLoad6			PutData[0].Loads.Load5
#define		MLoad7			PutData[0].Loads.Load6
#define		MLoad8			PutData[0].Loads.Load7
/**************led config***************/
//bit1柱子报警2  前门 + 中门 +车速报警灯 + 转速报警灯
#define LEFTLogicLed     LeftRightLoadLogic[0].Loads.Load4
#define RIGHTLogicLed    LeftRightLoadLogic[1].Loads.Load4


#define   LCD_LEFT_WARM_LOAD            LogicOutPut[6].Loads.Load0    //柱子1
#define   LCD_RIGHT_WARM_LOAD           LogicOutPut[6].Loads.Load1    //柱子2
#define   MIDDLE_DOOR_LED_LOAD          LogicOutPut[6].Loads.Load3    //中门开led
#define   SPEED_LED_WARM_LOAD           LogicOutPut[6].Loads.Load4    //车速报警led
#define   ENGSPEED_LED_WARM_LOAD        LogicOutPut[6].Loads.Load5    //转速报警led
#define   BACK_LED_LOAD                 LogicOutPut[6].Loads.Load6    //位置灯开背光
#define   GEAR_SOUND_LOAD               0//LogicOutPut[0].Loads.Load7 


void doLoads(void);
void initDashboardLoadOutputPin(void);
void getDriverLoadImage(void);
void PwmLoadLogic(void);
void LoadsLedPost(void); 
void driverLoads(void);
void doDiagnoseStatus(void);
void PowerContorolStatus(void);
void dashboardStatus(void);
void FrontDriverStatus(void);
void BackDriverStatus(void);
void MiddleDriverStatus(void);
void TopDriverStatus(void);
void PowerStatus(void);
void doAccOff(void);
void doOnOff(void);
void initLoadsParameter(void);


#endif
