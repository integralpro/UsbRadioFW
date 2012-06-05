/*
 * led_driver.c
 *
 *  Created on: Dec 30, 2011
 *      Author: pavel_pro
 */

#include <stm32f10x.h>
#include <leds_driver.h>
#include <gpio_driver.h>

#define LED_A_NUM 12
#define LED_B_NUM 13
#define LED_C_NUM 14

#define MACRO_GPIO_BSRR_BS(x) GPIO_BSRR_BS##x
#define MACRO_GPIO_BSRR_BR(x) GPIO_BSRR_BR##x

#define SET_LED_MASK(led, pin_number) \
	if(bitmask & led) { \
		if(state & led) { \
			setter |= MACRO_GPIO_BSRR_BS(pin_number); \
		} else { \
			setter |= MACRO_GPIO_BSRR_BR(pin_number); \
		} \
	}

#define SET_LED(led, pin_number) \
	if(state & led) { \
		setter |= MACRO_GPIO_BSRR_BS(pin_number); \
	} else { \
		setter |= MACRO_GPIO_BSRR_BR(pin_number); \
	}

void leds_init() {
	gpio_mode(GPIOB, LED_A_NUM, GPIO_MODE_OUT_10 | GPIO_CFGO_PUSH_PULL);
	gpio_mode(GPIOB, LED_B_NUM, GPIO_MODE_OUT_10 | GPIO_CFGO_PUSH_PULL);
	gpio_mode(GPIOB, LED_C_NUM, GPIO_MODE_OUT_10 | GPIO_CFGO_PUSH_PULL);
}

void leds_set_mask(uint8_t state, uint8_t bitmask) {
	uint32_t setter = 0;
	SET_LED_MASK(LED_A, LED_A_NUM);
	SET_LED_MASK(LED_B, LED_B_NUM);
	SET_LED_MASK(LED_C, LED_C_NUM);
	GPIOB->BSRR = setter;
}

void leds_set(uint8_t state) {
	uint32_t setter = 0;
	SET_LED(LED_A, LED_A_NUM);
	SET_LED(LED_B, LED_A_NUM);
	SET_LED(LED_C, LED_C_NUM);
	GPIOB->BSRR = setter;
}
