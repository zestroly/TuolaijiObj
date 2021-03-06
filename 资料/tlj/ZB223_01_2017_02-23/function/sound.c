#define 	SOUND_C
#include	"sound.h"
//声音报警可分为以下几类：
//一、图标类报警
//二、负载开路短路报警
//另外,声音还有左转、右转、双闪类声音。  虚拟输出类报警，比如、门未关报警。
//无模块报警声音、油量、机油压力报警、车速报警、转速报警、气压报警等等   无高CAN报警


//声音报警之图标类配置：仪表表盘上LED及LCD上图标报警。
//有些图标为报警指示图标，用两个位来配置一个图标相应为哪一种报警类型。
//0b00: 无报警
//0b01: 一级长响报警
//0b10: 二级脉冲报警
//0b11: 下客门报警声
//驱动 采用下降沿采集。
/****************************************************************************************************
图标报警：为板上16个LED指示灯 + 板上8个LED指示灯 + 24个LCD指示灯 + 2个首页报警指示灯 = 50个指示灯
用一个字节来表示每个指示灯报警状态。
bit.1_0==11 -> 该报警为声音+文字报警
bit.1_0==01 -> 该报警为声音报警
bit.1_0==10 -> 该报警为文字报警
bit.1_0==00 -> 无任何报警
****************************************************************************************************/

//#pragma CONST_SEG    DEFAULT
//#pragma CODE_SEG  USER_ROM
UINT8 BackDriveLoadsSound[3];
UINT8 Gear2SondCnt;
UINT8 Gear3SondCnt;
UINT8 DashboardLoadsSound[2];
UINT8 OldDashboardLoadsSound;
UINT8 FrontDriveLoadsSound[3];
UINT8 OldFrontDriveLoadsSound[3];
UINT8 BackDriveLoadsSound[3];
UINT8 OldBackDriveLoadsSound[3];
UINT8 MiddleDriveLoadsSound[3];
UINT8 OldMiddleDriveLoadsSound[3];
UINT8 TopDriveLoadsSound[3];
UINT8 OldTopDriveLoadsSound[3];
UINT8 PowerLoadsSound[3];
UINT8 PowerLoadsFuse[3];

UINT8 PowerContorolLoadSound[3];
UINT8 OldPowerLoadsSound[3];
UINT8 J1939FaultSoundWarn;
UINT16 Cat4016DataOld;

UINT8  Gear1Warn[20];
UINT8  OldGear1Warn[20];
UINT8  ComGear1Warn[20];
UINT8  Gear2Warn[20];
UINT8  OldGear2Warn[20];
UINT8  ComGear2Warn[20];
UINT8  Gear3Warn[20];
UINT8  OldGear3Warn[20];
UINT8  ComGear3Warn[20];
UINT8  Gear4Warn[20];
UINT8  OldGear4Warn[20];
UINT8  ComGear4Warn[20]; 
UINT8  SensorFaultSound[6];
UINT8  OldSensorFaultSound[6];
UINT8  ComSensorFaultSound[6];
UINT8  ModuleErrorFlagWarn;
UINT8  OldModuleErrorFlagWarn;
UINT8  ComModuleErrorFlagWarn;

UINT8 IndicatorLed[20];
UINT8 IndicatorOldLed[20];



UINT8 Gear2SondCnt;
UINT8 Gear3SondCnt;
UINT8 Gear4SondCnt;
//UINT8 LcdWarm;
//UINT8 OldLcdWarm;

