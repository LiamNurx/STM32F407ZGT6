/*
****************************************************************************************************************************
FileName:			app_uart.h
Author:				Liam.Nurx
Date:				2020.11.14

Description:
	1.
****************************************************************************************************************************
*/

#ifndef				APP_USART_H
#define				APP_USART_H

#include "app_sysconfig.h"

#define				DEBUG_EN						TRUE						//	
#define				DEBUG_USART						USART1						//	
#define				DEBUG_BOUND_RATE				115200						//	

#define				MAX_FIFO_BUFF_LENGTH			512							//	

typedef	struct
{
	UINT8			element[MAX_FIFO_BUFF_LENGTH];								//	512 Bytes
	UINT16			front;														//	2 Bytes
	UINT16			rear;														//	2 Bytes
}CIRCULAR_QUEUE;																//	==> 516 Bytes

INT8 InitDebugUsart1(UINT32 boundRate);
INT8 InitUsartRxBuff(CIRCULAR_QUEUE *usartRxBuff);
INT8 UsartRxBuffStore(UINT8 ch);
INT8 UsartRxBuffTest(void);
INT8 EnQueue(CIRCULAR_QUEUE *fifo, UINT8 byteData);
INT8 DeQueue(CIRCULAR_QUEUE *fifo);
INT16 GetQueueLength(CIRCULAR_QUEUE *fifo);
#endif	//	End of "APP_UART_H"












