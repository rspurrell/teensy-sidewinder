#include "main.hpp"

#define PIN_SW_CLOCK CORE_RXD1_PIN
#define PIN_SW_DATA CORE_RXD0_PIN
#define PIN_SW_TRIGGER CORE_RXD2_PIN
#define PIN_SW_POWER_SWITCH CORE_INT19_PIN

extern "C" int main(void)
{
#ifdef DEBUG
    while (!Serial || !Serial.availableForWrite());
    Serial.begin(9600);
    Serial.println("Debug serial connected");
    delay(1000);
#endif

    Joystick.useManualSend(true);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWriteFast(LED_BUILTIN, LOW);

    pinMode(PIN_SW_POWER_SWITCH, OUTPUT);
    digitalWriteFast(PIN_SW_POWER_SWITCH, LOW);

    Sidewinder sw(PIN_SW_CLOCK, PIN_SW_TRIGGER, PIN_SW_DATA);

#ifdef DEBUG
    Serial.printf("\r\nPowering Sidewinder...");
#endif
    delay(1000);

    digitalWriteFast(PIN_SW_POWER_SWITCH, HIGH);
#ifdef DEBUG
    Serial.printf("\r\nSidewinder powered on, waiting 1 second for it to initialize...");
#endif
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
        bool isPacketValid = Sidewinder::CheckParity(packet);
#ifdef DEBUG
        Serial.printf("Packet valid: %d\r\n", isPacketValid);
#endif
        if (!isPacketValid)
        {
            digitalWriteFast(LED_BUILTIN, HIGH);
            sw.IncrementDelay();
            delay(100);
            continue; // packet parity check failed
        }
        digitalWriteFast(LED_BUILTIN, LOW);

        // axis
        Joystick.X(packet.XAxis());
        Joystick.Y(packet.YAxis());
        Joystick.Zrotate(packet.Rotation());
        Joystick.slider(packet.Throttle());

        // hat
        Joystick.hat(packet.Head(), !packet.Shift());

        // buttons
        Joystick.buttons(~packet.bytes[0], !packet.Shift());

        // send packet data to usb output to pc
        Joystick.send_now();

#ifdef DEBUG
        Serial.printf("Cycle time: %d\r\n", (uint32_t)elapsed);
        delay(30); // delay to allow terminal to catch up and show results consistently
#endif
        // delay before next update
        delay(4);
    }
}
