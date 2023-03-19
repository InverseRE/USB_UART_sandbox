/* -*-*- mode: c++ -*-*- */

#include <Arduino.h>
#include "Ticker.h"

#define BUFFER_USB_UART_SIZE 64
#define BUFFER_UART_USB_SIZE 128

Ticker led_ticker;
#define LED PC13
#define LED_ON LOW
#define LED_OFF HIGH

auto& p1 = Serial;  // USB CDC
auto& p2 = Serial2; // A2 (TX) A3 (RX)

static char buf1[BUFFER_USB_UART_SIZE];
static char buf2[BUFFER_UART_USB_SIZE];
static int buf1_len = 0;
static int buf2_len = 0;

void setup()
{
    while (!p1);
    while (!p2);
    p1.begin(115200);
    p2.begin(400000);

    pinMode(LED, OUTPUT);
    led_ticker.set_next(1000);
    digitalWrite(LED, LED_ON);
}

void loop()
{
    // UART RX
    int p2_len = p2.available();

    if (p2_len > 0) {
        digitalWrite(LED, LED_ON);
        p2_len = min(p2_len, (int)sizeof(buf2) - buf2_len);
        buf2_len += p2.readBytes(buf2 + buf2_len, p2_len);

        if (buf2_len < (int)sizeof(buf2))
            return;
    }

    // USB TX
    if (buf2_len > 0) {
        digitalWrite(LED, LED_ON);
        int len = p1.write(buf2, buf2_len);
        buf2_len -= len;
        memmove(buf2, buf2 + len, buf2_len);

        if (len > 0)
            return;
    }

    // UART TX
    if (buf1_len > 0) {
        digitalWrite(LED, LED_ON);
        int len = p2.write(buf1, buf1_len);
        buf1_len -= len;
        memmove(buf1, buf1 + len, buf1_len);

        if (len > 0)
            return;
    }

    // USB RX
    int p1_len = p1.available();

    if (p1_len > 0) {
        digitalWrite(LED, LED_ON);
        p1_len = min(p1_len, (int)sizeof(buf1) - buf1_len);
        buf1_len += p1.readBytes(buf1 + buf1_len, p1_len);

        if (buf1_len < (int)sizeof(buf1))
            return;
    }

    // NO OP
    digitalWrite(LED, LED_OFF);
}
