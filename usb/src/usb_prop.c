/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
 * File Name          : usb_prop.c
 * Author             : MCD Application Team
 * Version            : V3.3.0
 * Date               : 21-March-2011
 * Description        : All processings related to Custom HID Demo
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
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_pwr.h"

#include "gpio_driver.h"
#include "leds_driver.h"

enum _HID_REPORTS
{
   HID_INPUT = 1,
   HID_OUTPUT,
   HID_FEATURE
};

uint32_t ProtocolValue;
uint8_t Request = 0;
uint8_t feature_buf_in[0x40];
uint8_t feature_buf_out[0x40];
int x = 0;
uint32_t MUTE_DATA = 0;

uint8_t *CustomHID_SetReport(uint16_t);
uint8_t *CustomHID_GetReport(uint16_t);
uint8_t *Mute_Command(uint16_t Length);

DEVICE Device_Table = {
		3, // Number of endpoints
		1, // Number if configurations
};

DEVICE_PROP Device_Property =
{
		CustomHID_Init,
		CustomHID_Reset,
		CustomHID_Status_In,
		CustomHID_Status_Out,
		CustomHID_Data_Setup,
		CustomHID_NoData_Setup,
		CustomHID_Get_Interface_Setting,
		CustomHID_GetDeviceDescriptor,
		CustomHID_GetConfigDescriptor,
		CustomHID_GetStringDescriptor,
		0,
		0x40 /*MAX PACKET SIZE*/
};

USER_STANDARD_REQUESTS User_Standard_Requests =
{
		CustomHID_GetConfiguration,
		CustomHID_SetConfiguration,
		CustomHID_GetInterface,
		CustomHID_SetInterface,
		CustomHID_GetStatus,
		CustomHID_ClearFeature,
		CustomHID_SetEndPointFeature,
		CustomHID_SetDeviceFeature,
		CustomHID_SetDeviceAddress
};

ONE_DESCRIPTOR Device_Descriptor = {
		(uint8_t*) CustomHID_DeviceDescriptor,
		CUSTOMHID_SIZ_DEVICE_DESC
};

ONE_DESCRIPTOR Config_Descriptor = {
		(uint8_t*) CustomHID_ConfigDescriptor,
		CUSTOMHID_SIZ_CONFIG_DESC
};

ONE_DESCRIPTOR CustomHID_Report_Descriptor = {
		(uint8_t *) CustomHID_ReportDescriptor,
		CUSTOMHID_SIZ_REPORT_DESC
};

ONE_DESCRIPTOR CustomHID_Hid_Descriptor = {
		(uint8_t*) CustomHID_ConfigDescriptor + CUSTOMHID_OFF_HID_DESC,
		CUSTOMHID_SIZ_HID_DESC
};

ONE_DESCRIPTOR String_Descriptor[4] =
{
		{ (uint8_t*) CustomHID_StringLangID, CUSTOMHID_SIZ_STRING_LANGID },
		{ (uint8_t*) CustomHID_StringVendor, CUSTOMHID_SIZ_STRING_VENDOR },
		{ (uint8_t*) CustomHID_StringProduct, CUSTOMHID_SIZ_STRING_PRODUCT },
		{ (uint8_t*) CustomHID_StringSerial, CUSTOMHID_SIZ_STRING_SERIAL }
};

static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len)
{
  uint8_t idx = 0;

  for( idx = 0 ; idx < len ; idx ++)
  {
    if( ((value >> 28)) < 0xA )
    {
      pbuf[ 2* idx] = (value >> 28) + '0';
    }
    else
    {
      pbuf[2* idx] = (value >> 28) + 'A' - 10;
    }

    value = value << 4;

    pbuf[ 2* idx + 1] = 0;
  }
}


void Get_SerialNum(void)
{
  uint32_t Device_Serial0, Device_Serial1, Device_Serial2;

  Device_Serial0 = *(uint32_t*)ID1;
  Device_Serial1 = *(uint32_t*)ID2;
  Device_Serial2 = *(uint32_t*)ID3;

  Device_Serial0 += Device_Serial2;

  if (Device_Serial0 != 0)
  {
    IntToUnicode (Device_Serial0, &CustomHID_StringSerial[2] , 8);
    IntToUnicode (Device_Serial1, &CustomHID_StringSerial[18], 4);
  }
}

void CustomHID_Init(void) {
	/* Update the serial number string descriptor with the data from the unique
	 ID*/
	Get_SerialNum();
	pInformation->Current_Configuration = 0;
	/* Connect the device */
	PowerOn();

	/* Perform basic device initialization operations */
	USB_SIL_Init();

	bDeviceState = UNCONNECTED;
}

void CustomHID_Reset(void) {
	pInformation->Current_Configuration = 0;
	pInformation->Current_Interface = 0;

	SetBTABLE(BTABLE_ADDRESS);

	/* Initialize Endpoint 0 */
	SetEPType(ENDP0, EP_CONTROL);
	SetEPTxStatus(ENDP0, EP_TX_STALL);
	SetEPRxAddr(ENDP0, ENDP0_RXADDR);
	SetEPTxAddr(ENDP0, ENDP0_TXADDR);
	Clear_Status_Out(ENDP0);
	SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
	SetEPRxValid(ENDP0);

	SetEPType(ENDP1, EP_INTERRUPT);
	SetEPTxStatus(ENDP1, EP_TX_STALL);
	SetEPRxStatus(ENDP1, EP_RX_DIS);
	SetEPTxAddr(ENDP1, ENDP1_TXADDR);
	SetEPTxCount(ENDP1, 2);
	SetEPTxValid(ENDP1);

	SetEPType(ENDP2, EP_ISOCHRONOUS);
	SetEPDoubleBuff(ENDP2);
	SetEPDblBuffAddr(ENDP2, ENDP2_BUF0Addr, ENDP2_BUF1Addr);
	SetEPDblBuffCount(ENDP2, EP_DBUF_IN, 0);

	ClearDTOG_RX(ENDP2);
	ClearDTOG_TX(ENDP2);
	//ToggleDTOG_TX(ENDP2);
	SetEPRxStatus(ENDP2, EP_RX_DIS);
	SetEPTxStatus(ENDP2, EP_TX_VALID);

	/* Set this device to response on default address */
	SetDeviceAddress(0);

	bDeviceState = ATTACHED;
}

