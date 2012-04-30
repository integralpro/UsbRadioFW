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

enum _HID_REPORTS
{
   HID_INPUT = 1,
   HID_OUTPUT,
   HID_FEATURE
};

uint32_t ProtocolValue;
uint8_t Request = 0;
uint8_t feature_buf_in[2];
uint8_t feature_buf_out[2];
int x = 0;

uint8_t *CustomHID_SetReport(uint16_t);
uint8_t *CustomHID_GetReport(uint16_t);

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

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
 * Function Name  : CustomHID_init.
 * Description    : Custom HID init routine.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void CustomHID_Init(void) {
	/* Update the serial number string descriptor with the data from the unique
	 ID*/
	//Get_SerialNum();
	pInformation->Current_Configuration = 0;
	/* Connect the device */
	PowerOn();

	/* Perform basic device initialization operations */
	USB_SIL_Init();

	bDeviceState = UNCONNECTED;
}

/*******************************************************************************
 * Function Name  : CustomHID_Reset.
 * Description    : Custom HID reset routine.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
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

	/* Initialize Endpoint 1 */
	SetEPType(ENDP1, EP_INTERRUPT);
	SetEPTxAddr(ENDP1, ENDP1_TXADDR);
	SetEPRxAddr(ENDP1, ENDP1_RXADDR);
	SetEPTxCount(ENDP1, 2);
	SetEPRxCount(ENDP1, 2);
	SetEPRxStatus(ENDP1, EP_RX_VALID);
	SetEPTxStatus(ENDP1, EP_TX_NAK);

	/* Set this device to response on default address */
	SetDeviceAddress(0);

	bDeviceState = ATTACHED;
}
/*******************************************************************************
 * Function Name  : CustomHID_SetConfiguration.
 * Description    : Update the device state to configured and command the ADC
 *                  conversion.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void CustomHID_SetConfiguration(void) {
	if (pInformation->Current_Configuration != 0) {
		/* Device configured */
		bDeviceState = CONFIGURED;

		/* Start ADC Software Conversion */
		//ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	}
}
/*******************************************************************************
 * Function Name  : CustomHID_SetConfiguration.
 * Description    : Update the device state to addressed.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void CustomHID_SetDeviceAddress(void) {
	bDeviceState = ADDRESSED;
}
/*******************************************************************************
 * Function Name  : CustomHID_Status_In.
 * Description    : Joystick status IN routine.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void CustomHID_Status_In(void) {
}

/*******************************************************************************
 * Function Name  : CustomHID_Status_Out
 * Description    : Joystick status OUT routine.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void CustomHID_Status_Out(void) {
}

/*******************************************************************************
 * Function Name  : CustomHID_Data_Setup
 * Description    : Handle the data class specific requests.
 * Input          : Request Nb.
 * Output         : None.
 * Return         : USB_UNSUPPORT or USB_SUCCESS.
 *******************************************************************************/
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

	}

	else if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT)) {
		if(RequestNo == GET_PROTOCOL) {
			CopyRoutine = CustomHID_GetProtocolValue;
		} else if ((RequestNo == SET_REPORT) && (pInformation->USBwValue1 == HID_FEATURE)) {
			CopyRoutine = CustomHID_SetReport;
			Request = SET_REPORT;
		} else if ((RequestNo == GET_REPORT) && (pInformation->USBwValue1 == HID_FEATURE)) {
			CopyRoutine = CustomHID_GetReport;
			Request = GET_REPORT;
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

	if (Request == SET_REPORT) {
		int i;
		//for(i=0;i<sizeof(feature_buf);i++) {
		//	feature_buf[i] = 0xa0;
		//}
		Request = 0;
	}

	return feature_buf_in;
}

uint8_t *CustomHID_GetReport(uint16_t Length) {
	if (Length == 0) {
		pInformation->Ctrl_Info.Usb_wLength = sizeof(feature_buf_out);
		return NULL;
	}

	if (Request == GET_REPORT) {
		int i;
		for (i = 0; i < sizeof(feature_buf_out); i++) {
			feature_buf_out[i] = x++;
		}
		Request = 0;
	}

	return &feature_buf_out[pInformation->Ctrl_Info.Usb_wOffset];
}

/*******************************************************************************
 * Function Name  : CustomHID_NoData_Setup
 * Description    : handle the no data class specific requests
 * Input          : Request Nb.
 * Output         : None.
 * Return         : USB_UNSUPPORT or USB_SUCCESS.
 *******************************************************************************/
RESULT CustomHID_NoData_Setup(uint8_t RequestNo) {
	if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT)) && (RequestNo == SET_PROTOCOL)) {
		return CustomHID_SetProtocol();
	} else {
		return USB_UNSUPPORT;
	}
}

