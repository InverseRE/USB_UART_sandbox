/* -*-*- mode: c++ -*-*- */

#include <Arduino.h>
#include "Ticker.h"

#define BUFFER_SIZE 64

Ticker led_ticker;
#define LED PC13
#define LED_ON LOW
#define LED_OFF HIGH

auto& p1 = Serial;
auto& p2 = Serial2;

void setup()
{
    while (!p1);
    while (!p2);
    p1.begin(115200);
    p2.begin(250000);

    pinMode(LED, OUTPUT);
    led_ticker.set_next(1000);
    digitalWrite(LED, LED_ON);
}

static char buf_1[BUFFER_SIZE];
static char buf_2[BUFFER_SIZE];

void loop()
{
    if (led_ticker.mark()) {
        led_ticker.set_next(5000);
        digitalWrite(LED, LED_OFF);
    }

    int len_r1 = p1.available();
    int len_r2 = p2.available();

    if (len_r1 > 0) {
        len_r1 = min(len_r1, (int)sizeof(buf_1));
        p1.readBytes(buf_1, len_r1);
        p2.write(buf_1, len_r1);
    }

    if (len_r2 > 0) {
        len_r2 = min(len_r2, (int)sizeof(buf_2));
        p2.readBytes(buf_2, len_r2);
        p1.write(buf_2, len_r2);
    }

    if (len_r1 > 0|| len_r2 > 0) {
        led_ticker.set_next(200);
        digitalWrite(LED, LED_ON);
    }
}
