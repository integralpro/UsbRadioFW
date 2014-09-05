/*
 * main.c
 *
 *  Created on: Dec 29, 2011
 *      Author: pavel_pro
 */

#include <hw_config.h>
#include <stdlib.h>

#include <usb_lib.h>
#include <usb_pwr.h>
#include <delay.h>
#include <leds_driver.h>
#include <gpio_driver.h>
#include <si4705.h>

#include <si4705_conf.h>

static uint16_t ccr2[100];

//bit_word_addr = bit_band_base + (byte_offset x 32) + (bit_number x 4)
#define GPIOA_PIN10 ((uint32_t *)(PERIPH_BB_BASE + (((uint32_t)&GPIOA->IDR - PERIPH_BASE) * 32) + (10 * 4)))

static int c = 0;

void TIM1_CC_IRQHandler() {
	//uint32_t x = *GPIOA_PIN10;

	if(c & 1) {
		uint32_t y = (GPIOA->IDR & (1 << RF_DOUT)) >> RF_DOUT;

		if(y & 1) {
			leds_set_mask(LED_A, LED_A);
		} else {
			leds_set_mask(0, LED_A);
		}
	}

	c = (c + 1) % 50;

	TIM1->SR &= ~TIM_SR_CC2IF;
}

static void audio_di_init() {
	int i;

	/*
	for(i=0; i<32; i++) {
		ccr2[i] = i*4;
	}

	for(i=0; i<32; i++) {
		ccr2[32+i] = 150 + i*4;
	}
	*/

	for(i=0; i<100; i++) {
		ccr2[i] = i*3;
	}

	RCC->APB2ENR |= RCC_AHBENR_DMA1EN;
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

	gpio_mode(GPIOA, RF_DOUT, GPIO_MODE_INPUT | GPIO_CFGI_PUSH_PULL);
	gpio_mode(GPIOA, RF_DFS, GPIO_MODE_OUT_10 | GPIO_CFGO_A_PUSH_PULL);
	gpio_mode(GPIOA, RF_DCLK, GPIO_MODE_OUT_10 | GPIO_CFGO_A_PUSH_PULL);

	TIM1->PSC = 5 - 1;
	TIM1->ARR = 299;
	TIM1->CCR1 = 150;
	TIM1->EGR = TIM_EGR_UG;
	TIM1->CCMR1 = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 |
				  TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_0;
	TIM1->CCER = TIM_CCER_CC1E | TIM_CCER_CC2E;
	TIM1->BDTR = TIM_BDTR_MOE;

	TIM1->DIER = TIM_DIER_CC2DE | TIM_DIER_CC2IE;

	TIM1->DCR = TIM_DCR_DBA_1 | TIM_DCR_DBA_2 | TIM_DCR_DBA_3;

	DMA1_Channel3->CMAR = (unsigned int)ccr2;
	DMA1_Channel3->CPAR = (unsigned int)&TIM1->DMAR;
	DMA1_Channel3->CNDTR = sizeof(ccr2)/sizeof(ccr2[0]);
	DMA1_Channel3->CCR = DMA_CCR3_MINC | DMA_CCR3_CIRC | DMA_CCR3_DIR |
											 DMA_CCR3_MSIZE_0 | DMA_CCR3_PSIZE_0;
	DMA1_Channel3->CCR |= DMA_CCR3_EN;

	TIM1->EGR = TIM_EGR_CC2G;
}

#define audio_clk_en() (TIM1->CR1 = TIM_CR1_CEN)

void interrupts_config(void)
{
	NVIC_SetPriorityGrouping(SCB_AIRCR_PRIGROUP2);

	NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 1));
	NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);

	NVIC_SetPriority(TIM1_CC_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0));
}

void InitAll(void) {
	interrupts_config();
	delay_init();

	leds_init();

	RCC->CFGR &= ~RCC_CFGR_USBPRE;		// Select USBCLK source
	RCC->APB1ENR |= RCC_APB1ENR_USBEN;	// Enable the USB clock
	USB_Init();

	si4705_init();
}

int main() {

	InitAll();

	leds_set_mask(LED_A | LED_B | LED_C, LED_A | LED_B | LED_C);
	//delay_ms(500);
	leds_set_mask(0, LED_A | LED_B | LED_C);
	//delay_ms(500);

	while(bDeviceState != CONFIGURED);

	/*
	leds_set_mask(LED_A | LED_B | LED_C, LED_A | LED_B | LED_C);

	uint8_t x = 0;
	x = si4705_powerup(SI4705_MODE_ANALOG | SI4705_MODE_DIGITAL);

	SI4705_REV rev;
	x = si4705_getrev(&rev);

	uint16_t freq;
	x = si4705_getprop(SI4705_PROP_REFCLK_FREQ, &freq);

	//x = si4705_seek(1);

	x = si4705_setprop(SI4705_PROP_DIGITAL_OUTPUT_FORMAT,
					SI4705_OFALL_RE | SI4705_OMODE_LJ | SI4705_OMONO_BLEND | SI4705_OSIZE_16);

	audio_di_init();
	audio_clk_en();
	delay_ms(1);
	x = si4705_setprop(SI4705_PROP_DIGITAL_OUTPUT_SAMPLE_RATE, 48000);
	x = si4705_getprop(SI4705_PROP_DIGITAL_OUTPUT_SAMPLE_RATE, &freq);

	x = si4705_tune(10320);

	NVIC_EnableIRQ(TIM1_CC_IRQn);
	*/

	//if(x == SI4705_STATUS_OK) {
	//	leds_set_mask(LED_C, LED_C);
	//}

	/*
	while(1) {
		leds_set_mask(LED_A | LED_B | LED_C, LED_A | LED_B | LED_C);
		delay_ms(500);
		leds_set_mask(0, LED_A | LED_B | LED_C);
		delay_ms(500);
	}
	*/
	while(1);

	return 0;
}
