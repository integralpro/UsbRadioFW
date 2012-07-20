#include <string.h>

#include <si4705_conf.h>
#include <si4705.h>

#include <leds_driver.h>
#include <delay.h>
#include <gpio_driver.h>

#define RF_NRST_VAL (1 << RF_NRST)
#define RF_NSEN_VAL (1 << RF_NSEN)
#define RF_GPO1_VAL (1 << RF_GPO1)
#define RF_GPO2_VAL (1 << RF_GPO2)
#define RF_SDI_VAL 	(1 << RF_SDI)
#define RF_SDO_VAL 	(1 << RF_SDO)
#define RF_SCLK_VAL (1 << RF_SCLK)
#define RF_RCLK_VAL (1 << RF_RCLK)

void si4705_init() {
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	// Init SPI for RF module
#ifdef CONTROL_HARDWARE_SPI
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;			// Enable clock on SPI

	gpio_mode(GPIOA, RF_SCLK, GPIO_MODE_OUT_10 | GPIO_CFGO_A_PUSH_PULL);
	gpio_mode(GPIOA, RF_SDO, GPIO_MODE_OUT_10 | GPIO_CFGO_A_PUSH_PULL);
	SPI1->CR1 |= SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0 | SPI_CR1_MSTR | SPI_CR1_BIDIMODE | SPI_CR1_SSM | SPI_CR1_SSI;
#else
	gpio_mode(GPIOA, RF_SCLK, GPIO_MODE_OUT_10 | GPIO_CFGO_PUSH_PULL);
	gpio_mode(GPIOA, RF_SDO, GPIO_MODE_OUT_10 | GPIO_CFGO_PUSH_PULL);
#endif // CONTROL_HARDWARE_SPI

	gpio_mode(GPIOA, RF_NSEN, GPIO_MODE_OUT_10 | GPIO_CFGO_PUSH_PULL);
	gpio_set(GPIOA, RF_NSEN_VAL);

	// Enable Timers
	gpio_mode(GPIOA, RF_RCLK, GPIO_MODE_OUT_10 | GPIO_CFGO_A_PUSH_PULL);
	TIM2->CR1 = TIM_CR1_ARPE;
	TIM2->PSC = 8;
	TIM2->ARR = 244;
	TIM2->EGR = TIM_EGR_UG;
	TIM2->CR2 = 0;

	//TIM2->CCER &= ~(TIM_CCER_CC2E | TIM_CCER_CC2P);
	TIM2->CCR3 = 122;
	TIM2->CCMR2 = TIM2->CCMR2 & 0xFF00;
	TIM2->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1         // PWM 1
	        	|  TIM_CCMR2_OC3PE;                            // enable preload;

	TIM2->CCER |= TIM_CCER_CC3E;
	TIM2->DIER |= TIM_DIER_UIE;
}

#ifdef CONTROL_HARDWARE_SPI

static void si4705_send(uint8_t data[8]) {
	uint8_t i;
	gpio_reset(GPIOA, RF_NSEN_VAL);

	SPI1->CR1 |= SPI_CR1_BIDIOE | SPI_CR1_SPE;
	SPI1->DR = 0x48;

	while(!(SPI1->SR & SPI_SR_TXE));

	for(i = 0; i < 8; i++) {
		SPI1->DR = data[i];
		while(!(SPI1->SR & SPI_SR_TXE));
	}
	while(SPI1->SR & SPI_SR_BSY);
	SPI1->CR1 &= ~SPI_CR1_SPE;
	gpio_set(GPIOA, RF_NSEN_VAL);
}

static uint8_t si4705_recv() {
	uint8_t ret;
	gpio_reset(GPIOA, RF_NSEN_VAL);

	SPI1->CR1 |= SPI_CR1_BIDIOE | SPI_CR1_SPE;
	SPI1->DR = 0x80;

	while(!(SPI1->SR & SPI_SR_TXE));
	while(SPI1->SR & SPI_SR_BSY);
	//SPI1->CR1 &= ~SPI_CR1_SPE;

	SPI1->CR1 &= ~SPI_CR1_BIDIOE;
	//SPI1->CR1 |= SPI_CR1_SPE;

	while(!(SPI1->SR & SPI_SR_RXNE));
	ret = SPI1->DR;

	SPI1->CR1 &= ~SPI_CR1_SPE;
	SPI1->CR1 |= SPI_CR1_BIDIOE;
	gpio_set(GPIOA, RF_NSEN_VAL);
	return ret;
}

