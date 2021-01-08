/*
****************************************************************************************************************************
FileName:			app_usart.c
Author:				Liam.Nurx
Date:				2020.11.14
Version:			00.00.01

Description:
****************************************************************************************************************************
*/

#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "stm32f4xx.h"
#include "app_sysconfig.h"
#include "app_usart.h"

CIRCULAR_QUEUE		gDebugRxBuff;	
CIRCULAR_QUEUE		*gpDebugRxBuff = &gDebugRxBuff;

#if DEBUG_EN
#pragma import(__use_no_semihosting)             

struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       

_sys_exit(int x) 
{ 
	x = x; 
} 

/*
**********************************************************************************************************************
fputc:
	1.等待数据发送完成(TC 置1)，将待发送数据传入 USART_DR;

Note:
	1.TC 位可以通过读取寄存器 USART_SR 然后写入寄存器 USART_DR 来清零; 
**********************************************************************************************************************
*/
int fputc(int ch, FILE *f)
{ 	
	while(RESET == USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TC))
	{
		;
	}

	USART_SendData(DEBUG_USART, (uint16_t) ch);

	return ch;
}
#endif				//	End of "DEBUG_EN"


/*
**********************************************************************************************************************
InitDebugUsart1:
	1.使能 DEBUG_USART 对应的 GPIO 外设时钟;
	2.使能 DEBUG_USART 对应的外设时钟;
	3.DEBUG_USART 对应的 GPIO 的初始化参数配置;
	4.DEBUG_USART 初始化参数配置;
	5.DEBUG_USART 使能;
	6.DEBUG_USART 中断初始化配置;

Note:
	USART1		<==>	(PA9_TX , PA10_RX)	/	(PB6_TX , PB7_RX)
	USART2		<==>	(PA2_TX , PA3_RX)	/	(PD5_TX , PD6_RX)
	USART3		<==>	(PB10_TX , PB11_RX)	/	(PC10_TX , PC11_RX)	/	(PD8_TX , PD9_RX)
	UART4		<==>	(PA0_TX , PA1_RX)	/	(PC10_TX , PC11_RX)
	UART5		<==>	(PC12_TX , PD2_RX)
	USART6		<==>	(PC6_TX , PC7_RX)	/	(PG14_TX ,PG9_RX)	
**********************************************************************************************************************
*/
INT8 InitDebugUsart1(UINT32 boundRate)
{
	GPIO_InitTypeDef 		debugGpioInitStruct;
	USART_InitTypeDef		debugUsartInitStruct;
	NVIC_InitTypeDef		usartNvicInitStruct;

	if((9600 != boundRate) && (115200 != boundRate))
	{
		#if DEBUG_EN
		printf("BOUNDRATE SET ERROR. DebugUsart1Init failed! <===> [fault]\r\n");
		#endif
		
		return -1;
	}

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);				//	Set the "GPIOBEN" bit of RCC_AHB1ENR register.
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);				//	Set the "USART1EN" bit of RCC_APB2ENR register.

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);			//	PB6 ==> USART1_TX
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);			//	PB6 ==> USART1_TX

	debugGpioInitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	debugGpioInitStruct.GPIO_Mode = GPIO_Mode_AF;
	debugGpioInitStruct.GPIO_OType = GPIO_OType_PP;
	debugGpioInitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	debugGpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &debugGpioInitStruct);								//	PB6 ==> USART1_TX GPIO_Init

	debugUsartInitStruct.USART_BaudRate = boundRate;
	debugUsartInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	debugUsartInitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	debugUsartInitStruct.USART_Parity = USART_Parity_No;
	debugUsartInitStruct.USART_StopBits = USART_StopBits_1;
	debugUsartInitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &debugUsartInitStruct);
	USART_Cmd(USART1, ENABLE);

	usartNvicInitStruct.NVIC_IRQChannel = USART1_IRQn;
	usartNvicInitStruct.NVIC_IRQChannelCmd = ENABLE;
	usartNvicInitStruct.NVIC_IRQChannelPreemptionPriority = PRIORITY_GROUP_1;
	usartNvicInitStruct.NVIC_IRQChannelSubPriority = PRI_GROUP1_SUB7_DEBUG;
	NVIC_Init(&usartNvicInitStruct);

	return 0;
}

