/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : usb_endp.c
* Author             : MCD Application Team
* Version            : V3.3.0
* Date               : 21-March-2011
* Description        : Endpoint routines
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

#include "usb_lib.h"
#include "usb_istr.h"

#include "leds_driver.h"

extern int16_t sin_table[];

void EP1_IN_Callback(void) {
	uint8_t buffer[2];
	buffer[0] = 1;
	buffer[1] = 2;

	USB_SIL_Write(EP1_IN, buffer, 2);
	SetEPTxValid(ENDP1);
}

#define PACKET_LEN (2 * 48 * sizeof(int16_t))

void EP2_IN_Callback(void) {
	int i,j;
	static uint16_t prepareBuf[48*2];

	static uint8_t x = 0;
	leds_set_mask(x, LED_A | LED_B | LED_C);

	if(x == 0) {
		x = LED_A | LED_B | LED_C;
	} else {
		x = 0;
	}

	for(j=0,i=0;i<48;i++,j+=2) {
		//prepareBuf[j] = prepareBuf[j+1] = -32767 + rand() * (2*32767);
		int16_t x = sin_table[i];
		prepareBuf[j] = prepareBuf[j+1] = sin_table[i];
		//prepareBuf[j+1] = 0;
		//prepareBuf[j] = 0;
	}

	if(_GetENDPOINT(ENDP2) & EP_DTOG_TX){
		UserToPMABufferCopy((uint8_t*)prepareBuf, ENDP2_BUF0Addr, PACKET_LEN);
		_SetEPDblBuf0Count(ENDP2, EP_DBUF_IN, PACKET_LEN);
	} else {
		UserToPMABufferCopy((uint8_t*)prepareBuf, ENDP2_BUF1Addr, PACKET_LEN);
		_SetEPDblBuf1Count(ENDP2, EP_DBUF_IN, PACKET_LEN);
	}

	_ToggleDTOG_TX(ENDP2);
}

void SOF_Callback(void) {
}
