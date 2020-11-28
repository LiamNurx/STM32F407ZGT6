/*
****************************************************************************************************************************
FileName:			exti.c
Author:				Liam.Nurx
Date:				2020.11.23

�ⲿ�жϵ�ʹ��ʾ��:
	1.���� GPIO_Init() ������Ӧ I/O Ϊ����ģʽ;
	2.���� SYSCFG_EXTILineConfig() ����Ӧ I/O ���õ��ж���;
	3.���� EXTI_Init() �����ж�/�¼�ģʽ�ʹ�����ʽ;
	4.���� NVIC_Init() ���ж���ӳ�䵽��Ӧ�� NVIC �ж�ͨ��;
****************************************************************************************************************************
*/

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_syscfg.h"

#include "exti.h"
#include "sysconfig.h"

/*
****************************************************************************************************************************
Note:
	1.KeyUp		<==>	PA0					//	�ߵ�ƽ������Ч
	2.ʹ�� SYSCFG ����ʱ��;					//	��Ҫ���üĴ���: SYSCFG_EXTICRx
	3.��ʼ��������Ӧ�� GPIO;
	4.����Ӧ�� I/O ���õ��ж�����;
****************************************************************************************************************************
*/
void KeyUpExtiInit(void)
{
	EXTI_InitTypeDef keyUpExtiInitStruct;
	NVIC_InitTypeDef keyUpNvicInitStruct;

	keyUpExtiInitStruct.EXTI_Line = EXTI_Line0;
	keyUpExtiInitStruct.EXTI_LineCmd = ENABLE;
	keyUpExtiInitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	keyUpExtiInitStruct.EXTI_Trigger = EXTI_Trigger_Rising;

	keyUpNvicInitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	keyUpNvicInitStruct.NVIC_IRQChannelCmd = ENABLE;
	keyUpNvicInitStruct.NVIC_IRQChannelSubPriority = PRIORITY_GROUP_1;
	keyUpNvicInitStruct.NVIC_IRQChannelPreemptionPriority = PRI_GROUP1_SUB3_KEYUP;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	KeyUpGpioInit();
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	EXTI_Init(&keyUpExtiInitStruct);
	NVIC_Init(&keyUpNvicInitStruct);
}

void Key0ExtiInit(void)
{
	
}

void Key1ExtiInit(void)
{

}

void Key2ExtiInit(void)
{

}



