/***************************************************************************************************
函数名：void getIcoWarn(UINT8 baseFaultCode)
功能：处理图标报警
参数说明：UINT8 baseFaultCode -> 故障码基值
***************************************************************************************************/
void getIcoWarn(void)
{
	UINT8 i;
	UINT8 type;
	UINT8 row;
	UINT8 line;   
	for(i=0;i<19;i++)    IndicatorLed[i] = LoadLogicResult[i+1].Load;// lcd1~8

	for(i=0;i<160;i++)   //88 75 56       
	{
		if(IcoWarnConfig.DashboardIcoWarn[i]==0) continue;//add by L:上位机补0
		row = i / 0x08;
		line = i % 0x08;
		type = IcoWarnConfig.DashboardIcoWarn[i] >>4;    
		if((type&0x08)==0)          //声音报警
		{
			type&=0x07;
			if(IndicatorLed[row] & (0x01 << line))
			{
				switch(type)
				{
					case 0x01: Gear1Warn[row] |= 0x01 << line;break;
					case 0x02: Gear2Warn[row] |= 0x01 << line;break;
					case 0x03: Gear3Warn[row] |= 0x01 << line;break;
					case 0x04: Gear4Warn[row] |= 0x01 << line;break;
					default:break;                        
				}
			}
			else
			{
				switch(type)
				{
					case 0x01: Gear1Warn[row] &= ~(0x01 << line);break;
					case 0x02: Gear2Warn[row] &= ~(0x01 << line);break;
					case 0x03: Gear3Warn[row] &= ~(0x01 << line);break;
					case 0x04: Gear4Warn[row] &= ~(0x01 << line);break;
					default:break;                              
				}                 
			}

		}
		
		type = IcoWarnConfig.DashboardIcoWarn[i]&0x0f;    //图文字报警
		if((type&0x08)==0)          
		{
			LoadWarnCnt++;
			if((IndicatorLed[row] & (0x01 << line)) != (IndicatorOldLed[row] & (0x01 << line)))
			{
				if(IndicatorLed[row] & (0x01 << line))
				{  
					push_dis(LoadWarnCnt);
				}
				else
				{
					pop_dis(LoadWarnCnt);
				}           
			}
		}
	}
	
	for(i=0;i<20;i++)
	{
		IndicatorOldLed[i] = IndicatorLed[i];
	}
}


