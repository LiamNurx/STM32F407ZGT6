/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    06-March-2015
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_usart.h"

#include "app_usart.h"
#include "app_sysconfig.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
extern unsigned int g1MsStkCounter;
void SysTick_Handler(void)
{
	g1MsStkCounter++;
}

/*
**********************************************************************************************************************
USART1_IRQHandler:
	1.若读取数据寄存器不为空(USART_SR 的 RXNE 位为1)，则读取接收到的数据放入缓存队列;

Note:
	1.状态位 RXNE 由硬件自动置1，可以通过对 USART_DR 寄存器执行读入操作进行清零，从而达到清中断的效果;
**********************************************************************************************************************
*/
void USART1_IRQHandler(void)
{	
	if(SET == USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE))
	{
		UsartRxBuffStore((UINT8)USART_ReceiveData(DEBUG_USART));
	}
}

/*
**********************************************************************************************************************
Note:

**********************************************************************************************************************
*/
void EXTI0_IRQHandler(void)
{
	static UINT8 sStatus = 1;
	
	if(SET == EXTI_GetITStatus(EXTI_Line0))
	{
		sStatus++;

		if(0 == sStatus % 2)
		{
			LedOnOff(LED_GPIO_PORT, LED0_GPIO_PIN, Bit_SET);
			LedOnOff(LED_GPIO_PORT, LED1_GPIO_PIN, Bit_SET);
		}
		else
		{
			LedOnOff(LED_GPIO_PORT, LED0_GPIO_PIN, Bit_RESET);
			LedOnOff(LED_GPIO_PORT, LED1_GPIO_PIN, Bit_RESET);
		}
		
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

void EXTI1_IRQHandler(void)
{
	
}

void EXTI2_IRQHandler(void)
{

}

void EXTI3_IRQHandler(void)
{

}

void EXTI4_IRQHandler(void)
{

}

void EXTI9_5_IRQHandler(void)
{

}

void EXTI15_10_IRQHandler(void)
{
	
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
