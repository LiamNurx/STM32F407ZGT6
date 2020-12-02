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

volatile unsigned int g1MsStkCounter = 0;				//	���� SysTick ÿ 1Ms �����ۼ�һ


/*
***********************************************************************************************************************
Note:
	1.SysTick ��صĿ⺯����ο��ļ�:	misc.c  core_cm4.h;
	2.���� RELOAD ����ֵ��ʹ�� SysTick ÿ 1ms �ж�һ��;
	3.ѡ�� SysTick ʱ��Ϊ: AHB Clock;
	4.ʹ�� SysTick ʱ�Ӽ���;
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

























