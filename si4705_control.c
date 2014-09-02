#include <hw_config.h>
#include <si4705_private.h>

void si4705_control_init() {
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
}

#ifdef CONTROL_HARDWARE_SPI

void si4705_send(uint8_t data[8]) {
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

uint8_t si4705_recv() {
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

uint8_t si4705_recv_15(uint8_t output[15]) {
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
	uint8_t i;
	uint8_t byte;

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

void si4705_send(uint8_t data[8]) {
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

uint8_t si4705_recv() {
	uint8_t ret;
	gpio_reset(GPIOA, RF_NSEN_VAL);

	si4705_send_raw(0x80);

	ret = si4705_recv_raw();

	gpio_set(GPIOA, RF_NSEN_VAL);
	return ret;
}

uint8_t si4705_recv_15(uint8_t output[15]) {
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