/************************************************************************************************
函数名：void sound(void)
功能：总汇声音报警变量
1、图标报警声音
2、负载开路短路报警声音
3、丢高CAN报警
4、丢模块报警
5、传感器故障报警
************************************************************************************************/
void sound(void)
{
	loadOpenOrShortSoundWarn(&USR_s_OV.DashboardLoad[0].Data,&DashboardLoadsSound[0],4);       //仪表负载开路短路声音。
	loadOpenOrShortSoundWarn(&USR_s_OV.FrontDriveLoad[0].Data,FrontDriveLoadsSound,20);  //前驱负载开路短路声音。
	loadOpenOrShortSoundWarn(&USR_s_OV.BackDriveLoad[0].Data,BackDriveLoadsSound,20);    //后驱负载开路短路声音。
	//loadOpenOrShortSoundWarn(&USR_s_OV.MiddleDriveLoad[0].Data,MiddleDriveLoadsSound,18);//中驱负载开路短路声音。
	loadOpenOrShortSoundWarn(&USR_s_OV.TopDriveLoad[0].Data,TopDriveLoadsSound,20);      //顶驱负载开路短路声音�

	if(SysConfigData.selectPowerCtr==1) 
	{ 
		loadOpenOrShortSoundWarn(&USR_s_OV.PowerFuse[0].Data,PowerLoadsFuse,20);  
	}
	else
	{
		loadOpenOrShortSoundWarn(&USR_s_OV.PowerLoadState[0].Data,PowerLoadsSound,20);       //配电负载开路短路声音。
		loadOpenOrShortSoundWarn(&USR_s_OV.PowerFuse[0].Data,PowerLoadsFuse,17); 
	}
 
	loadOpenOrShortSoundWarn(&USR_s_OV.PowerContorolLoadState[0].Data,PowerContorolLoadSound,16);                                                                    
	sensorFaultSound();
}  
/**********************************************************************************************
函数名：void loadOpenOrShortSoundWarn(UINT8 *pLoadStatus,UINT8 *pSoundWarn,UINT8 len)
功能：把负载报警状态转化为声音报警。
参数说明：UINT8 *pLoadStatus -> 负载状态
          UINT8 *pSoundWarn -> 负载声音报警指示
          UINT8 len -> 负载状态长度
返回值：UINT8 *pSoundWarn

* note by ap ,2016/3/25
* @brief:		
* @parameter	*pLoadStatus 负载状态，两位表示
*						0x01 is short circuit,0x02 is open circuit
*				pSoundWarn 输出量。一位表示
*				len 负载个数
**********************************************************************************************/
void loadOpenOrShortSoundWarn(UINT8 *pLoadStatus,UINT8 *pSoundWarn,UINT8 len)
{
    UINT8 i;
    UINT8 srcRow,srcLine;
    UINT8 warnRow,warnLine;
    UINT8 temp;
	
    for(i=0;i<len;i++)
    {
        srcRow = i/0x04;
        srcLine = i%0x04;
        warnRow = i/0x08;
        warnLine = i%0x08;
        temp = pLoadStatus[srcRow] >> (srcLine * 0x02);
        temp &= 0x03;
        if(temp == LOAD_OPEN || temp == LOAD_SHORT)
        {
            pSoundWarn[warnRow] |= 0x01 << warnLine;
        }
        else
        {
            pSoundWarn[warnRow] &= ~(0x01 << warnLine);
        }
    }    
}
/************************************************************************************************
函数名：void sensorFaultSound(void)
功能：处理传感器故障声音
************************************************************************************************/
void sensorFaultSound(void)
{
	UINT8 i;
	UINT8 row,line;
	UINT8 type;
	for(i=0;i<6;i++)     //仪表    6
	{
		row = i / 0x08;
		line = i % 0x08;
		type = IcoWarnConfig.DashSensorIcoWarn[i] >>4; 
		if((type&0x08)==0)          //声音报警
		{
			if(InsideData[i] == 0xfffe)
			{
				SensorFaultSound[row] |= (0x01 << line);
			}
			else
			{
				SensorFaultSound[row] &= ~(0x01 << line);
			}  
		}

	}
	for(i=6;i<14;i++)   //前驱
	{
		row = i / 0x08;
		line = i % 0x08;
		type = IcoWarnConfig.FrontSensorIcoWarn[i-6] >>4; 
		if((type&0x08)==0)          //声音报警
		{
			if(InsideData[FRONT_INSIDE_OFFSET+i-6] == 0xfffe)
			{
				SensorFaultSound[row] |= (0x01 << line);
			}
			else
			{
				SensorFaultSound[row] &= ~(0x01 << line);
			} 
		}
	}
	for(i=14;i<22;i++)   //后驱
	{
		row = i / 0x08;
		line = i % 0x08;
		type = IcoWarnConfig.BackSensorIcoWarn[i-14] >>4; 
		if((type&0x08)==0)          //声音报警
		{
			if(InsideData[BACK_INSIDE_OFFSET+i-14] == 0xfffe)
			{
				SensorFaultSound[row] |= (0x01 << line);
			}
			else
			{
				SensorFaultSound[row] &= ~(0x01 << line);
			} 
		}
	}
	for(i=22;i<30;i++)   //中
	{
		row = i / 0x08;
		line = i % 0x08;
		type = IcoWarnConfig.MiddleSensorIcoWarn[i-22] >>4; 
		if((type&0x08)==0)          //声音报警
		{
			if(InsideData[MIDDLE_INSIDE_OFFSET+i-22] == 0xfffe)
			{
				SensorFaultSound[row] |= (0x01 << line);
			}
			else
			{
				SensorFaultSound[row] &= ~(0x01 << line);
			} 
		}
	}

	for(i=30;i<38;i++)   //顶驱
	{
		row = i / 0x08;
		line = i % 0x08;
		type = IcoWarnConfig.TopSensorIcoWarn[i-30] >>4; 
		if((type&0x08)==0)          //声音报警
		{
			if(InsideData[TOP_INSIDE_OFFSET+i-30] == 0xfffe)
			{
				SensorFaultSound[row] |= (0x01 << line);
			}
			else
			{
				SensorFaultSound[row] &= ~(0x01 << line);//38路
			}  
		}
	}
	for(i=38;i<41;i++) 
	{
		row = i / 0x08;
		line = i % 0x08;
		type = IcoWarnConfig.PowerContorolSensorIcoWarn[i-38] >>4; 
		if((type&0x08)==0)          //声音报警
		{
			if(InsideData[POWER_CONTOROL_OFFSET+i-38] == 0xfffe)
			{
				SensorFaultSound[row] |= (0x01 << line);
			}
			else
			{
				SensorFaultSound[row] &= ~(0x01 << line);//38路
			}  
		}
	}
}

