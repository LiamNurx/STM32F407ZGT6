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

#include "sysconfig.h"
#include "usart.h"
#include "exti.h"

extern CIRCULAR_QUEUE gDebugRxBuff;
extern CIRCULAR_QUEUE *gpDebugRxBuff;

extern unsigned int g1MsStkCounter;

/*
**********************************************************************************************************************
Note:
	1.系统上电后应先初始化 DEBUG, 便于及时打印调试信息;
**********************************************************************************************************************
*/
int main()
{
	static UINT32 sCurMsCnt = 0;
	
	SysConfigInit();											//	系统初始化

	LedGpioInit(LED0_GPIO_PIN | LED1_GPIO_PIN);					//	初始化 LED GPIO
	BeepGpioInit();												//	初始化 BEEP GPIO
	//KeyUpExtiInit();											//	KeyUp 外部中断配置
		
	LedOnOff(LED_GPIO_PORT, LED0_GPIO_PIN, Bit_RESET);
	LedOnOff(LED_GPIO_PORT, LED1_GPIO_PIN, Bit_RESET);
	//BeepOnOff(BEEP_GPIO_PORT, BEEP_GPIO_PIN, Bit_SET);
	
	while(1)
	{
		//UsartRxBuffTest();

		if(g1MsStkCounter - sCurMsCnt >= 5000)
		{
			sCurMsCnt = g1MsStkCounter;
			printf("Hello, Welcome to the world of STM32!\r\n");
		}
	}
	
	//return 0;
}