/*
**********************************************************************************************************************
InitUsartRxBuff:
	1.初始化指定串口缓存队列;

Note:
	1.函数 memset() 按字节初始化数据;
**********************************************************************************************************************
*/
INT8 InitUsartRxBuff(CIRCULAR_QUEUE *usartRxBuff)
{
	if(NULL == usartRxBuff)
	{
		#if DEBUG_EN
		printf("Pointer parameter is NULL. UsartRxBuffInit failed! <===> [fault]\r\n");
		#endif

		return -1;
	}

	memset(usartRxBuff, 0, sizeof(CIRCULAR_QUEUE));
	
	#if DEBUG_EN
	printf("front: %d   rear: %d\r\n", usartRxBuff->front, usartRxBuff->rear);
	#endif

	return 0;
}


/*
**********************************************************************************************************************
UsartRxBuffStore:
	1.将收到的数据缓存入队列;
	2.若缓存队列已满，新数据被丢弃不入队列;

Note:
**********************************************************************************************************************
*/
INT8 UsartRxBuffStore(UINT8 rxByte)
{
	INT8 result;

	result = EnQueue(gpDebugRxBuff, rxByte);

	if(result)
	{
		#if DEBUG_EN
		printf("gDebugRxBuff is full. UsartRxBuffStore failed! <===> [fault]\r\n");
		#endif
	}

	return result;
}


/*
**********************************************************************************************************************
UsartRxBuffTest:
	1.若 DEBUG_USART 的接收缓存队列有更新，则将更新的数据发送出去;

Note:
	1.本函数主要是用于测试构建的循环队列处理函数是否能正常工作，验证是否不会出现数据溢等问题;
	2.主要针对以下函数:
		EnQueue();				//	Test OK!
		DeQueue();				//	Test OK!
		GetQueueLength();		//	Test OK!
**********************************************************************************************************************
*/
INT8 UsartRxBuffTest(void)
{
	while(gpDebugRxBuff->front != gpDebugRxBuff->rear)
	{
		while(RESET == USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TC))
		{
			;
		}

		USART_SendData(DEBUG_USART, (uint16_t) gpDebugRxBuff->element[gpDebugRxBuff->front]);
		gpDebugRxBuff->front = (gpDebugRxBuff->front + 1) % MAX_FIFO_BUFF_LENGTH;
	}
	
	return 0;
}

/*
*************************************************************************************************************************
EnQueue:
	1.在指定队列的队首处入队一个字节的数据;
	2.若队列已满，则新数据丢弃，维持原队列数据不变;

Note:
	1.队列已满的判断条件为: front == (rear + 1) % MAX_SIZE;
*************************************************************************************************************************
*/
INT8 EnQueue(CIRCULAR_QUEUE *fifo, UINT8 newByte)
{
	if(NULL == fifo) 
	{
		#if DEBUG_EN
		printf("Pointer parameter is NULL. EnQueue failed! <===> [fault]\r\n");
		#endif

		return -1;
	}
	
	if(fifo->front == (fifo->rear + 1) % MAX_FIFO_BUFF_LENGTH)
	{
		#if DEBUG_EN
		printf("Queue is full. EnQueue failed! <===> [fault]\r\n");
		#endif
		
		return -2;
	}
	
	fifo->element[fifo->front] = newByte;
	fifo->rear = (fifo->rear + 1) % MAX_FIFO_BUFF_LENGTH;
	
	return 0;
}



/*
*************************************************************************************************************************
DeQueue:
	1.对指定队列的队头位置出队1字节的数据;
	2.若队列为空，则返回;

Note:
	1.队列为空的判定条件: rear == front;
*************************************************************************************************************************
*/
INT8 DeQueue(CIRCULAR_QUEUE *fifo)
{
	if(NULL == fifo)
	{
		#if DEBUG_EN
		printf("Pointer parameter is NULL. DeQueue failed! <===> [fault]\r\n");
		#endif
		
		return -1;
	}
	
	if(fifo->front == fifo->rear)
	{
		#if DEBUG_EN
		printf("Queue is empty. DeQueue failed! <===> [fault]\r\n");
		#endif
		
		return -2;
	}
	
	#if DEBUG_EN
	printf("fifo.[%d] = %d is out from fifo.\r\n", fifo->front, fifo->element[fifo->front]);
	#endif
	
	fifo->front = (fifo->front + 1) % MAX_FIFO_BUFF_LENGTH;
	
	return 0;
}

/*
*************************************************************************************************************************
GetQueueLength:
	1.获取指定队列的长度;
	
Note:
*************************************************************************************************************************
*/
INT16 GetQueueLength(CIRCULAR_QUEUE *fifo)
{
	if(NULL == fifo)
	{
		#if DEBUG_EN
		printf("Pointer parameter is NULL. GetQueueLength failed! <===> [fault]\r\n");
		#endif		
		
		return -1;
	}
	
	return ((fifo->rear - fifo->front + MAX_FIFO_BUFF_LENGTH) % MAX_FIFO_BUFF_LENGTH);
}




