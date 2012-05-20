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

	////NVIC_SetPriority(USB_HP_CAN1_TX_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0));
	////NVIC_EnableIRQ(USB_HP_CAN1_TX_IRQn);

	NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 1));
	NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);

	////NVIC_SetPriority(TIM1_UP_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 2));
	////NVIC_EnableIRQ(TIM1_CC_IRQn);

	//NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 2));
	//NVIC_EnableIRQ(TIM2_IRQn);
}

void InitAll(void) {
	interrupts_config();
	delay_init();

	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;			// Enable clock on port A
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;			// Enable clock on port B

	////RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

	//gpio_mode(GPIOA, RF_RCLK, GPIO_MODE_OUT_50 | GPIO_CFGO_A_PUSH_PULL);
	////TIM1->PSC = 15 - 1;
	////TIM1->ARR = 100 - 1;
	////TIM1->EGR = TIM_EGR_UG;
	//TIM1->CR2 = 0;

	//TIM2->CCER &= ~(TIM_CCER_CC2E | TIM_CCER_CC2P);
	//TIM1->CCR3 = 122;
	//TIM1->CCMR2 = TIM2->CCMR2 & 0xFF00;
	//TIM1->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1         // PWM 1
	//			|  TIM_CCMR2_OC3PE;                            // enable preload;

	//TIM1->CCER |= TIM_CCER_CC3E;

	////TIM1->DIER |= TIM_DIER_UIE;

	////TIM1->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;

	leds_init();

	RCC->CFGR &= ~RCC_CFGR_USBPRE;		// Select USBCLK source
	RCC->APB1ENR |= RCC_APB1ENR_USBEN;	// Enable the USB clock

	USB_Init();

	si4705_init();
}

int main() {
	InitAll();

	//leds_set_mask(LED_C, LED_C);

	//uint8_t x;
	//x = si4705_powerup();

	//SI4705_REV rev;
	//x = si4705_getrev(&rev);

	//x = si4705_seek(1);
	//x = si4705_tune(10320);

	//if(x == SI4705_STATUS_OK) {
	//	leds_set_mask(LED_C, LED_C);
	//}

	while(bDeviceState != CONFIGURED);

	int led = 1;
	while(1) {
		leds_set_mask(led?LED_A:0, LED_A);
		delay_ms(1000);
		led = (led + 1) % 2;
	}

	return 0;
}
