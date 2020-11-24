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

#include "sysconfig.h"


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
INT8 KeyGpioInit(GPIO_TypeDef* keyGpioPort, UINT32 keyGpioPin)
{
	GPIO_InitTypeDef keyGpioInitStruct;

	if((KEYX_GPIO_PORT != keyGpioPort) && (KEY_UP_GPIO_PORT != keyGpioPort))
	{
		#if DEBUG_EN
		printf("GPIO_PORT of KEY is error! <===> [fault]\r\n");
		#endif

		return -1;
	}

	if ((KEY_UP_GPIO_PIN != keyGpioPin) \
	 && (KEY0_GPIO_PIN != keyGpioPin) \
	 && (KEY1_GPIO_PIN != keyGpioPin) \
	 && (KEY2_GPIO_PIN != keyGpioPin))
	{
		#if DEBUG_EN
		printf("GPIO_PIN of KEY is error! <===> [fault]\r\n");
		#endif

		return -2;
	}

	if(KEY_UP_GPIO_PORT == keyGpioPort)
	{
		RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	}
	else
	{
		RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	}
	
	keyGpioInitStruct.GPIO_Pin = keyGpioPin;
	keyGpioInitStruct.GPIO_Mode = GPIO_Mode_IN;
	keyGpioInitStruct.GPIO_OType = GPIO_OType_PP;
	keyGpioInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	keyGpioInitStruct.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_Init(keyGpioPort, &keyGpioInitStruct);

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
INT8 LedGpioInit(GPIO_TypeDef* ledGpioPort, UINT32 ledGpioPin)
{
	GPIO_InitTypeDef ledGpioInitStruct;
	
	if(LED_GPIO_PORT != ledGpioPort)
	{
		#if DEBUG_EN
		printf("GPIO_PORT of LED is error! <===> [fault]\r\n");
		#endif

		return -1;
	}

	if ((LED0_GPIO_PIN != ledGpioPin) \
	 && (LED1_GPIO_PIN != ledGpioPin) \
	 && ((LED0_GPIO_PIN | LED1_GPIO_PIN) != ledGpioPin))
	{
		#if DEBUG_EN
		printf("GPIO_PIN of LED is error! <==> [fault]\r\n");
		#endif

		return -2;
	}

	RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOF, ENABLE);

	ledGpioInitStruct.GPIO_Pin = ledGpioPin;
	ledGpioInitStruct.GPIO_Mode = GPIO_Mode_OUT;
	ledGpioInitStruct.GPIO_OType = GPIO_OType_PP;
	ledGpioInitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	ledGpioInitStruct.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_Init(ledGpioPort, &ledGpioInitStruct);
	
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
INT8 BeepGpioInit(GPIO_TypeDef* beepGpioPort, UINT32 beepGpioPin)
{
	GPIO_InitTypeDef beepGpioInitStruct;
	
	if(BEEP_GPIO_PORT != beepGpioPort)
	{
		#if DEBUG_EN
		printf("GPIO_PORT of BEEP is error! <===> [fault]\r\n");
		#endif

		return -1;
	}

	if(BEEP_GPIO_PIN != beepGpioPin)
	{
		#if DEBUG_EN
		printf("GPIO_PIN of BEEP is error! <===> [fault]\r\n");
		#endif 

		return -2;
	}

	RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOF, ENABLE);

	beepGpioInitStruct.GPIO_Pin = beepGpioPin;
	beepGpioInitStruct.GPIO_Mode = GPIO_Mode_OUT;
	beepGpioInitStruct.GPIO_OType = GPIO_OType_PP;
	beepGpioInitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	beepGpioInitStruct.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_Init(beepGpioPort, &beepGpioInitStruct);
	
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










