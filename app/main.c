/*
****************************************************************************************************************************
FileName:			main.c
Author:				Liam.Nurx
Date:				2020.11.14

Description:
	1.
****************************************************************************************************************************
*/

#include <stdio.h>

#include "app_sysconfig.h"
#include "app_usart.h"
#include "app_exti.h"
#include "app_timer.h"

extern CIRCULAR_QUEUE gDebugRxBuff;
extern CIRCULAR_QUEUE *gpDebugRxBuff;

extern unsigned int g1MsStkCounter;


//	Local function declare;
INT8 TaskSchedule(void);
INT8 DealPerSec(void);
INT8 TaskCheck(void);


/*
**********************************************************************************************************************
Note:

**********************************************************************************************************************
*/
int main()
{
	static UINT32 sCurMsCnt = 0;
	
	InitSysConfig();

	InitLedGpio(LED0_GPIO_PIN | LED1_GPIO_PIN);	
	InitBeepGpio();	
	//InitKeyUpExti();
		
	LedOnOff(LED_GPIO_PORT, LED0_GPIO_PIN, Bit_RESET);
	LedOnOff(LED_GPIO_PORT, LED1_GPIO_PIN, Bit_RESET);
	//BeepOnOff(BEEP_GPIO_PORT, BEEP_GPIO_PIN, Bit_SET);
	
	while(1)
	{
		//UsartRxBuffTest();

		if(g1MsStkCounter - sCurMsCnt >= 10000)
		{
			sCurMsCnt = g1MsStkCounter;
			printf("Hello, Welcome to the world of STM32!\r\n");
		}
	}
	
	//return 0;
}


INT8 TaskSchedule()
{
	FeedIWDG();
	DealPerSec();

	return 0;
}


INT8 DealPerSec()
{
	static UINT32 sSystemRtcRunTick = 0;
	
	if(0 == sSystemRtcRunTick || CalcRtcTickDlt(sSystemRtcRunTick) >= RTC_RATE)
	{
		TaskCheck();
		//UpdateSystemTime();
	}
	
	return 0;
}


INT8 TaskCheck(void)
{
	

	return 0;
}


