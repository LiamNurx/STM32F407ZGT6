/*
****************************************************************************************************************************
FileName:			main.c
Author:				Liam.Nurx
Date:				2020.11.14

Description:
	1.
	2.
****************************************************************************************************************************
*/

#include <stdio.h>

#include "sysconfig.h"
#include "usart.h"
#include "exti.h"

extern CIRCULAR_QUEUE gDebugRxBuff;
extern CIRCULAR_QUEUE *gpDebugRxBuff;

/*
**********************************************************************************************************************
Note:
	1.系统上电后应先初始化 DEBUG, 便于及时打印调试信息;
**********************************************************************************************************************
*/
int main()
{
#if DEBUG_EN
	DebugUsart1Init(DEBUG_BOUND_RATE);							//	DEBUG_USART 初始化;
	UsartRxBuffInit(gpDebugRxBuff);								//	DEBUG 接收缓存初始化;
#endif

	KeyGpioInit(KEY_UP_GPIO_PORT, KEY_UP_GPIO_PIN);				//	初始化 KEY_UP GPIO
	LedGpioInit(LED_GPIO_PORT, LED0_GPIO_PIN | LED1_GPIO_PIN);	//	初始化 LED GPIO
	BeepGpioInit(BEEP_GPIO_PORT, BEEP_GPIO_PIN);				//	初始化 BEEP GPIO

	KeyUpExtiInit();											//	KeyUp 外部中断配置
	
	while(1)
	{
		;
	}
	
	//return 0;
}


