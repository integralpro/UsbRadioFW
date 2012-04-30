/*
 * si4705_driver.h
 *
 *  Created on: Dec 30, 2011
 *      Author: pavel_pro
 */

#ifndef _SI4705_H_
#define _SI4705_H_

#define SI4705_STATUS_OK 0x80

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
uint8_t si4705_powerup();
uint8_t si4705_tune(uint16_t freq);
uint8_t si4705_seek(uint8_t up);
uint8_t si4705_getrev(SI4705_REV *rev);
uint8_t si4705_getprop(uint16_t prop_id, uint16_t *prop_val);

#endif /* _SI4705_H_ */
