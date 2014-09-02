/*
 * utils.c
 *
 *  Created on: Jan 4, 2012
 *      Author: pavel_pro
 */

#include <hw_config.h>
#include <delay.h>

/*
volatile uint32_t msTicks; // timeTicks counter

void SysTick_Handler(void) {
	msTicks++; // increment timeTicks counter
}

__INLINE void delay_init() {
	if (SysTick_Config(SystemCoreClock / 1000)) // Setup SysTick for 1 msec interrupts
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
*/

inline void delay_init() {
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
}

inline void delay_ms (uint16_t ms) {
	TIM4->PSC = 0xFFFF;
	TIM4->ARR = ms;
	TIM4->EGR = TIM_EGR_UG;
	TIM4->SR &= ~TIM_SR_UIF;
	TIM4->CR1 = TIM_CR1_OPM | TIM_CR1_CEN;
	while(!(TIM4->SR & TIM_SR_UIF)) {
 		__WFI();
 	}
	TIM4->SR &= ~TIM_SR_UIF;
 };

/*
inline void delay_ms (uint16_t ms) {
	TIM7->PSC = 0xFFFF;
	TIM7->ARR = ms - 1;
	TIM7->EGR = TIM_EGR_UG;
	TIM7->SR &= ~TIM_SR_UIF;
	TIM7->CR1 = TIM_CR1_OPM | TIM_CR1_CEN;
	while(!(TIM7->SR & TIM_SR_UIF)) {
		__WFI();
	}
	TIM7->SR &= ~TIM_SR_UIF;
};

inline void delay_us(uint16_t us) {

}
*/


