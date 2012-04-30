/*
 * si4705_driver.h
 *
 *  Created on: Dec 30, 2011
 *      Author: pavel_pro
 */

#ifndef _SI4705_CONF_H_
#define _SI4705_CONF_H_

#include <stm32f10x.h>

//#define CONTROL_HARDWARE_SPI

#define RF_NRST 10
#define RF_NSEN 4
#define RF_GPO1 0
#define RF_GPO2 1
#define RF_SDI 	6
#define RF_SDO 	7
#define RF_SCLK 5
#define RF_RCLK 2

#define RF_NRST_VAL (1 << RF_NRST)
#define RF_NSEN_VAL (1 << RF_NSEN)
#define RF_GPO1_VAL (1 << RF_GPO1)
#define RF_GPO2_VAL (1 << RF_GPO2)
#define RF_SDI_VAL 	(1 << RF_SDI)
#define RF_SDO_VAL 	(1 << RF_SDO)
#define RF_SCLK_VAL (1 << RF_SCLK)
#define RF_RCLK_VAL (1 << RF_RCLK)

#endif /* _SI4705_CONF_H_ */