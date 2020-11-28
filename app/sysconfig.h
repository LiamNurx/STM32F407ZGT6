/*
****************************************************************************************************************************
FileName:			sysconfig.h
Author:				Liam.Nurx
Date:				2020.11.14

Description:
	1.�Կ�������Ӳ����Դ�� GPIO �˿ڽ����ض���;
	
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
#	���� GPIO ����:
KEY0	<==>	PE4			//	�͵�ƽ������Ч
KEY1	<==>	PE3			//	�͵�ƽ������Ч
KEY2	<==>	PE2			//	�͵�ƽ������Ч
WK_UP	<==>	PA0			//	�ߵ�ƽ������Ч

#	LED GPIO ����
LED0	<==>	PF9			//	�͵�ƽ����
LED1	<==>	PF10		//	�͵�ƽ����

#	������ GPIO ����
BEEP	<==>	PF8			//	�ߵ�ƽ����
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

//	PRIORITY_GROUP
#define		PRIORITY_GROUP_0			0				//	����������ϵͳ���жϷ���
#define		PRIORITY_GROUP_1			1				//	����������Ӧ�ü��жϷ���

//	PRIORITY_SUB
/* ϵͳ���ж����ȼ����� */
#define		PRI_GROUP0_SUB0_			0
#define		PRI_GROUP0_SUB1_			1
#define		PRI_GROUP0_SUB2_			2
#define		PRI_GROUP0_SUB3_			3
#define		PRI_GROUP0_SUB4_			4
#define		PRI_GROUP0_SUB5_			5
#define		PRI_GROUP0_SUB6_			6
#define		PRI_GROUP0_SUB7_			7
/* Ӧ�ü��ж����ȼ����� */
#define		PRI_GROUP1_SUB0_			0
#define		PRI_GROUP1_SUB1_			1
#define		PRI_GROUP1_SUB2_			2
#define		PRI_GROUP1_SUB3_KEYUP		3
#define		PRI_GROUP1_SUB4_KEY0		4
#define		PRI_GROUP1_SUB5_KEY1		5
#define		PRI_GROUP1_SUB6_KEY2		6
#define		PRI_GROUP1_SUB7_DEBUG		7


INT8 SysConfigInit(void);
INT8 KeyUpGpioInit(void);
INT8 KeyxGpioInit(UINT32 keyGpioPin);
INT8 LedGpioInit(UINT32 ledGpioPin);
INT8 BeepGpioInit(void);
INT8 LedOnOff(GPIO_TypeDef* ledPort, UINT16 ledPin, BitAction ledStatus);
INT8 BeepOnOff(GPIO_TypeDef* beepPort, UINT16 beepPin, BitAction beepStatus);

#endif			//	End of "SYSCONFIG_H"

























