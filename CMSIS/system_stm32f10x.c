/*
 * gpio_driver.h
 *
 *  Created on: Jun 6, 2012
 *      Author: pavel_pro
 */

#include "stm32f10x.h"

uint32_t SystemCoreClock = 72000000;

// Uncomment the following line if you need to relocate your vector Table in Internal SRAM.
// #define VECT_TAB_SRAM
#define VECT_TAB_OFFSET  0x0 // Vector Table base offset field. This value must be a multiple of 0x200.

__I uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};

static void SetSysClock(void);

void SystemInit (void)
{
  // Reset the RCC clock configuration to the default reset state(for debug purpose)
  // Set HSION bit
  RCC->CR |= (uint32_t)0x00000001;

  // Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits
  RCC->CFGR &= (uint32_t)0xF8FF0000;
  
  // Reset HSEON, CSSON and PLLON bits
  RCC->CR &= (uint32_t)0xFEF6FFFF;

  // Reset HSEBYP bit
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  // Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits
  RCC->CFGR &= (uint32_t)0xFF80FFFF;

  // Disable all interrupts and clear pending bits
  RCC->CIR = 0x009F0000;

  // Configure the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers
  // Configure the Flash Latency cycles and enable prefetch buffer
  SetSysClock();

#ifdef VECT_TAB_SRAM
  SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM. */
#else
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH. */
#endif 
}

static void SetSysClock(void)
{
	__IO uint32_t StartUpCounter = 0, HSEStatus = 0;

	// SYSCLK, HCLK, PCLK2 and PCLK1 configuration
	// Enable HSE
	RCC->CR |= ((uint32_t) RCC_CR_HSEON);

	// Wait till HSE is ready and if Time out is reached exit
	do {
		HSEStatus = RCC->CR & RCC_CR_HSERDY;
		StartUpCounter++;
	} while ((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

	if ((RCC->CR & RCC_CR_HSERDY) != RESET) {
		HSEStatus = (uint32_t) 0x01;
	} else {
		HSEStatus = (uint32_t) 0x00;
	}

	if (HSEStatus == (uint32_t) 0x01) {
		/* Enable Prefetch Buffer */
		FLASH->ACR |= FLASH_ACR_PRFTBE;

		// Flash 2 wait state
		FLASH->ACR &= (uint32_t) ((uint32_t) ~FLASH_ACR_LATENCY);
		FLASH->ACR |= (uint32_t) FLASH_ACR_LATENCY_2;

		// HCLK = SYSCLK
		RCC->CFGR |= (uint32_t) RCC_CFGR_HPRE_DIV1;

		// PCLK2 = HCLK
		RCC->CFGR |= (uint32_t) RCC_CFGR_PPRE2_DIV1;

		// PCLK1 = HCLK
		RCC->CFGR |= (uint32_t) RCC_CFGR_PPRE1_DIV2;

		//  PLL configuration: PLLCLK = HSE * 6 = 72 MHz */
		RCC->CFGR &= (uint32_t) ((uint32_t) ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
		RCC->CFGR |= (uint32_t) (RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL6);

		// Enable PLL
		RCC->CR |= RCC_CR_PLLON;

		// Wait till PLL is ready
		while ((RCC->CR & RCC_CR_PLLRDY) == 0) {
		}

		// Select PLL as system clock source
		RCC->CFGR &= (uint32_t) ((uint32_t) ~(RCC_CFGR_SW));
		RCC->CFGR |= (uint32_t) RCC_CFGR_SW_PLL;

		// Wait till PLL is used as system clock source
		while ((RCC->CFGR & (uint32_t) RCC_CFGR_SWS) != (uint32_t) 0x08) {
		}
	} else {
		// If HSE fails to start-up, the application will have wrong clock configuration. User can add here some code to deal with this error
	}
}

