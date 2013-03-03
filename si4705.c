#include <common.h>
#include <string.h>
#include <si4705_private.h>

void si4705_init() {
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	si4705_control_init();

	gpio_mode(GPIOA, RF_NSEN, GPIO_MODE_OUT_10 | GPIO_CFGO_PUSH_PULL);
	gpio_set(GPIOA, RF_NSEN_VAL);

	// Prepare Timer for the main clock
	gpio_mode(GPIOA, RF_RCLK, GPIO_MODE_OUT_10 | GPIO_CFGO_A_PUSH_PULL);
	TIM2->CR1 = TIM_CR1_ARPE;								   // Enable preaload for ARR why??
	TIM2->PSC = 9 - 1; // To get 8Mhz (72Mhz/9)
	TIM2->ARR = 244;
	TIM2->EGR = TIM_EGR_UG;

	TIM2->CCR3 = 122;
	TIM2->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1         // PWM 1
	        	|  TIM_CCMR2_OC3PE;                            // Enable preload; why??
	TIM2->CCER |= TIM_CCER_CC3E;							   // Enable compare channel 3
}

#define si4705_rclk_en() (TIM2->CR1 |= TIM_CR1_CEN)

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

	si4705_rclk_en();

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
