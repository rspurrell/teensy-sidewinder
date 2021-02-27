#ifndef TeensySidewinder
#define TeensySidewinder

#include "usb_sidewinder.h"
#include <Arduino.h>

#define PACKET_DATA_BYTES_COUNT 6
#define TIMEOUT 1000 // 1 millisecond
#define TRIGGER_HOLD 600 // microseconds

typedef struct
{
    // the ints are used to access the struct-data at bit-level
    uint8_t volatile bytes[6];

    // bit  1
    const uint8_t Trigger() const { return bytes[0] & 0x01; }
    // bit  2
    const uint8_t Top() const { return bytes[0] >> 1 & 0x01; }
    // bit  3
    const uint8_t Up() const { return bytes[0] >> 2 & 0x01; }
    // bit  4
    const uint8_t Down() const { return bytes[0] >> 3 & 0x01; }

    // bit  5
    const uint8_t A() const { return bytes[0] >> 4 & 0x01; }
    // bit  6
    const uint8_t B() const { return bytes[0] >> 5 & 0x01; }
    // bit  7
    const uint8_t C() const { return bytes[0] >> 6 & 0x01; }
    // bit  8
    const uint8_t D() const { return bytes[0] >> 7 & 0x01; }
        
    // bit  9
    const uint8_t Shift() const { return bytes[1] & 0x01; }

    // bits 10-19
    const uint16_t XAxis() const { return (bytes[2] & 0x07) << 7 | bytes[1] >> 1; }
    // bits 20-29
    const uint16_t YAxis() const { return (bytes[3] & 0x1F) << 5 | bytes[2] >> 3; }
    // bits 30-36
    const uint8_t Throttle() const { return (bytes[4] & 0x0F) << 3 | bytes[3] >> 5; }
    // bits 37-42
    const uint8_t Rotation() const { return (bytes[5] & 0x03) << 4 | bytes[4] >> 4; }

    // bits 43-46
    const uint8_t Head() const { return bytes[5] >> 2 & 0x0F; }

    // bit 47
    const uint8_t Reserved() const { return bytes[5] >> 6 & 0x01; }
    // bit 48
    const bool Parity() const { return bytes[5] >> 7 & 0x01; }
} sw_data_t;

class Sidewinder
{
private:
    static const sw_data_t SW_DATA_EMPTY;
    uint8_t pin_clock, pin_trigger, pin_data;

    #ifdef DEBUG
    void debug(sw_data_t p, uint32_t bytesRead, uint32_t elapsed);
    #endif

public:
    Sidewinder(uint8_t pinClock, uint8_t pinTrigger, uint8_t pinData);
    ~Sidewinder();

    sw_data_t Poll();
    bool CheckParity(sw_data_t p);
};

#endif