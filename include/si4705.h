/*
 * si4705_driver.h
 *
 *  Created on: Dec 30, 2011
 *      Author: pavel_pro
 */

#ifndef _SI4705_H_
#define _SI4705_H_

#define SI4705_STATUS_OK 0x80

#define SI4705_MODE_ANALOG 0x05
#define SI4705_MODE_DIGITAL 0xB0

#define SI4705_PROP_REFCLK_FREQ 0x0201
#define SI4705_PROP_REFCLK_PRESCALE 0x0202
#define SI4705_PROP_DIGITAL_OUTPUT_FORMAT 0x0102
#define SI4705_PROP_DIGITAL_OUTPUT_SAMPLE_RATE 0x0104

typedef struct {
	uint8_t		pn;
	union {
		uint16_t	fw_revision_hex;
		uint8_t		fw_revision[2];
	};
	uint16_t	patch_id;
	union {
		uint16_t	cmp_revision_hex;
		uint8_t		cmp_revision[2];
	};
	uint8_t		chip_revision;
} SI4705_REV;

void si4705_init();
uint8_t si4705_powerup(uint8_t mode);
uint8_t si4705_tune(uint16_t freq);
uint8_t si4705_seek(uint8_t up);
uint8_t si4705_getrev(SI4705_REV *rev);
uint8_t si4705_getprop(uint16_t prop_id, uint16_t *prop_val);
uint8_t si4705_setprop(uint16_t prop_id, uint16_t prop_val);

#endif /* _SI4705_H_ */
