/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : usb_desc.c
* Author             : MCD Application Team
* Version            : V3.3.0
* Date               : 21-March-2011
* Description        : Descriptors for Custom HID Demo
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* USB Standard Device Descriptor */
const uint8_t CustomHID_DeviceDescriptor[CUSTOMHID_SIZ_DEVICE_DESC] =
{
	CUSTOMHID_SIZ_DEVICE_DESC,  /*bLength */
    USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType*/
    0x00, 0x02,                 /*bcdUSB */
    0x00,                       /*bDeviceClass*/
    0x00,                       /*bDeviceSubClass*/
    0x00,                       /*bDeviceProtocol*/
    0x40,                       /*bMaxPacketSize40*/
    0x83, 0x04,                 /*idVendor (0x0483)*/
    0x50, 0x57,                 /*idProduct = 0x5750*/
    0x00, 0x02,                 /*bcdDevice rel. 2.00*/
    1,                          /*Index of string descriptor describing manufacturer */
    2,                          /*Index of string descriptor describing product*/
    3,                          /*Index of string descriptor describing the device serial number */
    0x01                        /*bNumConfigurations*/
} ; /* CustomHID_DeviceDescriptor */

/* USB Configuration Descriptor */
/* All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
const uint8_t CustomHID_ConfigDescriptor[CUSTOMHID_SIZ_CONFIG_DESC] =
{
    0x09,								// bLength: Configuration Descriptor size
    USB_CONFIGURATION_DESCRIPTOR_TYPE, 	// bDescriptorType: Configuration
    CUSTOMHID_SIZ_CONFIG_DESC, 0x00, 	// Total length
    0x03,         						// bNumInterfaces: 3 interface
    0x01,         						// bConfigurationValue: Configuration value
    0x00,         						// iConfiguration: Index of string descriptor describing the configuration
    0x80,         						// bmAttributes: Bus powered
    0x32,         						// MaxPower 100 mA: this current is used for detecting Vbus

		// Audio Standard interface descriptor
		0x09,								// bLength
		USB_INTERFACE_DESCRIPTOR_TYPE,		// bDescriptorType
		0x00,								// bInterfaceNumber
		0x00,                               // bAlternateSetting
		0x00,                               // bNumEndpoints
		USB_DEVICE_CLASS_AUDIO,             // bInterfaceClass
		AUDIO_SUBCLASS_AUDIOCONTROL,        // bInterfaceSubClass
		AUDIO_PROTOCOL_UNDEFINED,           // bInterfaceProtocol
		0x00,                               // iInterface

			// Audio Class-specific AC Interface Descriptor
			0x09,								// bLength
			AUDIO_INTERFACE_DESCRIPTOR_TYPE,    // bDescriptorType
			AUDIO_CONTROL_HEADER,               // bDescriptorSubtype
			0x00, 0x01,                         // bcdADC 1.00
			0x2B, 0x00,                         // wTotalLength = ?
			0x01,                               // bInCollection
			0x01,                               // baInterfaceNr

			// Audio Input Terminal Descriptor
			0x0C,       // bLength
			AUDIO_INTERFACE_DESCRIPTOR_TYPE,    // bDescriptorType
			AUDIO_CONTROL_INPUT_TERMINAL,       // bDescriptorSubtype
			0x01,                               // bTerminalID
			0x10, 0x07,                         // wTerminalType Radio RF
			0x00,                               // bAssocTerminal
			0x02,                               // bNrChannels
			0x03, 0x00,                         // wChannelConfig
			0x00,                               // iChannelNames
			0x00,                               // iTerminal

			// Audio Feature Unit Descriptor
			0x0D,								// bLength (13)
			AUDIO_INTERFACE_DESCRIPTOR_TYPE,	// bDescriptorType
			AUDIO_CONTROL_FEATURE_UNIT,			// bDescriptorSubtype
			0x02,								// bUnitID
			0x01,								// bSourceID
			0x02,								// bControlSize
			0x01, 0x00,							// Master Controls
			0x00, 0x00,							// Channel 0 controls
			0x00, 0x00,							// Channel 1 controls
			0x00,								// iFeature

			// Audio Output Terminal Descriptor
			0x09,								// bLength
			AUDIO_INTERFACE_DESCRIPTOR_TYPE,	// bDescriptorType
			AUDIO_CONTROL_OUTPUT_TERMINAL,		// bDescriptorSubtype
			0x03,								// bTerminalID
			0x01, 0x01,							// wTerminalType (USB Streaming)
			0x00,								// bAssocTerminal,
			0x02,								// bSourceID
			0x00,								// iTerminal

		// Audio Interface Descriptor
		0x09,								// bLength
		USB_INTERFACE_DESCRIPTOR_TYPE,		// bDescriptorType
		0x01,								// bInterfaceNumber
		0x00,								// bAlternateSetting
		0x00,								// bNumEndpoints
		USB_DEVICE_CLASS_AUDIO,				// bInterfaceClass (Audio)
		AUDIO_SUBCLASS_AUDIOSTREAMING,		// bInterfaceSubClass (Audio Streaming)
		AUDIO_PROTOCOL_UNDEFINED,			// bInterfaceProtocol
		0x00,								// iInterface

		// Alternate Audio Interface Descriptor
		0x09,								// bLength
		USB_INTERFACE_DESCRIPTOR_TYPE,		// bDescriptorType
		0x01,								// bInterfaceNumber
		0x01,								// bAlternateSetting
		0x01,								// bNumEndpoints
		USB_DEVICE_CLASS_AUDIO,				// bInterfaceClass (Audio)
		AUDIO_SUBCLASS_AUDIOSTREAMING,		// bInterfaceSubClass (Audio Streaming)
		AUDIO_PROTOCOL_UNDEFINED,			// bInterfaceProtocol
		0x00,								// iInterface

			// Audio Stream Class Descriptor
			0x07,								// bLength
			AUDIO_INTERFACE_DESCRIPTOR_TYPE,	// bDescriptorType
			AUDIO_STREAMING_GENERAL,			// bDescriptorSubtype
			0x03,								// bTerminalLink (terminal 3)
			0x00,								// bDelay
			0x01, 0x00,							// wFormatTag (PCM format)

			// Audio Format Descriptor
			0x0B,								// bLength
			AUDIO_INTERFACE_DESCRIPTOR_TYPE,	// bDescriptorType
			AUDIO_STREAMING_FORMAT_TYPE,		// bDescriptorSubtype
			AUDIO_FORMAT_TYPE_I,				// bFormatType (TYPE_I)
			0x02,								// bNrChannels (2)
			0x02,								// bSubFrameSize (2)
			// The next field should be 10, but 16 works with more standard software
			0x10,								// bBitResolution
			0x01,								// bSamFreqType (1 sampling freq.)
			0x80, 0xBB, 0x00,					// 48,000 Hz

			// Isochronous Endpoint Descriptor
			0x09,								// bLength
			USB_ENDPOINT_DESCRIPTOR_TYPE,		// bDescriptorType
			EP3_IN,								// bEndpointAddress (EP3 IN)
			0x05,								// bmAttributes (asynchronous)
			0x00, 0x02,							// wMaxPacketSize (512)
			0x01,								// bInterval (1 millisecond)
			0x00,								// bRefresh
			0x00,								// bSyncAddress (no sync)

			// Isochronous Endpoint Audio Class Descriptor
			0x07,								// bLength
			AUDIO_ENDPOINT_DESCRIPTOR_TYPE,		// bDescriptorType
			AUDIO_STREAMING_GENERAL,			// bDescriptorSubtype
			0x00,								// bmAttributes
			0x02,								// bLockDelayUnits (PCM Samples)
			0x00, 0x00,							// wLockDelay

    // Descriptor of Custom HID interface
    0x09,										// bLength: Interface Descriptor size
    USB_INTERFACE_DESCRIPTOR_TYPE,				// bDescriptorType: Interface descriptor type
    0x02,										// bInterfaceNumber: Number of Interface
    0x00,										// bAlternateSetting: Alternate setting
    0x01,										// bNumEndpoints
    0x03,										// bInterfaceClass: HID
    0x00,										// bInterfaceSubClass : 1=BOOT, 0=no boot
    0x00,										// nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse
    0x00,										// iInterface: Index of string descriptor
			// Descriptor of Custom HID
			0x09,								// bLength: HID Descriptor size
			HID_DESCRIPTOR_TYPE,				// bDescriptorType: HID
			0x11, 0x01,							// bcdHID: HID Class Spec release number
			0x00,								// bCountryCode: Hardware target country
			0x01,								// bNumDescriptors: Number of HID class descriptors to follow
			0x22,								// bDescriptorType (report desc.)
			CUSTOMHID_SIZ_REPORT_DESC, 0x00,	// wItemLength: Total length of Report descriptor

			// IN
			0x07,								// bLength: Endpoint Descriptor size
			USB_ENDPOINT_DESCRIPTOR_TYPE,		// bDescriptorType:
			EP1_IN,								// bEndpointAddress: Endpoint Address (EP1 IN)
			0x03,								// bmAttributes: Interrupt endpoint
			0x02, 0x00,							// wMaxPacketSize: 2 Bytes max
			0x20,								// bInterval: Polling Interval (32 ms)
};

const uint8_t CustomHID_ReportDescriptor[CUSTOMHID_SIZ_REPORT_DESC] =
{
		0x06, 0x00, 0xff,              // USAGE_PAGE (Vendor Defined Page 1)
		0x09, 0x01,                    // USAGE (Vendor Usage 1)
		0xa1, 0x01,                    // COLLECTION (Application)

		0x85, 0x01,                    //   REPORT_ID (1)
		0x09, 0x01,                    //   USAGE (Vendor Usage 1)
		0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
		0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
		0x75, 0x08,                    //   REPORT_SIZE (8)
		0x95, 0x01,                    //   REPORT_COUNT (1)
		0xb1, 0x82,                    //   FEATURE (Data,Var,Abs,Vol)

		0x85, 0x02,                    //   REPORT_ID (2)
		0x09, 0x02,                    //   USAGE (Vendor Usage 1)
		0x15, 0x00,                    //   LOGICAL_MINIMUM (-1)
		0x25, 0x02,                    //   LOGICAL_MAXIMUM (1)
		0x75, 0x02,                    //   REPORT_SIZE (1)
		0x95, 0x03,                    //   REPORT_COUNT (3)
		0xb1, 0x02,                    //   FEATURE (Data,Var,Abs)
		0x75, 0x02,                    //   REPORT_SIZE (1)
		0x95, 0x01,                    //   REPORT_COUNT (3)
		0xb1, 0x01,                    //   FEATURE (Data,Var,Abs)

		0xc0                           // END_COLLECTION
}; /* CustomHID_ReportDescriptor */