static uint8_t si4705_recv_15(uint8_t output[15]) {
	uint8_t i;
	uint8_t ret;
	gpio_reset(GPIOA, RF_NSEN_VAL);

	SPI1->CR1 |= SPI_CR1_BIDIOE | SPI_CR1_SPE;
	SPI1->DR = 0xC0;

	while(!(SPI1->SR & SPI_SR_TXE));
	while(SPI1->SR & SPI_SR_BSY);

	SPI1->CR1 &= ~SPI_CR1_BIDIOE;

	while(!(SPI1->SR & SPI_SR_RXNE));
	ret = SPI1->DR;

	for(i=0;i<15;i++) {
		while(!(SPI1->SR & SPI_SR_RXNE));
		output[i] = SPI1->DR;
	}

	SPI1->CR1 &= ~SPI_CR1_SPE;
	gpio_set(GPIOA, RF_NSEN_VAL);
	return ret;
}

#else

static void si4705_send_raw(uint8_t byte) {
	uint8_t i;

	for(i = 0; i < 8; i++) {
		if(byte & 0x80) {
			gpio_set(GPIOA, RF_SDO_VAL);
		} else {
			gpio_reset(GPIOA, RF_SDO_VAL);
		}
		delay_ms(1);
		gpio_set(GPIOA, RF_SCLK_VAL);
		delay_ms(1);
		gpio_reset(GPIOA, RF_SCLK_VAL);

		byte <<= 1;
	}
	delay_ms(1);
}

static uint8_t si4705_recv_raw() {
	uint8_t i, byte;

	byte = 0;
	gpio_mode(GPIOA, RF_SDO, GPIO_MODE_INPUT | GPIO_CFGI_FLOAT);
	delay_ms(1);

	for(i = 0; i < 8; i++) {
		gpio_set(GPIOA, RF_SCLK_VAL);
		delay_ms(1);
		uint8_t val = gpio_get(GPIOA) & RF_SDO_VAL;
		gpio_reset(GPIOA, RF_SCLK_VAL);
		delay_ms(1);

		if(val) {
			byte |= (0x80 >> i);
		}
	}
	gpio_mode(GPIOA, RF_SDO, GPIO_MODE_OUT_50 | GPIO_CFGO_PUSH_PULL);
	delay_ms(1);
	return byte;
}

static void si4705_send(uint8_t data[8]) {
	uint8_t i;
	gpio_reset(GPIOA, RF_NSEN_VAL);

	delay_ms(1);
	si4705_send_raw(0x48);

	for(i = 0; i < 8; i++) {
		delay_ms(1);
		si4705_send_raw(data[i]);
	}

	delay_ms(1);
	gpio_set(GPIOA, RF_NSEN_VAL);
}

static uint8_t si4705_recv() {
	uint8_t ret;
	gpio_reset(GPIOA, RF_NSEN_VAL);

	si4705_send_raw(0x80);

	ret = si4705_recv_raw();

	gpio_set(GPIOA, RF_NSEN_VAL);
	return ret;
}

static uint8_t si4705_recv_15(uint8_t output[15]) {
	uint8_t i;
	uint8_t ret;
	gpio_reset(GPIOA, RF_NSEN_VAL);

	si4705_send_raw(0xC0);

	ret = si4705_recv_raw();

	for(i=0;i<15;i++) {
		output[i] = si4705_recv_raw();
	}

	gpio_set(GPIOA, RF_NSEN_VAL);
	return ret;
}

#endif // CONTROL_HARDWARE_SPI

static void si4705_rclk(uint8_t enable) {
	TIM2->CR1 |= TIM_CR1_CEN;
}

