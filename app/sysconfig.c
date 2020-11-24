/*
****************************************************************************************************************************
FileName:			sysconfig.c
Author:				Liam.Nurx
Date:				2020.11.14

Description:
	1.板载硬件资源初始化驱动;
****************************************************************************************************************************
*/

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

#include "sysconfig.h"


/*
****************************************************************************************************************************
Note:
	1.使能对应 GPIO 外设时钟;
	2.配置对应 GPIO;
	3.单独配置某个 Key;

Param:
	keyGpioPort:			待初始化按键对应的 GPIO 端口;
	keyGpioPin:				待初始化按键对应的 GPIO PIN 口;
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
	1.使能对应 GPIO 外设时钟;
	2.配置对应 GPIO;
	3.可以同时配置两个 LED;

Param:
	ledGpioPort:			待初始化按键对应的 GPIO 端口;
	ledGpioPin:				待初始化按键对应的 GPIO PIN 口;
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
	1.使能对应 GPIO 外设时钟;
	2.配置对应 GPIO;

Param:
	beepGpioPort:			待初始化按键对应的 GPIO 端口;
	beepGpioPin:			待初始化按键对应的 GPIO PIN 口;
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
	1.控制 LED 的亮灭状态;

Param:
	ledPort:			LED 对应的 GPIO 端口;
	ledPin:				LED 对应的 PIN 口;
	ledStatus:			指定 LED 的状态;
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
	1.控制 LED 的亮灭状态;

Param:
	beepPort:			LED 对应的 GPIO 端口;
	beepPin:			LED 对应的 PIN 口;
	beepStatus:			指定 LED 的状态;
****************************************************************************************************************************
*/
INT8 BeepOnOff(GPIO_TypeDef* beepPort, UINT16 beepPin, BitAction beepStatus)
{
	GPIO_WriteBit(beepPort, beepPin, beepStatus);
	
	return 0;
}










