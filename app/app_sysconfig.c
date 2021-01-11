/*
****************************************************************************************************************************
FileName:			app_sysconfig.c
Author:				Liam.Nurx
Date:				2020.11.14

Description:
	1.本文件主要用于管理板载系统资源的初始化配置;
****************************************************************************************************************************
*/

#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_iwdg.h"

#include "basetype.h"
#include "app_usart.h"
#include "app_timer.h"
#include "app_sysconfig.h"


extern CIRCULAR_QUEUE		*gpDebugRxBuff;
extern volatile unsigned int g1MsStkCounter;

SYSTEM_TIME		gSystemTime;
SYSTEM_TIME		*gpSystemTime = &gSystemTime;


//	Get compilation time through predefined macros "__DATE__" and "__TIME__";
const char *gpVerCreatedTime = __TIME__" , "__DATE__;


/*
****************************************************************************************************************************
InitSysConfig:
	1.初始化中断优先级分组;
	2.DebugUsart/DebugGpio 初始化;
	3.串口接收缓存队列初始化;
	4.系统时间初始化;
	5.独立看门狗初始化;

Note:
//	关于中断优先级分组及中断初始化的相关说明(详参: 《权威指南》、《Programming Manual》/《内核编程手册》);
	1.系统的中断优先级分组应该在上电之初就确定，设备后续运行过程中不再更改;
	2.系统中断优先级分组由系统控制块的寄存器 SCB_AIRCR 的 PRIGROUP 位域控制;
	3.通过对系统中断优先级分组的设置，从而确定中断优先级寄存器 NVIC_IPRx 中的 Group priority bits 和 Subpriority bits;
	4.中断优先级寄存器 NVIC_IPRx 中对应每个中断的优先级位域为 Bits[7-4];
	5.本系统中设置为 "NVIC_PriorityGroup_1"，即:
		1 bits for pre-emption priority
		3 bits for subpriority
		---------------------------------------------------------
		| Group Priority 	|	Sub	Priority 					|
		| (1 Bits)			|	(3 Bits)						|
		|-------------------------------------------------------|
		| 0					|	0 | 1 | 2 | 3 | 4 | 5 | 6 | 7	|
		|-------------------------------------------------------|
		| 1					|	0 | 1 | 2 | 3 | 4 | 5 | 6 | 7	|
		---------------------------------------------------------
	6.设置中断的主要步骤包括:
		-- 设置中断优先级分组;
		-- 设置中断优先级;
		-- 在 NVIC 或外设中使能中断;
****************************************************************************************************************************
*/
INT8 InitSysConfig()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	
	
#if DEBUG_EN
	InitDebugUsart1(DEBUG_BOUND_RATE);	
	ShowSystemInformation();
#endif	

	InitUsartRxBuff(gpDebugRxBuff);	
	InitSysTickTimer();	
	InitIWDG();

	return 0;
}

void ShowSystemInformation()
{
	printf("Ver: %s\r\n", gpVerCreatedTime);
}

INT8 InitSystemTime(SYSTEM_TIME *sysTime)
{
	if(NULL == sysTime)
	{
		#if DEBUG_EN
		printf("Pointer parameter is NULL. SystemTimeInit failed! <===> [fault]\r\n");
		#endif

		return -1;
	}

	memset(sysTime, 0, sizeof(SYSTEM_TIME));

	sysTime->year = 2020;
	sysTime->month = 12;
	sysTime->day = 2;
	sysTime->hour = 23;
	sysTime->minute = 0;
	sysTime->second = 0;

	return 0;
}

INT8 UpdateSystemTime(SYSTEM_TIME *sysTime)
{
	//static UINT32 msCalculate = 0;
	
	if(NULL == sysTime)
	{
		#if DEBUG_EN
		printf("Pointer parameter is NULL. SystemTimeInit failed! <===> [fault]\r\n");
		#endif

		return -1;
	}

	
	
	return 0;
}

UINT32 GetRtcTick(void)
{
	
	return 0;
}


UINT32 CalcRtcTickDlt(UINT32 preRtcTick)
{
	UINT32 curRtcTick;

	curRtcTick = GetRtcTick();

    preRtcTick &= 0x00FFFFFF;
    curRtcTick &= 0x00FFFFFF;
    return (curRtcTick >= preRtcTick)?(curRtcTick - preRtcTick):(0x00FFFFFF-preRtcTick + curRtcTick);
}


