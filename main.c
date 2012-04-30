/*
 * main.c
 *
 *  Created on: Dec 29, 2011
 *      Author: pavel_pro
 */

#include <stm32f10x.h>
#include <stdlib.h>

#include <usb_lib.h>
#include <usb_pwr.h>
#include <delay.h>
#include <leds_driver.h>
#include <gpio_driver.h>
#include <si4705.h>

__IO uint8_t PrevXferComplete = 1;

void interrupts_config(void)
{
	//__enable_irq();

	NVIC_SetPriorityGrouping(SCB_AIRCR_PRIGROUP2);

	NVIC_SetPriority(USB_HP_CAN1_TX_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0));
	NVIC_EnableIRQ(USB_HP_CAN1_TX_IRQn);

	NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 1));
	NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);

	//NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 2));
	//NVIC_EnableIRQ(TIM2_IRQn);

//  /* Enable the EXTI9_5 Interrupt */
//  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//  NVIC_Init(&NVIC_InitStructure);
//
//  /* Enable the EXTI15_10 Interrupt */
//  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//  NVIC_Init(&NVIC_InitStructure);
//
//  /* Enable the DMA1 Channel1 Interrupt */
//  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//  NVIC_Init(&NVIC_InitStructure);
}

void InitAll(void) {
	interrupts_config();
	delay_init();

	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;			// Enable clock on port A
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;			// Enable clock on port B

	leds_init();

	RCC->CFGR &= ~RCC_CFGR_USBPRE;		// Select USBCLK source
	RCC->APB1ENR |= RCC_APB1ENR_USBEN;	// Enable the USB clock

	USB_Init();

	si4705_init();
}

int main() {
	InitAll();

	//leds_set(LED_A | LED_B | LED_C);

	uint8_t x;
	x = si4705_powerup();

	SI4705_REV rev;
	x = si4705_getrev(&rev);

	//x = si4705_seek(1);
	x = si4705_tune(10320);

	if(x == SI4705_STATUS_OK) {
		leds_set_mask(LED_C, LED_C);
	}

	while(bDeviceState != CONFIGURED);

	leds_set_mask(LED_A, LED_A);

	return 0;
}