/**************************************************************************************************
函数名：void renewSound(void)
功能：恢复声音
**************************************************************************************************/
void renewSound(void)
{
	UINT8 soundF;
	
	soundF = 0;
	FeedWatchDog();
	ModuleErrorFlagWarn  = ModuleErrorFlag.Byte;
	soundF |= sound_byte_compare(Gear1Warn,ComGear1Warn,sizeof(Gear1Warn)); 
	if(!soundF) 
	{
		soundF |= sound_byte_compare(Gear2Warn,ComGear2Warn,sizeof(Gear2Warn));
		if(!soundF) 
		{ 
			soundF |= sound_byte_compare(Gear3Warn,ComGear3Warn,sizeof(Gear3Warn));
			if(!soundF) 
			{  
				soundF |= sound_byte_compare(Gear4Warn,ComGear4Warn,sizeof(Gear4Warn)); 
				if(!soundF) 
				{ 
					soundF |= sound_byte_compare(SensorFaultSound,ComSensorFaultSound,sizeof(SensorFaultSound)); 
					if(!soundF) 
					{  
						soundF |= sound_byte_compare(&ModuleErrorFlagWarn,&ComModuleErrorFlagWarn,1);  
					}
				}
			}
		}
	}

	if(soundF) 
	{
		CopyDataSound(Gear1Warn,ComGear1Warn,sizeof(Gear1Warn)); 
		CopyDataSound(Gear2Warn,ComGear2Warn,sizeof(Gear2Warn)); 
		CopyDataSound(Gear3Warn,ComGear3Warn,sizeof(Gear3Warn)); 
		CopyDataSound(Gear4Warn,ComGear4Warn,sizeof(Gear4Warn)); 
		CopyDataSound(SensorFaultSound,ComSensorFaultSound,sizeof(SensorFaultSound)); 
		CopyDataSound(&ModuleErrorFlagWarn,&ComModuleErrorFlagWarn,1); 
		MUTE_SOUNDS_F = 0;
		GEAR_SOUND_OFF_F = 1;       
	}

	if(MUTE_SOUNDS_KEY) //ap 静音
	{
		CopyDataSound(Gear1Warn,OldGear1Warn,sizeof(Gear1Warn)); 
		CopyDataSound(Gear2Warn,OldGear2Warn,sizeof(Gear2Warn)); 
		CopyDataSound(Gear3Warn,OldGear3Warn,sizeof(Gear3Warn)); 
		CopyDataSound(Gear4Warn,OldGear4Warn,sizeof(Gear4Warn)); 
		CopyDataSound(SensorFaultSound,OldSensorFaultSound,sizeof(SensorFaultSound)); 
		CopyDataSound(&ModuleErrorFlagWarn,&OldModuleErrorFlagWarn,1); 
		MUTE_SOUNDS_KEY = 0;
		MUTE_SOUNDS_F = 1;
	}   
}

/***************************************************************************************************
函数名：sound_byte_compare(UINT8 *newSt UINT8 *oldSt UINT8 len) 
功能：处理图标报警
参数说明：UINT8 baseFaultCode -> 故障码基值
* note by ap,2016/3/25
* @return		1 启动
***************************************************************************************************/
UINT8 sound_byte_compare(UINT8 *newSt,UINT8 *oldSt,UINT8 len) 
{
    UINT8 i,j;
    UINT8 newTemp,oldTemp;
    UINT8 reSt;

	reSt = 0;  
    for(i=0;i<len;i++) 
	{
		if(newSt[i] == oldSt[i])  continue;
		for(j=0;j<8;j++) 
		{
			newTemp = newSt[i]&(1<<j);
			oldTemp = oldSt[i]&(1<<j);
			if(newTemp > oldTemp) //(newTemp==1,oldTemp==0
			{
				reSt = 1;  
			} 
			else if(oldTemp > newTemp) 
			{
				oldSt[i] &= (~(1<<j));
			}
		}  
	}
    return   reSt;
}

void  CopyDataSound(UINT8 *pScr,UINT8 *pDst,UINT8 cnt) 
{
    UINT8 i;
    for(i=0;i<cnt;i++)
    {
        pDst[i] = pScr[i];
    }
}  





