/*
 * leds_driver.h
 *
 *  Created on: Dec 30, 2011
 *      Author: pavel_pro
 */

#ifndef _LEDS_DRIVER_H_
#define _LEDS_DRIVER_H_

#define LED_A ((uint8_t)0x01)
//#define LED_B ((uint8_t)0x02)
#define LED_C ((uint8_t)0x04)

void leds_init();
void leds_set(uint8_t state);
void leds_set_mask(uint8_t state, uint8_t bitmask);

#endif /* _LEDS_DRIVER_H_ */
