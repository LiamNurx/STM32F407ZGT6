/*
****************************************************************************************************************************
FileName:			app_exti.c
Author:				Liam.Nurx
Date:				2020.11.23

****************************************************************************************************************************
*/

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_syscfg.h"

#include "app_exti.h"
#include "app_sysconfig.h"

/*
****************************************************************************************************************************
Note:

****************************************************************************************************************************
*/
void InitKeyUpExti(void)
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
	InitKeyUpGpio();
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	EXTI_Init(&keyUpExtiInitStruct);
	NVIC_Init(&keyUpNvicInitStruct);
}

void InitKey0Exti(void)
{
	
}

void InitKey1Exti(void)
{

}

void InitKey2Exti(void)
{

}



































