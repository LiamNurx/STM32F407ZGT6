/*
****************************************************************************************************************************
FileName:			app_sysconfig.h
Author:				Liam.Nurx
Date:				2020.11.14

Description:
	
****************************************************************************************************************************
*/

#ifndef			APP_SYSCONFIG_H
#define			APP_SYSCONFIG_H

#include "stm32f4xx_gpio.h"

#include "basetype.h"

/*
****************************************************************************************************************************
KEY0	<==>	PE4	
KEY1	<==>	PE3	
KEY2	<==>	PE2	
WK_UP	<==>	PA0	

LED0	<==>	PF9	
LED1	<==>	PF10

BEEP	<==>	PF8	
****************************************************************************************************************************
*/
//	KEY
#define		KEY_UP_GPIO_PORT			GPIOA
#define		KEYX_GPIO_PORT				GPIOE

#define		KEY_UP_GPIO_PIN				GPIO_Pin_0
#define		KEY0_GPIO_PIN				GPIO_Pin_4
#define		KEY1_GPIO_PIN				GPIO_Pin_3
#define		KEY2_GPIO_PIN				GPIO_Pin_2


//	LED
#define		LED_GPIO_PORT				GPIOF

#define		LED0_GPIO_PIN				GPIO_Pin_9
#define		LED1_GPIO_PIN				GPIO_Pin_10


//	BEEP
#define		BEEP_GPIO_PORT				GPIOF

#define		BEEP_GPIO_PIN				GPIO_Pin_8

//	PRIORITY_GROUP
#define		PRIORITY_GROUP_0			0
#define		PRIORITY_GROUP_1			1

//	PRIORITY_SUB
#define		PRI_GROUP0_SUB0_			0
#define		PRI_GROUP0_SUB1_			1
#define		PRI_GROUP0_SUB2_			2
#define		PRI_GROUP0_SUB3_			3
#define		PRI_GROUP0_SUB4_			4
#define		PRI_GROUP0_SUB5_			5
#define		PRI_GROUP0_SUB6_			6
#define		PRI_GROUP0_SUB7_			7

#define		PRI_GROUP1_SUB0_			0
#define		PRI_GROUP1_SUB1_			1
#define		PRI_GROUP1_SUB2_			2
#define		PRI_GROUP1_SUB3_KEYUP		3
#define		PRI_GROUP1_SUB4_KEY0		4
#define		PRI_GROUP1_SUB5_KEY1		5
#define		PRI_GROUP1_SUB6_KEY2		6
#define		PRI_GROUP1_SUB7_DEBUG		7


INT8 InitSysConfig(void);
void ShowSystemInformation(void);
INT8 InitSystemTime(SYSTEM_TIME *sysTime);
INT8 UpdateSystemTime(SYSTEM_TIME *sysTime);
UINT32 GetRtcTick(void);
UINT32 CalcRtcTickDlt(UINT32 preRtcTick);
INT8 InitIWDG(void);
INT8 FeedIWDG(void);
INT8 IsNeedFeedIWDG(void);
INT8 InitKeyUpGpio(void);
INT8 InitKeyxGpio(UINT32 keyGpioPin);
INT8 InitLedGpio(UINT32 ledGpioPin);
INT8 InitBeepGpio(void);
INT8 LedOnOff(GPIO_TypeDef* ledPort, UINT16 ledPin, BitAction ledStatus);
INT8 BeepOnOff(GPIO_TypeDef* beepPort, UINT16 beepPin, BitAction beepStatus);

#endif			//	End of "APP_SYSCONFIG_H"

























