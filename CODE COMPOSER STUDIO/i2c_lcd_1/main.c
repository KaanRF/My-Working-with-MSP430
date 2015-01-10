
//#include "io430.h"
#include "common.h"
#include "uart.h"
#include "console.h"
#include "shell.h"
#include "i2c.h"

// LED is P1.0 LED1 on Launchpad
// Serial is Half-Duplex Software UART on Launchpad standard TX/RX Pins
// i2c is USI Hardware i2c interface on standard USI i2c pins
// P1.6/LED2 Jumper should be removed

// LED on P1.0
// TX  on P1.1
// RX  on P1.2
// SCL on P1.6
// SDA on P1.7

void cpu_init(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop WatchDog Timer

    BCSCTL1 = CALBC1_1MHZ;    // Set range to 1mhz Calibrated Range
    DCOCTL = CALDCO_1MHZ;     // Set DCO to 1mhz Calibrated DCO
                              // SMCLK = DCO = 1MHz
}

int main(void)
{
    cpu_init();
    uart_init();
    console_init();
    i2c_init();

    _BIS_SR(GIE); // Enable Interrupts, No Low Power Mode

    while(1)
    {
        console_tick();
    }
}
