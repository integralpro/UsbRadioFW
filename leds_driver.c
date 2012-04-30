/*
 * led_driver.c
 *
 *  Created on: Dec 30, 2011
 *      Author: pavel_pro
 */

#include <stm32f10x.h>
#include <leds_driver.h>
#include <gpio_driver.h>

#define LED_B ((uint8_t)0x02)

#define SET_LED_MASK(led, pin_number) \
	if(bitmask & led) { \
		if(state & led) { \
			setter |= GPIO_BSRR_BS##pin_number; \
		} else { \
			setter |= GPIO_BSRR_BR##pin_number; \
		} \
	}

#define SET_LED(led, pin_number) \
	if(state & led) { \
		setter |= GPIO_BSRR_BS##pin_number; \
	} else { \
		setter |= GPIO_BSRR_BR##pin_number; \
	}

void leds_init() {
	gpio_mode(GPIOB, 13, GPIO_MODE_OUT_10 | GPIO_CFGO_PUSH_PULL);
	gpio_mode(GPIOB, 14, GPIO_MODE_OUT_10 | GPIO_CFGO_PUSH_PULL);
	gpio_mode(GPIOB, 15, GPIO_MODE_OUT_10 | GPIO_CFGO_PUSH_PULL);
}

void leds_set_mask(uint8_t state, uint8_t bitmask) {
	uint32_t setter = 0;
	SET_LED_MASK(LED_A, 13);
	SET_LED_MASK(LED_B, 14);
	SET_LED_MASK(LED_C, 15);
	GPIOB->BSRR = setter;
}

void leds_set(uint8_t state) {
	uint32_t setter = 0;
	SET_LED(LED_A, 13);
	SET_LED(LED_B, 14);
	SET_LED(LED_C, 15);
	GPIOB->BSRR = setter;
}
