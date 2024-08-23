#include <debug.h>
#include <settings.h>

void debug_init(int uart_speed)
{
    #ifdef DEBUG_ENABLED
        Serial.begin(uart_speed);
    #endif
}

void debug_print(const __FlashStringHelper *msg)
{
    #ifdef DEBUG_ENABLED
    Serial.println(msg);
    #endif
}

void debug_print(const String msg)
{
    #ifdef DEBUG_ENABLED
    Serial.println(msg);
    #endif
}