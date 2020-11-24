/*
****************************************************************************************************************************
FileName:			sysconfig.h
Author:				Liam.Nurx
Date:				2020.11.14

Description:
	1.对开发板上硬件资源的 GPIO 端口进行重定义;
	
****************************************************************************************************************************
*/

#ifndef			SYSCONFIG_H
#define			SYSCONFIG_H

#include "stm32f4xx_gpio.h"

typedef		unsigned char				UINT8;
typedef		unsigned short				UINT16;
typedef		unsigned int				UINT32;
typedef		signed char					INT8;
typedef		signed short				INT16;
typedef		signed int					INT32;

#define		TRUE						0x01
#define		FALSE						0x00


/*
****************************************************************************************************************************
#	按键 GPIO 分配:
KEY0	<==>	PE4			//	低电平输入有效
KEY1	<==>	PE3			//	低电平输入有效
KEY2	<==>	PE2			//	低电平输入有效
WK_UP	<==>	PA0			//	高电平输入有效

#	LED GPIO 分配
LED0	<==>	PF9			//	低电平灯亮
LED1	<==>	PF10		//	低电平灯亮

#	蜂鸣器 GPIO 分配
BEEP	<==>	PF8			//	高电平发声
****************************************************************************************************************************
*/
//	KEY
#define		KEY_UP_GPIO_PORT			GPIOA
#define		KEYX_GPIO_PORT				GPIOE

#define		KEY_UP_GPIO_PIN				GPIO_Pin_0
#define		KEY0_GPIO_PIN				GPIO_Pin_4
#define		KEY1_GPIO_PIN				GPIO_Pin_3
#define		KEY2_GPIO_PIN				GPIO_Pin_2


//	LED
#define		LED_GPIO_PORT				GPIOF

#define		LED0_GPIO_PIN				GPIO_Pin_9
#define		LED1_GPIO_PIN				GPIO_Pin_10


//	BEEP
#define		BEEP_GPIO_PORT				GPIOF

#define		BEEP_GPIO_PIN				GPIO_Pin_8

INT8 KeyGpioInit(GPIO_TypeDef* keyGpioPort, UINT32 keyGpioPin);
INT8 LedGpioInit(GPIO_TypeDef* ledGpioPort, UINT32 ledGpioPin);
INT8 BeepGpioInit(GPIO_TypeDef* beepGpioPort, UINT32 beepGpioPin);

#endif			//	End of "SYSCONFIG_H"

























