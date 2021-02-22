#include "sidewinder.h"

#define PIN_SW_CLOCK CORE_RXD1_PIN
#define PIN_SW_DATA CORE_RXD0_PIN
#define PIN_SW_TRIGGER CORE_TXD1_PIN

extern "C" int main(void)
{
#ifdef USING_MAKEFILE

#ifdef DEBUG
    while (!Serial || !Serial.availableForWrite());
    Serial.begin(9600);
    Serial.println("Debug serial connected");
#endif

    Joystick.useManualSend(true);

    Sidewinder sw(PIN_SW_CLOCK, PIN_SW_TRIGGER, PIN_SW_DATA);

    delay(1000);
    
    sw_data_t packet;
    while (1)
    {
#ifdef DEBUG
        elapsedMicros elapsed;
#endif

        // retrieve sidewinder packet
		packet = sw.Poll();

		// validate packet
		bool isPacketValid = sw.CheckParity(packet);
#ifdef DEBUG	
		Serial.printf("Packet valid: %d\r\n", isPacketValid);
#endif
        if (!isPacketValid)
        {
            continue; // packet parity check failed
        }

        // axis
        Joystick.X(packet.XAxis());
        Joystick.Y(packet.YAxis());
        Joystick.Zrotate(((packet.Rotation() + 1) << 4) - 1);
        Joystick.slider(((packet.Throttle() + 1) << 3) - 1);

        // hat
        int16_t angle = -1;
        if (packet.Head() > 0)
        {
            uint8_t h = packet.Head();
            if (h == 1)
                h = 9;
            h--;
            angle = h * 45;
        }
        Joystick.hat(angle);

        // buttons
        uint8_t shift = !packet.Shift() << 3;
        uint8_t btns = packet.bytes[0];
        for (size_t i = 0; i < 8; i++)
        {
            // clear current button value
            Joystick.button(i + 1, 0);
            Joystick.button((i | 8) + 1, 0);

            // set new button value
            Joystick.button((i | shift) + 1, !(btns >> i & 1));
        }

        // send packet data to usb output to pc
        Joystick.send_now();

		// delay before next update
#ifndef DEBUG
		delay(4);
#else
		Serial.printf("Cycle time: %d\r\n", (uint32_t)elapsed);
#endif
    }

#endif
}

