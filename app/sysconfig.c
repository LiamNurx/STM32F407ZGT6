/*
****************************************************************************************************************************
FileName:			sysconfig.c
Author:				Liam.Nurx
Date:				2020.11.14

Description:
	1.����Ӳ����Դ��ʼ������;
****************************************************************************************************************************
*/

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

#include "usart.h"
#include "sysconfig.h"

extern CIRCULAR_QUEUE		*gpDebugRxBuff;

/*
****************************************************************************************************************************
Note:
	1.��ʼ�� DEBUG_UART �� GPIO �� UART �������;
	2.��ʼ�� DEBUG_UART �����ݽ��ջ������;
	3.��ʼ��ϵͳ�ж����ȼ�����;				//	���ȼ������ʼ��������ϵͳ��λǰֻӦ��ʼ��һ��;

#	�жϷ��鼰�ж����ȼ�����
	Cortex-M3/M4 ʵ�������ȼ����üĴ���������8λ
	Ȼ�� STM32F407 ֻʵ����16���ɱ�����ȼ�����ֻʹ�������ȼ����üĴ����ĸ���λ(bit7:bit4)
	��ϵͳ�����ж����ȼ�����ռ1��Bit���ж����ȼ�ռ3��Bit:
	-------------------------------------------------
	| Priority	|	Priority						|
	| Group		|	sub								|
	|-----------------------------------------------|
	| 0			|	0 | 1 | 2 | 3 | 4 | 5 | 6 | 7	|
	|-----------------------------------------------|
	| 1			|	0 | 1 | 2 | 3 | 4 | 5 | 6 | 7	|
	-------------------------------------------------
****************************************************************************************************************************
*/
INT8 SysConfigInit()
{
#if DEBUG_EN
	DebugUsart1Init(DEBUG_BOUND_RATE);							//	DEBUG_USART ��ʼ��;
	UsartRxBuffInit(gpDebugRxBuff);								//	DEBUG ���ջ����ʼ��;
#endif

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);				//	����ϵͳ���ȼ�����;
	
	return 0;
}



/*
****************************************************************************************************************************
Note:
	1.ʹ�ܶ�Ӧ GPIO ����ʱ��;
	2.���ö�Ӧ GPIO;
	3.��������ĳ�� Key;

Param:
	keyGpioPort:			����ʼ��������Ӧ�� GPIO �˿�;
	keyGpioPin:				����ʼ��������Ӧ�� GPIO PIN ��;
****************************************************************************************************************************
*/
INT8 KeyUpGpioInit(void)
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

INT8 KeyxGpioInit(UINT32 keyGpioPin)
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
	1.ʹ�ܶ�Ӧ GPIO ����ʱ��;
	2.���ö�Ӧ GPIO;
	3.����ͬʱ�������� LED;

Param:
	ledGpioPort:			����ʼ��������Ӧ�� GPIO �˿�;
	ledGpioPin:				����ʼ��������Ӧ�� GPIO PIN ��;
****************************************************************************************************************************
*/
INT8 LedGpioInit(UINT32 ledGpioPin)
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
	1.ʹ�ܶ�Ӧ GPIO ����ʱ��;
	2.���ö�Ӧ GPIO;

Param:
	beepGpioPort:			����ʼ��������Ӧ�� GPIO �˿�;
	beepGpioPin:			����ʼ��������Ӧ�� GPIO PIN ��;
****************************************************************************************************************************
*/
INT8 BeepGpioInit(void)
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
	1.���� LED ������״̬;

Param:
	ledPort:			LED ��Ӧ�� GPIO �˿�;
	ledPin:				LED ��Ӧ�� PIN ��;
	ledStatus:			ָ�� LED ��״̬;
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
	1.���� LED ������״̬;

Param:
	beepPort:			LED ��Ӧ�� GPIO �˿�;
	beepPin:			LED ��Ӧ�� PIN ��;
	beepStatus:			ָ�� LED ��״̬;
****************************************************************************************************************************
*/
INT8 BeepOnOff(GPIO_TypeDef* beepPort, UINT16 beepPin, BitAction beepStatus)
{
	GPIO_WriteBit(beepPort, beepPin, beepStatus);
	
	return 0;
}










