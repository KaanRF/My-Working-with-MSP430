#include <msp430.h> 
#include "Uart_1.h"

volatile unsigned char i=0;



void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    BCSCTL1=CALBC1_16MHZ;
    DCOCTL=CALDCO_16MHZ;

    uart_init();
    P1DIR = 0x41;//P1.3 input, P1.0 & P1.6 output
    	P1OUT = 0x00; // output bitleri temizleme

    P2SEL=0X00;
    P2DIR=0XFF;

    uart_init();
    uart_string("Kaan Kandemir\r\n");
    uart_string("Uart-1 Deneme");


    IE2|= UCA0TXIE+ UCA0RXIE; // ALIM VE GÖNDERME KESMELERI AKTIF
    __bis_SR_register(GIE); //tum kesmeler aktif
}
	//gönderme kesme
#pragma vector=USCIAB0TX_VECTOR
__interrupt void Uart_Gonderme_ISR(void)
{
	while(!(IFG2 & 0X01));
	P2OUT=i++;

}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void Uart_alma_ISR(void)
{
	while(!(IFG2 & 0X02));
	UCA0TXBUF=UCA0RXBUF;
}
