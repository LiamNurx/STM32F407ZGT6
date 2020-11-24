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
	1.ϵͳ�ϵ��Ӧ�ȳ�ʼ�� DEBUG, ���ڼ�ʱ��ӡ������Ϣ;
**********************************************************************************************************************
*/
int main()
{
#if DEBUG_EN
	DebugUsart1Init(DEBUG_BOUND_RATE);							//	DEBUG_USART ��ʼ��;
	UsartRxBuffInit(gpDebugRxBuff);								//	DEBUG ���ջ����ʼ��;
#endif

	KeyGpioInit(KEY_UP_GPIO_PORT, KEY_UP_GPIO_PIN);				//	��ʼ�� KEY_UP GPIO
	LedGpioInit(LED_GPIO_PORT, LED0_GPIO_PIN | LED1_GPIO_PIN);	//	��ʼ�� LED GPIO
	BeepGpioInit(BEEP_GPIO_PORT, BEEP_GPIO_PIN);				//	��ʼ�� BEEP GPIO

	KeyUpExtiInit();											//	KeyUp �ⲿ�ж�����
	
	while(1)
	{
		;
	}
	
	//return 0;
}


