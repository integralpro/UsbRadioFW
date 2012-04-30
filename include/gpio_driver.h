/*
 * gpio_driver.h
 *
 *  Created on: Jan 3, 2012
 *      Author: pavel_pro
 */

#ifndef GPIO_DRIVER_H_
#define GPIO_DRIVER_H_

#define GPIO_CFGI_ANALOG		((uint8_t)(0x00 << 2))
#define GPIO_CFGI_FLOAT			((uint8_t)(0x01 << 2))
#define GPIO_CFGI_PUSH_PULL		((uint8_t)(0x02 << 2))

#define GPIO_CFGO_PUSH_PULL		((uint8_t)(0x00 << 2))
#define GPIO_CFGO_OPEN_DRAIN	((uint8_t)(0x01 << 2))
#define GPIO_CFGO_A_PUSH_PULL	((uint8_t)(0x02 << 2))
#define GPIO_CFGO_A_OPEN_DRAIN	((uint8_t)(0x03 << 2))

#define GPIO_MODE_INPUT	 		((uint8_t)0x00)
#define GPIO_MODE_OUT_10 		((uint8_t)0x01)
#define GPIO_MODE_OUT_2  		((uint8_t)0x02)
#define GPIO_MODE_OUT_50 		((uint8_t)0x03)

#define gpio_mode(port, pin, mode) \
	(*(((uint32_t *)(port)) + ((pin) / 8))) = (((*(((uint32_t *)(port)) + ((pin) / 8))) & ~((uint32_t)0x0F << 4*((pin) % 8))) | ((mode) << 4*((pin) % 8)))

#define gpio_set(port, set) (port->BSRR = ((set) & 0xFFFF))
#define gpio_reset(port, reset) (port->BSRR = (((reset) << 16) & 0xFFFF0000))
#define gpio_set_reset(port, set, reset) (port->BSRR = (((set) & 0xFFFF) | (((reset) << 16) & 0xFFFF0000)))
#define gpio_get(port) (port->IDR)

#endif /* GPIO_DRIVER_H_ */
