/*
****************************************************************************************************************************
FileName:			uart.c
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
#include "sysconfig.h"
#include "usart.h"

CIRCULAR_QUEUE		gDebugRxBuff;							//	用于缓存从调试串口处接收到的数据;
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

int fputc(int ch, FILE *f)
{ 	
	while(RESET == USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TC))
	{
		;			//	等待数据发送完成。	0 == TC : (传送未完成)		1 == TC : (传送已完成)
	}

	USART_SendData(DEBUG_USART, (uint16_t) ch);

	return ch;
}
#endif				//	End of "DEBUG_EN"


/*
**********************************************************************************************************************
Note:
	1.使能 DEBUG 对应 GPIO 的外设时钟;
	2.使能 DEBUG 对应 UART 的外设时钟;
	3.DEBUG 对应 GPIO 引脚复用配置;
	4.初始化 DEBUG 对应 GPIO 的寄存器配置;
	5.初始化 DEBUG 对应 UART 的寄存器配置;
	6.使能 DEBUG 对应的 UART;

UxART:
	USART1		<==>	(PA9_TX , PA10_RX)	/	(PB6_TX , PB7_RX)
	USART2		<==>	(PA2_TX , PA3_RX)	/	(PD5_TX , PD6_RX)
	USART3		<==>	(PB10_TX , PB11_RX)	/	(PC10_TX , PC11_RX)	/	(PD8_TX , PD9_RX)
	UART4		<==>	(PA0_TX , PA1_RX)	/	(PC10_TX , PC11_RX)
	UART5		<==>	(PC12_TX , PD2_RX)
	USART6		<==>	(PC6_TX , PC7_RX)	/	(PG14_TX ,PG9_RX)	
**********************************************************************************************************************
*/
INT8 DebugUsart1Init(UINT32 boundRate)
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

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);						//	GPIOA--GPIOK 外设时钟使能
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);						//	UxART2--UxART5 外设时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);						//	UxART1/UxART6 外设时钟使能

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);					//	PB6 ==> USART1_TX
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);					//	PB6 ==> USART1_TX

	debugGpioInitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	debugGpioInitStruct.GPIO_Mode = GPIO_Mode_AF;
	debugGpioInitStruct.GPIO_OType = GPIO_OType_PP;
	debugGpioInitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	debugGpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &debugGpioInitStruct);										//	PB6 ==> USART1_TX GPIO_Init

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
	usartNvicInitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	usartNvicInitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&usartNvicInitStruct);
	
	return 0;
}

INT8 UsartRxBuffInit(CIRCULAR_QUEUE *usartRxBuff)
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
Note:
	1.从 DEBUG_USART 的 RX 处接收数据存入缓存;
**********************************************************************************************************************
*/
INT8 UsartRxBuffStore(UINT8 ch)
{
	INT8 result;

	result = EnQueue(gpDebugRxBuff, ch);

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
Note:
	1.将从调试接收串口接收到的数据全部实时打印输出;
	2.每输出一个字节，更新数据缓存队列读指针;
**********************************************************************************************************************
*/
INT8 UsartRxBuffTest(void)
{
	while(gpDebugRxBuff->front != gpDebugRxBuff->rear)
	{
		while(RESET == USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TC))
		{
			;			//	等待数据发送完成。	0 == TC : (传送未完成)		1 == TC : (传送已完成)
		}

		USART_SendData(DEBUG_USART, (uint16_t) gpDebugRxBuff->element[gpDebugRxBuff->front]);
		gpDebugRxBuff->front = (gpDebugRxBuff->front + 1) % MAX_FIFO_BUFF_LENGTH;
	}
	
	return 0;
}



/*
#	数据入队
*************************************************************************************************************************
Note:
	1.若参数指针为 NULL , 函数返回值为: -1
	2.若队列已满，数据丢弃，函数返异常返回值: -2;
	3.若队列未满，数据入队，更新队尾位置，函数正常返回: 0;
	4.入队时，只需修改队尾指针即可；
*************************************************************************************************************************
*/
INT8 EnQueue(CIRCULAR_QUEUE *fifo, UINT8 byteData)
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
	
	fifo->element[fifo->front] = byteData;
	fifo->rear = (fifo->rear + 1) % MAX_FIFO_BUFF_LENGTH;
	
	return 0;
}



/*
#	数据出队
*************************************************************************************************************************
Note:
	1.若参数指针为 NULL , 函数返回值为: -1
	2.若队列已空，函数返异常返回值: -2;
	3.若队列不空，从队首删除元素，更新队首位置，函数正常返回: 0;
	4.出队时，只需修改队首指针即可；
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
#	获取队列长度
*************************************************************************************************************************
Note:
	1.若指针参数为 NULL , 函数返回值为: -1
	2.函数正常返回队列长度;
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




