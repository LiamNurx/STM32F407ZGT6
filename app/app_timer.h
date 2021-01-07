/*
****************************************************************************************************************************
FileName:			app_timer.h
Author:				Liam.Nurx
Date:				2020.11.29
Version:			00.00.01

Description:
****************************************************************************************************************************
*/


#ifndef		APP_TIMER_H
#define		APP_TIMER_H

#include "app_sysconfig.h"

#define		STK_1MS_RELOAD_VALUE			21000

#define		STK_CTRL_ENABLE_BIT				0
#define		STK_CTRL_TICKINT_BIT			1
#define		STK_CTRL_CLKSOURCE_BIT			2
#define		STK_CTRL_COUNTFLAG_BIT			16

#define		SYSTICK_TIMER_ENABLE			(1 << STK_CTRL_ENABLE_BIT)
#define		SYSTICK_TIMER_TICKINT			(1 << STK_CTRL_TICKINT_BIT)
#define		SYSTICK_TIMER_CLKSOURCE			(1 << STK_CTRL_CLKSOURCE_BIT)			//	Processor clock (AHB)

#define		RTC_RATE						32


INT8 InitSysTickTimer(void);

#endif		//	End of "APP_TIMER_H"



























