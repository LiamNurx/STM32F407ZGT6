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

#include "usart.h"
#include "sysconfig.h"

extern CIRCULAR_QUEUE		*gpDebugRxBuff;

/*
****************************************************************************************************************************
Note:
	1.初始化 DEBUG_UART 的 GPIO 及 UART 相关配置;
	2.初始化 DEBUG_UART 的数据接收缓存队列;
	3.初始化系统中断优先级分组;				//	优先级分组初始化函数在系统复位前只应初始化一次;

#	中断分组及中断优先级配置
	Cortex-M3/M4 实现了优先级配置寄存器的所有8位
	然而 STM32F407 只实现了16个可编程优先级，即只使用了优先级配置寄存器的高四位(bit7:bit4)
	本系统配置中断优先级分组占1个Bit，中断优先级占3个Bit:
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
	DebugUsart1Init(DEBUG_BOUND_RATE);							//	DEBUG_USART 初始化;
	UsartRxBuffInit(gpDebugRxBuff);								//	DEBUG 接收缓存初始化;
#endif

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);				//	设置系统优先级分组;
	
	return 0;
}



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
	1.使能对应 GPIO 外设时钟;
	2.配置对应 GPIO;
	3.可以同时配置两个 LED;

Param:
	ledGpioPort:			待初始化按键对应的 GPIO 端口;
	ledGpioPin:				待初始化按键对应的 GPIO PIN 口;
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
	1.使能对应 GPIO 外设时钟;
	2.配置对应 GPIO;

Param:
	beepGpioPort:			待初始化按键对应的 GPIO 端口;
	beepGpioPin:			待初始化按键对应的 GPIO PIN 口;
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










