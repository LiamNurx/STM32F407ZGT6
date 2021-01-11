/*
****************************************************************************************************************************
FileName:			app_exti.c
Author:				Liam.Nurx
Date:				2020.11.23

Description:
	1.本文件主要用于管理外部中断;
****************************************************************************************************************************
*/

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_syscfg.h"

#include "app_exti.h"
#include "app_sysconfig.h"

/*
****************************************************************************************************************************
InitKeyUpExti:
	1.系统配置控制器外设时钟使能;
	2.按键(KEY_UP)对应 GPIO 初始化配置;
	3.将按键(KEY_UP)对应的 GPIO 连接映射到外部中断线 EXTI;
	4.外部中断初始化;
	5.配置选定的外部中断对应 NVIC 的中断使能、中断优先级，使得外部中断能够正常及时响应;

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



































