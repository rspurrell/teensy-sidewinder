// Included in usb_desc.h

#if defined(_usb_desc_h_) && defined(USB_SERIAL_SIDEWINDER_HID)

#include <stdint.h>

#define VENDOR_ID		0x16C0
#define PRODUCT_ID		0x0487
#define DEVICE_CLASS		0xEF
#define DEVICE_SUBCLASS	0x02
#define DEVICE_PROTOCOL	0x01
#define MANUFACTURER_NAME	{'T','e','e','n','s','y','d','u','i','n','o'}
#define MANUFACTURER_NAME_LEN	11
#define PRODUCT_NAME		{'T','e','e','n','s','y',' ','S','i','d','e','w','i','n','d','e','r'}
#define PRODUCT_NAME_LEN	17
#define EP0_SIZE		64
#define NUM_ENDPOINTS		4
#define NUM_USB_BUFFERS	30
#define NUM_INTERFACE		3
#define CDC_IAD_DESCRIPTOR	1
#define CDC_STATUS_INTERFACE	0
#define CDC_DATA_INTERFACE	1	// Serial
#define CDC_ACM_ENDPOINT	2
#define CDC_RX_ENDPOINT       3
#define CDC_TX_ENDPOINT       4
#define CDC_ACM_SIZE          16
#define CDC_RX_SIZE           64
#define CDC_TX_SIZE           64
#define JOYSTICK_INTERFACE    4	// Joystick
#define JOYSTICK_ENDPOINT     1
#define JOYSTICK_SIZE         8
#define JOYSTICK_INTERVAL     1
#define ENDPOINT1_CONFIG	ENDPOINT_TRANSMIT_ONLY
#define ENDPOINT2_CONFIG	ENDPOINT_TRANSMIT_ONLY
#define ENDPOINT3_CONFIG	ENDPOINT_RECEIVE_ONLY
#define ENDPOINT4_CONFIG	ENDPOINT_TRANSMIT_ONLY

#endif