/* USB String Descriptors (optional) */
const uint8_t CustomHID_StringLangID[] =
{
	CUSTOMHID_SIZ_STRING_LANGID, // Length
	USB_STRING_DESCRIPTOR_TYPE,
	0x09, 0x04
} ; /* LangID = 0x0409: U.S. English */

const uint8_t CustomHID_StringVendor[] =
{
	CUSTOMHID_SIZ_STRING_VENDOR, // Size
    USB_STRING_DESCRIPTOR_TYPE,  /* bDescriptorType*/
    /* Manufacturer: "STMicroelectronics" */
    'S', 0,
    'T', 0,
    'M', 0,
    'i', 0,
    'c', 0,
    'r', 0,
    'o', 0,
    'e', 0,
    'l', 0,
    'e', 0,
    'c', 0,
    't', 0,
    'r', 0,
    'o', 0,
    'n', 0,
    'i', 0,
    'c', 0,
    's', 0
};

const uint8_t CustomHID_StringProduct[] =
{
	CUSTOMHID_SIZ_STRING_PRODUCT, // Size
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'U', 0x00,
    'S', 0x00,
    'B', 0x00,
    ' ', 0x00,
    'R', 0x00,
    'a', 0x00,
    'd', 0x00,
    'i', 0x00,
    'o', 0x00,
    ' ', 0x00,
    'v', 0x00,
    '0', 0x00,
    '.', 0x00,
    '1', 0x00
};

uint8_t CustomHID_StringSerial[] =
{
	CUSTOMHID_SIZ_STRING_SERIAL, // Size
	USB_STRING_DESCRIPTOR_TYPE,     /* bDescriptorType */
	'S', 0,
	'T', 0,
	'M', 0,
	'3', 0,
	'2', 0,
	'1', 0,
	'0', 0
};

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

