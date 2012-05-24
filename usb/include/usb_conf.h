#ifndef __USB_CONF_H
#define __USB_CONF_H

#define BTABLE_ADDRESS      (0x00)

#define ENDP0_RXADDR        (0x18)
#define ENDP0_TXADDR        (0x58)

//#define ENDP1_RXADDR        (0x18)
#define ENDP1_TXADDR        (0x5A)

#define ENDP2_BUF0Addr      (0x90 + 0x40 + 8)
#define ENDP2_BUF1Addr      (0x90 + 0x40 + 8)//(0xC0+0x40)

#define	SOF_CALLBACK
#define RESET_CALLBACK

#define IMR_MSK (CNTR_CTRM  | CNTR_WKUPM | CNTR_SUSPM | CNTR_ERRM  | CNTR_SOFM \
                 | CNTR_ESOFM | CNTR_RESETM )

/* CTR service routines */
/* associated to defined endpoints */
//#define  EP1_IN_Callback   NOP_Process
//#define  EP2_IN_Callback   NOP_Process
#define  EP3_IN_Callback   NOP_Process
#define  EP4_IN_Callback   NOP_Process
#define  EP5_IN_Callback   NOP_Process
#define  EP6_IN_Callback   NOP_Process
#define  EP7_IN_Callback   NOP_Process

#define  EP1_OUT_Callback   NOP_Process
#define  EP2_OUT_Callback   NOP_Process
#define  EP3_OUT_Callback   NOP_Process
#define  EP4_OUT_Callback   NOP_Process
#define  EP5_OUT_Callback   NOP_Process
#define  EP6_OUT_Callback   NOP_Process
#define  EP7_OUT_Callback   NOP_Process

#endif /*__USB_CONF_H*/

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

