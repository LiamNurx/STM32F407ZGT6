/*
****************************************************************************************************************************
FileName:			exti.c
Author:				Liam.Nurx
Date:				2020.11.23

外部中断的使用示例:
	1.函数 GPIO_Init() 配置相应 I/O 为输入模式;
	2.函数 SYSCFG_EXTILineConfig() 将对应 I/O 复用到中断线;
	3.函数 EXTI_Init() 配置中断/事件模式和触发方式;
	4.函数 NVIC_Init() 将中断线映射到相应的 NVIC 中断通道;
****************************************************************************************************************************
*/

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_syscfg.h"

#include "exti.h"
#include "sysconfig.h"

/*
****************************************************************************************************************************
Note:
	1.KeyUp		<==>	PA0					//	高电平输入有效
	2.使能 SYSCFG 外设时钟;					//	需要配置寄存器: SYSCFG_EXTICRx
	3.初始化按键对应的 GPIO;
	4.将对应的 I/O 复用到中断线上;
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



