void CustomHID_SetConfiguration(void) {
	if (pInformation->Current_Configuration != 0) {
		/* Device configured */
		bDeviceState = CONFIGURED;

		/* Start ADC Software Conversion */
		//ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	}
}

void CustomHID_SetDeviceAddress(void) {
	bDeviceState = ADDRESSED;
}

void CustomHID_Status_In(void) {
}

void CustomHID_Status_Out(void) {
}

RESULT CustomHID_Data_Setup(uint8_t RequestNo) {
	uint8_t *(*CopyRoutine)(uint16_t);

	CopyRoutine = NULL;

	if ((RequestNo == GET_DESCRIPTOR)
			&& (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))
			&& (pInformation->USBwIndex0 == 0x02)) {

		if (pInformation->USBwValue1 == REPORT_DESCRIPTOR) {
			CopyRoutine = CustomHID_GetReportDescriptor;
		} else if (pInformation->USBwValue1 == HID_DESCRIPTOR_TYPE) {
			CopyRoutine = CustomHID_GetHIDDescriptor;
		}

	} else if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT)) {
		if(RequestNo == GET_PROTOCOL) {
			CopyRoutine = CustomHID_GetProtocolValue;
		} else if ((RequestNo == SET_REPORT) && (pInformation->USBwValue1 == HID_FEATURE)) {
			CopyRoutine = CustomHID_SetReport;
			Request = SET_REPORT;
		} else if ((RequestNo == GET_REPORT) && (pInformation->USBwValue1 == HID_FEATURE)) {
			CopyRoutine = CustomHID_GetReport;
			Request = GET_REPORT;
		} else if ((RequestNo == GET_CUR) || (RequestNo == SET_CUR)) {
		    CopyRoutine = Mute_Command;
		}
	}

	if (CopyRoutine == NULL) {
		return USB_UNSUPPORT;
	}

	pInformation->Ctrl_Info.CopyData = CopyRoutine;
	pInformation->Ctrl_Info.Usb_wOffset = 0;
	(*CopyRoutine)(0);
	return USB_SUCCESS;
}

uint8_t *CustomHID_SetReport(uint16_t Length) {
	if (Length == 0) {
		pInformation->Ctrl_Info.Usb_wLength = sizeof(feature_buf_in);
		return NULL;
	}

	return feature_buf_in;
}

uint8_t *CustomHID_GetReport(uint16_t Length) {
	if (Length == 0) {
		pInformation->Ctrl_Info.Usb_wLength = sizeof(feature_buf_out);
		return NULL;
	}

	return feature_buf_out;
}

RESULT CustomHID_NoData_Setup(uint8_t RequestNo) {
	if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT)) && (RequestNo == SET_PROTOCOL)) {
		return CustomHID_SetProtocol();
	} else {
		return USB_UNSUPPORT;
	}
}

uint8_t *CustomHID_GetDeviceDescriptor(uint16_t Length) {
	return Standard_GetDescriptorData(Length, &Device_Descriptor);
}

uint8_t *CustomHID_GetConfigDescriptor(uint16_t Length) {
	return Standard_GetDescriptorData(Length, &Config_Descriptor);
}

uint8_t *CustomHID_GetStringDescriptor(uint16_t Length) {
	uint8_t wValue0 = pInformation->USBwValue0;
	if (wValue0 > 4) {
		return NULL;
	} else {
		return Standard_GetDescriptorData(Length, &String_Descriptor[wValue0]);
	}
}

uint8_t *CustomHID_GetReportDescriptor(uint16_t Length) {
	return Standard_GetDescriptorData(Length, &CustomHID_Report_Descriptor);
}

uint8_t *CustomHID_GetHIDDescriptor(uint16_t Length) {
	return Standard_GetDescriptorData(Length, &CustomHID_Hid_Descriptor);
}

RESULT CustomHID_Get_Interface_Setting(uint8_t Interface,
		uint8_t AlternateSetting) {
	switch(Interface) {
	case 0:
	case 2:
		if (AlternateSetting > 0) return USB_UNSUPPORT;
		break;
	case 1:
		if (AlternateSetting > 1) return USB_UNSUPPORT;
		break;
	}

	return USB_SUCCESS;
}

RESULT CustomHID_SetProtocol(void) {
	uint8_t wValue0 = pInformation->USBwValue0;
	ProtocolValue = wValue0;
	return USB_SUCCESS;
}

uint8_t *CustomHID_GetProtocolValue(uint16_t Length) {
	if (Length == 0) {
		pInformation->Ctrl_Info.Usb_wLength = 1;
		return NULL;
	} else {
		return (uint8_t *) (&ProtocolValue);
	}
}

uint8_t *Mute_Command(uint16_t Length) {
	if (Length == 0) {
		pInformation->Ctrl_Info.Usb_wLength = pInformation->USBwLengths.w;
		return NULL;
	} else {
		return((uint8_t*)(&MUTE_DATA));
	}
}
