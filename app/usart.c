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

CIRCULAR_QUEUE		gDebugRxBuff;							//	���ڻ���ӵ��Դ��ڴ����յ�������;
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
		;			//	�ȴ����ݷ�����ɡ�	0 == TC : (����δ���)		1 == TC : (���������)
	}

	USART_SendData(DEBUG_USART, (uint16_t) ch);

	return ch;
}
#endif				//	End of "DEBUG_EN"


/*
**********************************************************************************************************************
Note:
	1.ʹ�� DEBUG ��Ӧ GPIO ������ʱ��;
	2.ʹ�� DEBUG ��Ӧ UART ������ʱ��;
	3.DEBUG ��Ӧ GPIO ���Ÿ�������;
	4.��ʼ�� DEBUG ��Ӧ GPIO �ļĴ�������;
	5.��ʼ�� DEBUG ��Ӧ UART �ļĴ�������;
	6.ʹ�� DEBUG ��Ӧ�� UART;

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

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);						//	GPIOA--GPIOK ����ʱ��ʹ��
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);						//	UxART2--UxART5 ����ʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);						//	UxART1/UxART6 ����ʱ��ʹ��

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
	1.�� DEBUG_USART �� RX ���������ݴ��뻺��;
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
	1.���ӵ��Խ��մ��ڽ��յ�������ȫ��ʵʱ��ӡ���;
	2.ÿ���һ���ֽڣ��������ݻ�����ж�ָ��;
**********************************************************************************************************************
*/
INT8 UsartRxBuffTest(void)
{
	while(gpDebugRxBuff->front != gpDebugRxBuff->rear)
	{
		while(RESET == USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TC))
		{
			;			//	�ȴ����ݷ�����ɡ�	0 == TC : (����δ���)		1 == TC : (���������)
		}

		USART_SendData(DEBUG_USART, (uint16_t) gpDebugRxBuff->element[gpDebugRxBuff->front]);
		gpDebugRxBuff->front = (gpDebugRxBuff->front + 1) % MAX_FIFO_BUFF_LENGTH;
	}
	
	return 0;
}



/*
#	�������
*************************************************************************************************************************
Note:
	1.������ָ��Ϊ NULL , ��������ֵΪ: -1
	2.���������������ݶ������������쳣����ֵ: -2;
	3.������δ����������ӣ����¶�βλ�ã�������������: 0;
	4.���ʱ��ֻ���޸Ķ�βָ�뼴�ɣ�
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
#	���ݳ���
*************************************************************************************************************************
Note:
	1.������ָ��Ϊ NULL , ��������ֵΪ: -1
	2.�������ѿգ��������쳣����ֵ: -2;
	3.�����в��գ��Ӷ���ɾ��Ԫ�أ����¶���λ�ã�������������: 0;
	4.����ʱ��ֻ���޸Ķ���ָ�뼴�ɣ�
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
#	��ȡ���г���
*************************************************************************************************************************
Note:
	1.��ָ�����Ϊ NULL , ��������ֵΪ: -1
	2.�����������ض��г���;
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




