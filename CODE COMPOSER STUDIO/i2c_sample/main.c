#include "msp430.h"

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;
  // Stop Watchdog Timer
  P1DIR|=BIT7+BIT6;
  P1SEL |= BIT6+BIT7;                            // Assign I2C pins to USCI_B0
  P1REN=BIT6+BIT7;
  UCB0CTL1 |= UCSWRST;                      // Enable SW reset
  UCB0CTL0 = UCMST+UCMODE_3+UCSYNC;         // I2C Master, synchronous mode
  UCB0CTL1 = UCSSEL_2+UCSWRST;              // Use SMCLK, keep SW reset
  UCB0BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz
  UCB0BR1 = 0;
  UCB0I2CSA = 0x20;                         // Set slave address
  UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
  IE2 |= UCB0RXIE;                          // Enable RX interrupt
  TA0CCTL0 = CCIE;                           // TACCR0 interrupt enabled
  TA0CTL = TASSEL_2 + MC_2;                  // SMCLK, contmode

  while (1)
  {
    __bis_SR_register(CPUOFF + GIE);        // CPU off, interrupts enabled
    UCB0CTL1 &= ~UCTR;                      // I2C RX
    UCB0CTL1 |= UCTXSTT;                    // I2C start condition
    while (UCB0CTL1 & UCTXSTT);             // Loop until I2C STT is sent
    UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition
    __bis_SR_register(CPUOFF + GIE);        // CPU off, interrupts enabled
    while (UCB0CTL1 & UCTXSTT);             // Loop until I2C STT is sent
    UCB0CTL1 |= UCTXSTP;                    // I2C stop condition after 1st TX
  }
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void TA0_ISR(void)
{
  __bic_SR_register_on_exit(CPUOFF);        // Exit LPM0
}

// USCI_B0 Data ISR
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
  UCB0TXBUF = (UCB0RXBUF << 4) | 0x0f;      // Move RX data to TX
  __bic_SR_register_on_exit(CPUOFF);        // Exit LPM0
}