/*******************************************************************************
 * Function Name  : CustomHID_GetDeviceDescriptor.
 * Description    : Gets the device descriptor.
 * Input          : Length
 * Output         : None.
 * Return         : The address of the device descriptor.
 *******************************************************************************/
uint8_t *CustomHID_GetDeviceDescriptor(uint16_t Length) {
	return Standard_GetDescriptorData(Length, &Device_Descriptor);
}

/*******************************************************************************
 * Function Name  : CustomHID_GetConfigDescriptor.
 * Description    : Gets the configuration descriptor.
 * Input          : Length
 * Output         : None.
 * Return         : The address of the configuration descriptor.
 *******************************************************************************/
uint8_t *CustomHID_GetConfigDescriptor(uint16_t Length) {
	return Standard_GetDescriptorData(Length, &Config_Descriptor);
}

/*******************************************************************************
 * Function Name  : CustomHID_GetStringDescriptor
 * Description    : Gets the string descriptors according to the needed index
 * Input          : Length
 * Output         : None.
 * Return         : The address of the string descriptors.
 *******************************************************************************/
uint8_t *CustomHID_GetStringDescriptor(uint16_t Length) {
	uint8_t wValue0 = pInformation->USBwValue0;
	if (wValue0 > 4) {
		return NULL;
	} else {
		return Standard_GetDescriptorData(Length, &String_Descriptor[wValue0]);
	}
}

/*******************************************************************************
 * Function Name  : CustomHID_GetReportDescriptor.
 * Description    : Gets the HID report descriptor.
 * Input          : Length
 * Output         : None.
 * Return         : The address of the configuration descriptor.
 *******************************************************************************/
uint8_t *CustomHID_GetReportDescriptor(uint16_t Length) {
	return Standard_GetDescriptorData(Length, &CustomHID_Report_Descriptor);
}

/*******************************************************************************
 * Function Name  : CustomHID_GetHIDDescriptor.
 * Description    : Gets the HID descriptor.
 * Input          : Length
 * Output         : None.
 * Return         : The address of the configuration descriptor.
 *******************************************************************************/
uint8_t *CustomHID_GetHIDDescriptor(uint16_t Length) {
	return Standard_GetDescriptorData(Length, &CustomHID_Hid_Descriptor);
}

/*******************************************************************************
 * Function Name  : CustomHID_Get_Interface_Setting.
 * Description    : tests the interface and the alternate setting according to the
 *                  supported one.
 * Input          : - Interface : interface number.
 *                  - AlternateSetting : Alternate Setting number.
 * Output         : None.
 * Return         : USB_SUCCESS or USB_UNSUPPORT.
 *******************************************************************************/
RESULT CustomHID_Get_Interface_Setting(uint8_t Interface,
		uint8_t AlternateSetting) {
	if (AlternateSetting > 0) {
		return USB_UNSUPPORT;
	} else if (Interface > 0) {
		return USB_UNSUPPORT;
	}
	return USB_SUCCESS;
}

/*******************************************************************************
 * Function Name  : CustomHID_SetProtocol
 * Description    : Joystick Set Protocol request routine.
 * Input          : None.
 * Output         : None.
 * Return         : USB SUCCESS.
 *******************************************************************************/
RESULT CustomHID_SetProtocol(void) {
	uint8_t wValue0 = pInformation->USBwValue0;
	ProtocolValue = wValue0;
	return USB_SUCCESS;
}

/*******************************************************************************
 * Function Name  : CustomHID_GetProtocolValue
 * Description    : get the protocol value
 * Input          : Length.
 * Output         : None.
 * Return         : address of the protocol value.
 *******************************************************************************/
uint8_t *CustomHID_GetProtocolValue(uint16_t Length) {
	if (Length == 0) {
		pInformation->Ctrl_Info.Usb_wLength = 1;
		return NULL;
	} else {
		return (uint8_t *) (&ProtocolValue);
	}
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/