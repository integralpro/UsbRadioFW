/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : stm32f10x_it.c
* Author             : MCD Application Team
* Version            : V3.3.0
* Date               : 21-March-2011
* Description        : Main Interrupt Service Routines.
*                      This file provides template for all exceptions handler
*                      and peripherals interrupt service routine.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <stm32f10x_it.h>
#include <usb_lib.h>
#include <usb_istr.h>
#include <usb_pwr.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//__IO uint8_t Send_Buffer[2];
//extern __IO uint8_t PrevXferComplete;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/*******************************************************************************
* Function Name  : USB_LP_CAN1_RX0_IRQHandler
* Description    : This function handles USB Low Priority or CAN RX0 interrupts
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	USB_Istr();
}

/*******************************************************************************
* Function Name  : USB_HP_CAN1_TX_IRQHandler
* Description    : This function handles USB High Priority or CAN TX interrupts requests
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_HP_CAN1_TX_IRQHandler(void)
{
	CTR_HP();
}

///*******************************************************************************
//* Function Name  : DMA1_Channel1_IRQHandler
//* Description    : This function handles DMA1 Channel 1 interrupt request.
//* Input          : None
//* Output         : None
//* Return         : None
//*******************************************************************************/
//void DMA1_Channel1_IRQHandler(void)
//{
//  Send_Buffer[0] = 0x07;
//
//  if((ADC_ConvertedValueX >>4) - (ADC_ConvertedValueX_1 >>4) > 4)
//  {
//    if (PrevXferComplete)
//    {
//      Send_Buffer[1] = (uint8_t)(ADC_ConvertedValueX >>4);
//
//      /* Write the descriptor through the endpoint */
//      USB_SIL_Write(EP1_IN, (uint8_t*) Send_Buffer, 2);
//
//      SetEPTxValid(ENDP1);
//
//      ADC_ConvertedValueX_1 = ADC_ConvertedValueX;
//      PrevXferComplete = 0;
//    }
//  }
//
//  DMA_ClearFlag(DMA1_FLAG_TC1);
//}
//
///*******************************************************************************
//* Function Name  : EXTI9_5_IRQHandler
//* Description    : This function handles External lines 9 to 5 interrupt request.
//* Input          : None
//* Output         : None
//* Return         : None
//*******************************************************************************/
//void EXTI9_5_IRQHandler(void)
//{
//  if(EXTI_GetITStatus(KEY_BUTTON_EXTI_LINE) != RESET)
//  {
//    if (PrevXferComplete)
//    {
//      Send_Buffer[0] = 0x05;
//
//      if (STM_EVAL_PBGetState(Button_KEY) == Bit_RESET)
//      {
//        Send_Buffer[1] = 0x01;
//      }
//      else
//      {
//        Send_Buffer[1] = 0x00;
//      }
//
//      /* Write the descriptor through the endpoint */
//      USB_SIL_Write(EP1_IN, (uint8_t*) Send_Buffer, 2);
//
//      SetEPTxValid(ENDP1);
//      PrevXferComplete = 0;
//    }
//    /* Clear the EXTI line  pending bit */
//    EXTI_ClearITPendingBit(KEY_BUTTON_EXTI_LINE);
//  }
//}
//
///*******************************************************************************
//* Function Name  : EXTI15_10_IRQHandler
//* Description    : This function handles External lines 15 to 10 interrupt request.
//* Input          : None
//* Output         : None
//* Return         : None
//*******************************************************************************/
//void EXTI15_10_IRQHandler(void)
//{
//  if(EXTI_GetITStatus(TAMPER_BUTTON_EXTI_LINE) != RESET)
//  {
//    if (PrevXferComplete)
//    {
//      Send_Buffer[0] = 0x06;
//
//      if (STM_EVAL_PBGetState(Button_TAMPER) == Bit_RESET)
//      {
//        Send_Buffer[1] = 0x01;
//      }
//      else
//      {
//        Send_Buffer[1] = 0x00;
//      }
//
//      /* Write the descriptor through the endpoint */
//      USB_SIL_Write(EP1_IN, (uint8_t*) Send_Buffer, 2);
//
//#ifndef STM32F10X_CL
//      SetEPTxValid(ENDP1);
//#endif /* STM32F10X_CL */
//
//      PrevXferComplete = 0;
//    }
//    /* Clear the EXTI line 13 pending bit */
//    EXTI_ClearITPendingBit(TAMPER_BUTTON_EXTI_LINE);
//  }
//}

void TIM1_UP_IRQHandler()
{

}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
