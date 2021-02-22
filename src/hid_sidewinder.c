// Included as part of usb_desc.c

#ifdef JOYSTICK_INTERFACE

#include <stdint.h>

/*
Usages Class Definitions https://www.usb.org/sites/default/files/hid1_11.pdf
Main items - pg28
Global usages - pg35
Local usages - pg40

Usage Page Tables https://www.usb.org/sites/default/files/hut1_21_0.pdf
Generic Desktop Usage Page (incl. Joystick Axis and Hat Switch) - pg30
Button Usage Page - pg102
*/

static uint8_t joystick_report_desc[] = {
    0x05, 0x01,         // USAGE_PAGE (Generic Desktop)
    0x09, 0x04,         // USAGE (Joystick)
    0xA1, 0x01,         // COLLECTION (Application)

    // buttons (16)
    0x15, 0x00,         //   LOGICAL_MINIMUM (0)
    0x25, 0x01,         //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,         //   REPORT_SIZE (1)
    0x95, 0x10,         //   REPORT_COUNT (16)
    0x05, 0x09,         //   USAGE_PAGE (Button)
    0x19, 0x01,         //   USAGE_MINIMUM (Button 1)
    0x29, 0x10,         //   USAGE_MAXIMUM (Button 16)
    0x81, 0x02,         //   INPUT (Data,Var,Abs)

    // hat switches (2)
    0x15, 0x01,         //   LOGICAL_MINIMUM (1)
    0x25, 0x08,         //   LOGICAL_MAXIMUM (8)
    0x75, 0x04,         //   REPORT_SIZE (4)
    0x95, 0x02,         //   REPORT_COUNT (2)
    0x05, 0x01,         //   USAGE_PAGE (Generic Desktop)
    0x09, 0x39,         //   USAGE (Hat switch)
    0x09, 0x39,         //   USAGE (Hat switch)
    0x81, 0x42,         //   INPUT (Data,Var,Abs,Null)

    // X & Y axis
    0x05, 0x01,         //   USAGE_PAGE (Generic Desktop)
    0x09, 0x01,         //   USAGE (Pointer)
    0xA1, 0x00,         //   COLLECTION (Physical)
    0x15, 0x00,         //     LOGICAL_MINIMUM (0)
    0x26, 0xFF, 0x03,   //     LOGICAL_MAXIMUM (1023)
    0x75, 0x0A,         //     REPORT_SIZE (10)
    0x95, 0x02,         //     REPORT_COUNT (2)
    0x09, 0x30,         //     USAGE (X)
    0x09, 0x31,         //     USAGE (Y)
    0x81, 0x02,         //     INPUT (Data,Var,Abs)
    0xC0,               //   END_COLLECTION C0

    // Rz axis
    0x15, 0x00,         //   LOGICAL_MINIMUM (0)
    0x25, 0x3F,         //   LOGICAL_MAXIMUM (63)
    0x75, 0x06,         //   REPORT_SIZE (6)
    0x95, 0x01,         //   REPORT_COUNT (1)
    0x09, 0x35,         //   USAGE (Rz)
    0x81, 0x02,         //   INPUT (Data,Var,Abs)

    // slider
    0x15, 0x00,         //   LOGICAL_MINIMUM (0)
    0x25, 0x7F,         //   LOGICAL_MAXIMUM (127)
    0x75, 0x07,         //   REPORT_SIZE (7)
    0x95, 0x01,         //   REPORT_COUNT (1)
    0x09, 0x36,         //   USAGE (Slider)
    0x81, 0x02,         //   INPUT (Data,Var,Abs)

    // filler
    0x75, 0x07,         //     Report Size (6)
    0x95, 0x01,         //     Report Count (1)
    0x81, 0x03,         //   Input (Constant)

    0xC0                // END_COLLECTION C0
};
#endif
