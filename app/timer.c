/*
****************************************************************************************************************************
FileName:			timer.c
Author:				Liam.Nurx
Date:				2020.11.29
Version:			00.00.01

Description:
****************************************************************************************************************************
*/

#include "core_cm4.h"

#include "timer.h"
#include "basetype.h"

volatile unsigned int g1MsStkCounter = 0;				//	根据 SysTick 每 1Ms 计数累加一


/*
***********************************************************************************************************************
Note:
	1.SysTick 相关的库函数请参考文件:	misc.c  core_cm4.h;
	2.配置 RELOAD 加载值，使得 SysTick 每 1ms 中断一次;
	3.选择 SysTick 时钟为: AHB Clock;
	4.使能 SysTick 时钟计数;
***********************************************************************************************************************
*/
INT8 SysTickTimerInit(void)
{
	SysTick->LOAD &= 0x00FFFFFF;
	SysTick->LOAD = STK_1MS_RELOAD_VALUE;

	SysTick->CTRL &= ~SYSTICK_TIMER_CLKSOURCE;		//	Clock source selection: Processor clock (AHB/8);
	SysTick->CTRL |= SYSTICK_TIMER_TICKINT;			//	SysTick exception request enable;
	SysTick->CTRL |= SYSTICK_TIMER_ENABLE;			//	Counter enable;
	
	return 0;
}


























