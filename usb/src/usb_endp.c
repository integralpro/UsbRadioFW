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

extern uint16_t sin_table[];

void EP1_IN_Callback(void) {
	uint8_t buffer[2];
	buffer[0] = 1;
	buffer[1] = 2;

	USB_SIL_Write(EP1_IN, buffer, 2);
	SetEPTxValid(ENDP1);
}

void EP2_IN_Callback(void) {
	int i;
	const int dataLen = 48 * sizeof(uint16_t);
	static uint16_t prepareBuf0[48];
	static uint16_t prepareBuf1[48];

	static uint16_t *prepareBuf = prepareBuf0;

	static int nextIndex = 0;
	int prevIndex = nextIndex;

	nextIndex = (nextIndex + 1) % 101;

	uint16_t prev = sin_table[prevIndex];
	uint16_t next = sin_table[nextIndex];

	int16_t coeff = (next - prev) / 48;

	for(i=0;i<48;i++) {
		prepareBuf[i] = coeff * i + prev;
	}

	if(_GetENDPOINT(ENDP3) & EP_DTOG_RX){
		UserToPMABufferCopy((uint8_t *)prepareBuf, ENDP2_BUF0Addr, dataLen);
		_SetEPDblBuf0Count(ENDP3, EP_DBUF_IN, dataLen);
	} else {
		UserToPMABufferCopy((uint8_t *)prepareBuf, ENDP2_BUF1Addr, dataLen);
		_SetEPDblBuf1Count(ENDP3, EP_DBUF_IN, dataLen);
	}

	_ToggleDTOG_RX(ENDP3);

	prepareBuf = (prepareBuf == prepareBuf0) ? prepareBuf1 : prepareBuf0;
}

void RESET_Callback(void) {
	static int state = 0;
	if(state == 0) {
		leds_set_mask(LED_C, LED_C);
		state = 1;
	} else {
		leds_set_mask(0, LED_C);
		state = 0;
	}
}

void SOF_Callback(void) {
}
