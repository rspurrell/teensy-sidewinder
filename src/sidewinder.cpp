#include "sidewinder.h"

const sw_data_t Sidewinder::SW_DATA_EMPTY = {};

Sidewinder::Sidewinder(uint8_t pinClock, uint8_t pinTrigger, uint8_t pinData)
{
    pin_clock = pinClock;
    pinMode(pin_clock, INPUT);

    pin_data = pinData;
    pinMode(pin_data, INPUT);

    pin_trigger = pinTrigger;
    pinMode(pin_trigger, OUTPUT);
    digitalWriteFast(pin_trigger, LOW);
}

Sidewinder::~Sidewinder(){}

bool Sidewinder::CheckParity(sw_data_t p)
{
    bool parity = 1; // even parity
    uint64_t n = ((uint64_t)p.bytes[5] & 0x7F) << 40 | // remove parity bit
        (uint64_t)p.bytes[4] << 32 |
        (uint32_t)p.bytes[3] << 24 |
        (uint32_t)p.bytes[2] << 16 |
        (uint16_t)p.bytes[1] << 8 |
        p.bytes[0];
    while (n)
    {
        parity = !parity;
        n &= (n - 1);
    }
    return parity == p.Parity();
}

sw_data_t Sidewinder::Poll()
{
    sw_data_t volatile p = SW_DATA_EMPTY;
    uint32_t clkFlip, bitsRead, bytesRead;
    clkFlip = bitsRead = bytesRead = 0;
        
    digitalWriteFast(pin_trigger, HIGH);
	delayMicroseconds(TRIGGER_HOLD);
    digitalWriteFast(pin_trigger, LOW);

    uint8_t volatile b;
    elapsedMicros elapsed;
    do
    {
        if (clkFlip && digitalReadFast(pin_clock))
        {
            b = digitalReadFast(pin_data);
            p.bytes[bytesRead] |= b << bitsRead;
            bitsRead++;
            if (bitsRead == 8)
            {
                bitsRead = 0;
                bytesRead++;
            }
            clkFlip = 0;
        }
        else if (!clkFlip && !digitalReadFast(pin_clock))
        {
            clkFlip = 1;
        }
        if (bytesRead >= PACKET_DATA_BYTES_COUNT)
        {
            break;
        }
        delayMicroseconds(1);
    } while (elapsed < TIMEOUT);

#ifdef DEBUG
    debug((sw_data_t&)p, bytesRead, elapsed);
#endif

    return (sw_data_t&)p;
}

#ifdef DEBUG
void Sidewinder::debug(sw_data_t p, uint32_t bytesRead, uint32_t elapsed)
{
    Serial.printf("\r\nBytes read: ");
    Serial.printf(bytesRead < 6 ? "TIMEOUT" : "%d in %d", bytesRead, elapsed);
    Serial.printf("\r\nTr:%d T:%d U:%d D:%d A:%d B:%d C:%d D:%d S:%d X:%4d Y:%4d Th:%3d R:%3d H:%d Chk:%d %d",
        p.Trigger()
        ,p.Top()
        ,p.Up()
        ,p.Down()
        ,p.A()
        ,p.B()
        ,p.C()
        ,p.D()
        ,p.Shift()
        ,p.XAxis()
        ,p.YAxis()
        ,p.Throttle()
        ,p.Rotation()
        ,p.Head()
        ,p.Reserved()
        ,p.Parity()
    );

    uint8_t b;

    // write bits segmented by report
    Serial.println("\r\npp HHHH RRRRRR TTTTTTT YYYYYYYYYY XXXXXXXXXX BBBBBBBBB\r\n   3210 543210 6543210 9876543210 9876543210 987654321");
    b = p.bytes[5];
    Serial.printf("%d%d %d%d%d%d %d%d", b >> 7 & 1, b >> 6 & 1, b >> 5 & 1, b >> 4 & 1, b >> 3 & 1, b >> 2 & 1, b >> 1 & 1, b & 1);
    b = p.bytes[4];
    Serial.printf("%d%d%d%d %d%d%d%d", b >> 7 & 1, b >> 6 & 1, b >> 5 & 1, b >> 4 & 1, b >> 3 & 1, b >> 2 & 1, b >> 1 & 1, b & 1);
    b = p.bytes[3];
    Serial.printf("%d%d%d %d%d%d%d%d", b >> 7 & 1, b >> 6 & 1, b >> 5 & 1, b >> 4 & 1, b >> 3 & 1, b >> 2 & 1, b >> 1 & 1, b & 1);
    b = p.bytes[2];
    Serial.printf("%d%d%d%d%d %d%d%d", b >> 7 & 1, b >> 6 & 1, b >> 5 & 1, b >> 4 & 1, b >> 3 & 1, b >> 2 & 1, b >> 1 & 1, b & 1);
    b = p.bytes[1];
    Serial.printf("%d%d%d%d%d%d%d %d", b >> 7 & 1, b >> 6 & 1, b >> 5 & 1, b >> 4 & 1, b >> 3 & 1, b >> 2 & 1, b >> 1 & 1, b & 1);
    b = p.bytes[0];
    Serial.printf("%d%d%d%d%d%d%d%d\r\n", b >> 7 & 1, b >> 6 & 1, b >> 5 & 1, b >> 4 & 1, b >> 3 & 1, b >> 2 & 1, b >> 1 & 1, b & 1);

    // write 8-bit bytes
    for (int i = 5; i >= 0; i--)
    {
        b = p.bytes[i];
        Serial.printf("%d%d%d%d%d%d%d%d ", b >> 7 & 1, b >> 6 & 1, b >> 5 & 1, b >> 4 & 1, b >> 3 & 1, b >> 2 & 1, b >> 1 & 1, b & 1);
    }
    Serial.println();
}
#endif