/*
 * si4705_driver.h
 *
 *  Created on: Dec 30, 2011
 *      Author: pavel_pro
 */

#ifndef _SI4705_PRIVATE_H_
#define _SI4705_PRIVATE_H_

#include <si4705_conf.h>
#include <si4705.h>

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

void si4705_control_init();
void si4705_send(uint8_t data[8]);
uint8_t si4705_recv();
uint8_t si4705_recv_15(uint8_t output[15]);

#endif /* _SI4705_PRIVATE_H_ */
