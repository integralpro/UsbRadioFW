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

#define SI4705_OFALL_RE (0x00)
#define SI4705_OFALL_FE (0x80)
#define SI4705_OMODE_I2S (0x00)
#define SI4705_OMODE_LJ (0x30)
#define SI4705_OMODE_MSBS (0x40)
#define SI4705_OMODE_MSBF (0x60)
#define SI4705_OMONO_BLEND (0x00)
#define SI4705_OMONO_MONO (0x04)
#define SI4705_OSIZE_8 (0x11)
#define SI4705_OSIZE_16 (0x00)
#define SI4705_OSIZE_20 (0x01)
#define SI4705_OSIZE_24 (0x10)

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
