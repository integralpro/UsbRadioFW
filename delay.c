/*
 * utils.c
 *
 *  Created on: Jan 4, 2012
 *      Author: pavel_pro
 */

#include <stm32f10x.h>
#include <stdlib.h>

volatile uint32_t msTicks; /* timeTicks counter */

void SysTick_Handler(void) {
	msTicks++; /* increment timeTicks counter */
}

__INLINE void delay_init() {
	if (SysTick_Config(SystemCoreClock / 1000)) /* Setup SysTick for 1 msec interrupts */
		exit(1);
}

__INLINE void delay_ms(uint32_t ms) {
	uint32_t curTicks = msTicks;
	while ((msTicks - curTicks) < ms);
}

__INLINE void delay_us(uint32_t us) {
	uint8_t i;
	uint8_t j;
	for(j=us;j>0;j--) {
		for(i=(SystemCoreClock / 1000000);i>0;i--) {
			asm("nop");
		}
	}
}

