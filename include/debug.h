#ifndef RDA5807_RECEIVER_DEBUG_H
#define RDA5807_RECEIVER_DEBUG_H

#include <Arduino.h>

void debug_init(int uart_speed);
void debug_print(const __FlashStringHelper* msg);
void debug_print(const String msg);

#endif