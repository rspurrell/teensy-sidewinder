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

    Sidewinder sw(PIN_SW_CLOCK, PIN_SW_TRIGGER, PIN_SW_DATA);

    delay(1000);
    
    sw_data_t packet;
    while (1)
    {
#ifdef DEBUG
        elapsedMicros elapsed;
#endif
		packet = sw.Poll();

		// validate packet
		bool isPacketValid = sw.CheckParity(packet);
#ifdef DEBUG	
		Serial.printf("Packet valid: %d\r\n", isPacketValid);
#endif

		// write packet data to usb output to pc

		// delay before next update
#ifndef DEBUG
		delay(4);
#else
		Serial.printf("Cycle time: %d\r\n", (uint32_t)elapsed);
#endif
    }

#endif
}