uint8_t si4705_powerup(uint8_t mode) {
	uint8_t count;
	gpio_mode(GPIOA, RF_GPO1, GPIO_MODE_OUT_10 | GPIO_CFGO_PUSH_PULL);
	gpio_mode(GPIOA, RF_GPO2, GPIO_MODE_OUT_10 | GPIO_CFGO_PUSH_PULL);
	gpio_mode(GPIOA, RF_NRST, GPIO_MODE_OUT_10 | GPIO_CFGO_PUSH_PULL);

	gpio_set(GPIOA, RF_GPO1_VAL | RF_GPO2_VAL);
	delay_ms(1);
	gpio_set(GPIOA, RF_NRST_VAL);
	delay_ms(100);

	gpio_mode(GPIOA, RF_GPO1, GPIO_MODE_INPUT | GPIO_CFGI_FLOAT);
	gpio_mode(GPIOA, RF_GPO2, GPIO_MODE_INPUT | GPIO_CFGI_FLOAT);

	uint8_t powerupCmd[8];
	memset(powerupCmd, 0, sizeof(powerupCmd));
	powerupCmd[0] = 0x01;
	powerupCmd[1] = 0x00;
	powerupCmd[2] = mode;
	si4705_send(powerupCmd);

	count = 11;
	uint8_t r;
	do {
		count--;
		r = si4705_recv();
	} while((!(r & SI4705_STATUS_OK)) && count != 0);

	if(!(r & SI4705_STATUS_OK)) {
		return r;
	}

	si4705_rclk(1);

	return r;
}

uint8_t si4705_getrev(SI4705_REV *rev) {
	uint8_t ret;
	uint8_t cmd[15];

	memset(cmd, 0, sizeof(cmd));
	cmd[0] = 0x10;
	si4705_send(cmd);

	memset(cmd, 0, sizeof(cmd));
	ret = si4705_recv_15(cmd);

	rev->pn = cmd[0];
	rev->fw_revision[0] = cmd[1];
	rev->fw_revision[1] = cmd[2];
	rev->patch_id = (cmd[3] << 8) | cmd[4];
	rev->cmp_revision[0] = cmd[5];
	rev->cmp_revision[1] = cmd[6];
	rev->chip_revision = cmd[7];

	return ret;
}

uint8_t si4705_tune(uint16_t freq) {
	uint8_t cmd[8];

	memset(cmd, 0, sizeof(cmd));
	cmd[0] = 0x20;
	cmd[1] = 0x00;
	cmd[2] = (uint8_t)(freq >> 8);
	cmd[3] = (uint8_t)(freq >> 0);
	cmd[4] = 0x00;
	si4705_send(cmd);
	return si4705_recv();
}

uint8_t si4705_seek(uint8_t up) {
	uint8_t cmd[8];

	memset(cmd, 0, sizeof(cmd));
	cmd[0] = 0x21;
	cmd[1] = 0x04;

	if(up != 0) {
		cmd[1] |= 0x08;
	}

	si4705_send(cmd);
	return si4705_recv();
}

uint8_t si4705_getprop(uint16_t prop_id, uint16_t *prop_val) {
	uint8_t ret;
	uint8_t cmd[8];
	uint8_t cmdin[15];

	memset(cmd, 0, sizeof(cmd));
	cmd[0] = 0x13;
	cmd[2] = (prop_id >> 8) & 0xFFFF;
	cmd[3] = (prop_id >> 0) & 0xFFFF;
	si4705_send(cmd);

	memset(cmdin, 0, sizeof(cmdin));
	ret = si4705_recv_15(cmdin);

	uint16_t val = 0;
	val |= cmdin[1];
	val <<= 8;
	val |= cmdin[2];

	*prop_val = val;

	return ret;
}

uint8_t si4705_setprop(uint16_t prop_id, uint16_t prop_val) {
	uint8_t cmd[8];

	memset(cmd, 0, sizeof(cmd));
	cmd[0] = 0x12;
	cmd[2] = (prop_id >> 8) & 0xFFFF;
	cmd[3] = (prop_id >> 0) & 0xFFFF;
	cmd[4] = (prop_val >> 8) & 0xFFFF;
	cmd[5] = (prop_val >> 0) & 0xFFFF;
	si4705_send(cmd);

	return si4705_recv();
}
