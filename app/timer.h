/*
****************************************************************************************************************************
FileName:			timer.h
Author:				Liam.Nurx
Date:				2020.11.29
Version:			00.00.01

Description:
****************************************************************************************************************************
*/


#ifndef		TIMER_H
#define		TIMER_H

#define		STK_CTRL_ENABLE_BIT				0
#define		STK_CTRL_TICKINT_BIT			1
#define		STK_CTRL_CLKSOURCE_BIT			2
#define		STK_CTRL_COUNTFLAG_BIT			16

#define		SYSTICK_TIMER_ENABLE			(1 << STK_CTRL_ENABLE_BIT)
#define		SYSTICK_TIMER_TICKINT			(1 << STK_CTRL_TICKINT_BIT)
#define		SYSTICK_TIMER_CLKSOURCE			(1 << STK_CTRL_CLKSOURCE_BIT)			//	Processor clock (AHB)


#include "sysconfig.h"

INT8 SysTickTimerInit(void);

#endif		End of "TIMER_H"



