/************************************************************************************************
函数名：UINT8 doLoadOpenShortSound(void)
************************************************************************************************/
UINT8 do1GearSound(void)
{
    BOOLEAN soundF=0;
    ModuleErrorFlagWarn  = ModuleErrorFlag.Byte;
    soundF |= sound_byte_compare(Gear1Warn,OldGear1Warn,sizeof(Gear1Warn)); 
    soundF |= sound_byte_compare(SensorFaultSound,OldSensorFaultSound,sizeof(SensorFaultSound)); 
    soundF |= sound_byte_compare(&ModuleErrorFlagWarn,&OldModuleErrorFlagWarn,1); 
    if(ACC_LOAD&&GEAR_SOUND_OFF_F)
    {  
        if(soundF && !MUTE_SOUNDS_F&& !GEAR_SOUND_LOAD)
        {
            SetBuzzer;
        }
        else
        {
            BuzzerOff;
        }
    }
    return (UINT8)(soundF);  
}
/************************************************************************************************************************
函数名：UINT8 do2GearSound(void)
功能：处理二级声音
************************************************************************************************************************/
UINT8 do2GearSound(void)
{
    BOOLEAN soundF=0; 
    soundF |= sound_byte_compare(Gear2Warn,OldGear2Warn,sizeof(Gear2Warn)); 
    if(soundF)
    {
       Gear2SondCnt++;
       if(Gear2SondCnt >= 50)       //500ms
       {
           Gear2SondCnt = 0x00;
       }
    }
    else
    {
        return 0;
    }
    if((soundF != OLD_2_GEAR_SOUND_F))
    {
        Gear2SondCnt = 0x00;
        OLD_2_GEAR_SOUND_F = soundF;
    }
    if(ACC_LOAD&&GEAR_SOUND_OFF_F)
    {  
        if(soundF)
        {
            if((Gear2SondCnt/25 == 0x00)&&!MUTE_SOUNDS_F&&!GEAR_SOUND_LOAD)
            {
              SetBuzzer; 
            }
            else
            {
               BuzzerOff;
            }
        }
        else
        {
              BuzzerOff;
        }
    }
    return (UINT8)soundF;
}
/************************************************************************************************************************
函数名：UINT8 do2GearSound(void)
功能：处理二级声音
************************************************************************************************************************/
UINT8 do3GearSound(void)
{
    BOOLEAN soundF=0;
    soundF |= sound_byte_compare(Gear3Warn,OldGear3Warn,sizeof(Gear3Warn)); 
    if(soundF)
    {
       Gear3SondCnt++;
       if(Gear3SondCnt >= 25)
       {
           Gear3SondCnt = 0x00;
       }
    }
    else
    {
        return 0;
    }
    if((soundF != OLD_3_GEAR_SOUND_F))
    {
        Gear3SondCnt = 0x00;
        OLD_3_GEAR_SOUND_F = soundF;
    }
    if(ACC_LOAD&&GEAR_SOUND_OFF_F)
    {  
        if(soundF)
        {
            if((Gear3SondCnt/12 == 0x00)&&!MUTE_SOUNDS_F)
            {
               SetBuzzer;
            }
            else
            {
               BuzzerOff;
            }
        }
        else
        {
              BuzzerOff;
        }
    }
    return (UINT8)soundF;    
}


/************************************************************************************************************************
函数名：UINT8 do2GearSound(void)
功能：处理二级声音
************************************************************************************************************************/
UINT8 do4GearSound(void)
{
    BOOLEAN soundF=0;
    soundF |= sound_byte_compare(Gear4Warn,OldGear4Warn,sizeof(Gear4Warn)); 
    if(soundF)
    {
       Gear4SondCnt++;
       if(Gear4SondCnt >= 15)     //250ms
       {
           Gear4SondCnt = 0x00;
       }
    }
    else
    {
        return 0;
    }
    if((soundF != OLD_4_GEAR_SOUND_F))
    {
        Gear4SondCnt = 0x00;
        OLD_4_GEAR_SOUND_F = soundF;
    }
    if(ACC_LOAD&&GEAR_SOUND_OFF_F)
    {  
        if(soundF)
        {
            if((Gear4SondCnt/7 == 0x00)&&!MUTE_SOUNDS_F)
            {
               SetBuzzer;
            }
            else
            {
               BuzzerOff;
            }
        }
        else
        {
              BuzzerOff;
        }
    }
    return (UINT8)soundF;    
}

void BuzzerOut(UINT8 mode,UINT8 on_flag)
{                
	switch(mode)
	{
		case 1: 
			if(BUZZER_FLAG) 
			{
				if(!SleepFlag)
					SetBuzzerDb;
				else
					BuzzerOffDb;
                                if(USR_s_DeviceConfigData.BuzzerLevel==1)  LEVEL3_H;
                                else  LEVEL3_L;
                                if(USR_s_DeviceConfigData.BuzzerLevel==2)  LEVEL2_H;
                                else  LEVEL2_L;
                                if(USR_s_DeviceConfigData.BuzzerLevel==3)  LEVEL1_H;
                                else  LEVEL1_L;       
			}
			else 
			{
                                LEVEL1_L;
                                LEVEL2_L;
                                LEVEL3_L;    
				BuzzerOffDb;
	
			}      
			break;
		case 4: 

                                LEVEL1_L;
                                LEVEL2_L;
                                LEVEL3_L;
			        BuzzerOffDb;
			break;

	}; 
}