/*
****************************************************************************************************************************
InitIWDG:
	1.Enable write access to IWDG_PR and IWDG_RLR registers;
	2.Sets IWDG Prescaler value;
	3.Sets IWDG Reload value;
	4.Reloads IWDG counter with value defined in the reload register;
	5.Enables IWDG;

Note:
	IWDG overflow time calculation formula:
		Time = (PreVal * RelVal)/(LSI);		<==>	Time = ((4 * 2^IWDG_PR) * RelVal)/(LSI);
	Param:
		PreVal:		Prescaler value;					//	PreVal = (4 * (2^IWDG_PR))
		RelVal:		Reload value;						//	Default Value:	0x400	(1024)
		LSI:		LSI Clock frequency;				//	32KHz

	TimeOutMs = 1024 MS
****************************************************************************************************************************
*/
INT8 InitIWDG(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	IWDG_SetReload(0x400);
	IWDG_ReloadCounter();
	IWDG_Enable();

	return 0;
}


/*
****************************************************************************************************************************
FeedIWDG:
	1.当达到喂狗时限，则喂狗;

Note:
	1.Reloads IWDG counter with value defined in the reload register;
****************************************************************************************************************************
*/
INT8 FeedIWDG(void)
{
	if(1 == IsNeedFeedIWDG())
	{
		IWDG_ReloadCounter();
	}
	
	return 0;
}

/*
****************************************************************************************************************************
IsNeedFeedIWDG:
	1.根据预设喂狗时限判断是否需要喂狗;
Param:
	void;
RtnVal:	
	1:	需要喂狗;
	0:	不需喂狗;
Note:
	1.Feed IWDG every 255Ms;
****************************************************************************************************************************
*/
INT8 IsNeedFeedIWDG(void)
{
	static UINT32 lastFeedIwdgMs = 0;

	if(g1MsStkCounter - lastFeedIwdgMs >= 255)
	{
		lastFeedIwdgMs = g1MsStkCounter;
		
		return 1;
	}

	return 0;
}

INT8 InitWWDG(void)
{


	return 0;
}

INT8 FeedWWDG(void)
{
	

	return 0;
}

INT8 IsNeedFeedWWDG(void)
{
	

	return 0;
}


/*
****************************************************************************************************************************
Note:

****************************************************************************************************************************
*/
INT8 InitKeyUpGpio(void)
{
	GPIO_InitTypeDef keyGpioInitStruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	keyGpioInitStruct.GPIO_Pin = KEY_UP_GPIO_PIN;
	keyGpioInitStruct.GPIO_Mode = GPIO_Mode_IN;
	keyGpioInitStruct.GPIO_OType = GPIO_OType_PP;
	keyGpioInitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	keyGpioInitStruct.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_Init(KEY_UP_GPIO_PORT, &keyGpioInitStruct);

	return 0;
}

/*
****************************************************************************************************************************
Note:

****************************************************************************************************************************
*/
INT8 InitKeyxGpio(UINT32 keyGpioPin)
{
	GPIO_InitTypeDef keyGpioInitStruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	keyGpioInitStruct.GPIO_Pin = keyGpioPin;
	keyGpioInitStruct.GPIO_Mode = GPIO_Mode_IN;
	keyGpioInitStruct.GPIO_OType = GPIO_OType_PP;
	keyGpioInitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	keyGpioInitStruct.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_Init(KEYX_GPIO_PORT, &keyGpioInitStruct);

	return 0;
}

/*
****************************************************************************************************************************
Note:

****************************************************************************************************************************
*/
INT8 InitLedGpio(UINT32 ledGpioPin)
{
	GPIO_InitTypeDef ledGpioInitStruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

	ledGpioInitStruct.GPIO_Pin = ledGpioPin;
	ledGpioInitStruct.GPIO_Mode = GPIO_Mode_OUT;
	ledGpioInitStruct.GPIO_OType = GPIO_OType_PP;
	ledGpioInitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	ledGpioInitStruct.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_Init(LED_GPIO_PORT, &ledGpioInitStruct);
	
	return 0;
}


/*
****************************************************************************************************************************
Note:

****************************************************************************************************************************
*/
INT8 InitBeepGpio(void)
{
	GPIO_InitTypeDef beepGpioInitStruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

	beepGpioInitStruct.GPIO_Pin = BEEP_GPIO_PIN;
	beepGpioInitStruct.GPIO_Mode = GPIO_Mode_OUT;
	beepGpioInitStruct.GPIO_OType = GPIO_OType_PP;
	beepGpioInitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	beepGpioInitStruct.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_Init(BEEP_GPIO_PORT, &beepGpioInitStruct);
	
	return 0;
}

/*
****************************************************************************************************************************
Note:

****************************************************************************************************************************
*/
INT8 LedOnOff(GPIO_TypeDef* ledPort, UINT16 ledPin, BitAction ledStatus)
{
	GPIO_WriteBit(ledPort, ledPin, ledStatus);
	
	return 0;	
}


/*
****************************************************************************************************************************
Note:

****************************************************************************************************************************
*/
INT8 BeepOnOff(GPIO_TypeDef* beepPort, UINT16 beepPin, BitAction beepStatus)
{
	GPIO_WriteBit(beepPort, beepPin, beepStatus);
	
	return 0;
}











