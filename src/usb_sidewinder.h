#ifndef usb_sidewinder_h_
#define usb_sidewinder_h_

#define USBjoystick_h_ // disable native teensy Joystick.h

#include "usb_desc.h"

#if defined(JOYSTICK_INTERFACE)

#include <inttypes.h>

// C language implementation
#ifdef __cplusplus
extern "C" {
#endif
int usb_joystick_send(void);
extern uint32_t usb_joystick_data[(JOYSTICK_SIZE+3)/4];
#ifdef __cplusplus
}
#endif

// C++ interface
#ifdef __cplusplus
class usb_joystick_class
{
public:
/*
SSSSSSS ZZZZZZ YYYYYYYYYY XXXXXXXXXX H2H2 H1H1 BBBBBBBBBBBBBBBB
6543210 543210 9876543210 9876543210 3210 3210 5432109876543210
---- ---S SSSS SSZZ ZZZZ YYYY YYYY YYXX  XXXX XXXX H2H2 H1H1 BBBB BBBB BBBB BBBB
0000 0006 5432 1054 3210 9876 5432 1098  7654 3210 3210 3210 5432 1098 7654 3210
*/

    void begin(void) { }
    void end(void) { }
    
    void button(uint8_t button, bool val)
    {
        if (button >= 16) return;
        if (val) usb_joystick_data[0] |= (1 << button);
        else usb_joystick_data[0] &= ~(1 << button);
        if (!manual_mode) usb_joystick_send();
    }
    
    void buttons(uint8_t val, bool shift)
    {
        usb_joystick_data[0] = (usb_joystick_data[0] & 0xFFFF0000) | (shift ? val << 8 : val);
        if (!manual_mode) usb_joystick_send();
    }

    void X(unsigned int val)
    {
        if (val > 1023) val = 1023;
        usb_joystick_data[0] = (usb_joystick_data[0] & 0x00FFFFFF) | (val << 24);
        usb_joystick_data[1] = (usb_joystick_data[1] & 0xFFFFFC00) | (val >> 8);
        if (!manual_mode) usb_joystick_send();
    }

    void Y(unsigned int val)
    {
        if (val > 1023) val = 1023;
        usb_joystick_data[1] = (usb_joystick_data[1] & 0xFFFFF003) | (val << 2);
        if (!manual_mode) usb_joystick_send();
    }

    void position(unsigned int x, unsigned int y)
    {
        if (x > 1023) x = 1023;
        if (y > 1023) y = 1023;
        usb_joystick_data[0] = (usb_joystick_data[0] & 0x00FFFFFF) | (x << 24);
        usb_joystick_data[1] = (usb_joystick_data[1] & 0xFFFFF000) | (x >> 8) | (y << 2);
        if (!manual_mode) usb_joystick_send();
    }

    void Zrotate(unsigned int val)
    {
        if (val > 1023) val = 1023;
        usb_joystick_data[1] = (usb_joystick_data[1] & 0xFFFC0FFF) | (val << 12);
        if (!manual_mode) usb_joystick_send();
    }

    void slider(unsigned int val)
    {
        if (val > 127) val = 127;
        usb_joystick_data[1] = (usb_joystick_data[1] & 0xFE03FFFF) | (val << 18);
        if (!manual_mode) usb_joystick_send();
    }

    inline void hat(uint8_t val, uint8_t shift)
    {
        if (val == 0) val = 15;
		switch(shift)
        {
            case 0:
                usb_joystick_data[0] = (usb_joystick_data[0] & 0xFF00FFFF) | (val << 20);
                break;
            case 1:
                usb_joystick_data[0] = (usb_joystick_data[0] & 0xFF00FFFF) | (val << 16);
                break;
            default:
                return;
        }
        if (!manual_mode) usb_joystick_send();
    }

    void useManualSend(bool mode)
    {
        manual_mode = mode;
    }

    void send_now(void)
    {
        usb_joystick_send();
    }
private:
    static uint8_t manual_mode;
};
extern usb_joystick_class Joystick;

#endif // __cplusplus

#endif // JOYSTICK_INTERFACE

#endif // USBjoystick_h_