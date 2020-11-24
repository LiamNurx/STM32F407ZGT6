/*
****************************************************************************************************************************
FileName:			uart.h
Author:				Liam.Nurx
Date:				2020.11.14

Description:
	1.
****************************************************************************************************************************
*/

#ifndef				USART_H
#define				USART_H

#include "sysconfig.h"

#define				DEBUG_EN						TRUE						//	默认 DEBUG 使能
#define				DEBUG_USART						USART1						//	默认使用 USART1 作为调试串口
#define				DEBUG_BOUND_RATE				115200						//	调试串口波特率

#define				MAX_FIFO_BUFF_LENGTH			512							//	最大缓存为 512 Bytes

typedef	struct
{
	UINT8			element[MAX_FIFO_BUFF_LENGTH];								//	512 Bytes
	UINT16			front;														//	2 Bytes
	UINT16			rear;														//	2 Bytes
}CIRCULAR_QUEUE;																//	==> 516 Bytes

INT8 DebugUsart1Init(UINT32 boundRate);
INT8 UsartRxBuffInit(CIRCULAR_QUEUE *usartRxBuff);
INT8 UsartRxBuffStore(UINT8 ch);
INT8 UsartRxBuffTest(void);
INT8 EnQueue(CIRCULAR_QUEUE *fifo, UINT8 byteData);
INT8 DeQueue(CIRCULAR_QUEUE *fifo);
INT16 GetQueueLength(CIRCULAR_QUEUE *fifo);
#endif	//	End of "UART_H"












