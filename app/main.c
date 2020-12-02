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
	1.ϵͳ�ϵ��Ӧ�ȳ�ʼ�� DEBUG, ���ڼ�ʱ��ӡ������Ϣ;
**********************************************************************************************************************
*/
int main()
{
	static UINT32 sCurMsCnt = 0;
	
	SysConfigInit();											//	ϵͳ��ʼ��

	LedGpioInit(LED0_GPIO_PIN | LED1_GPIO_PIN);					//	��ʼ�� LED GPIO
	BeepGpioInit();												//	��ʼ�� BEEP GPIO
	//KeyUpExtiInit();											//	KeyUp �ⲿ�ж�����
		
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


