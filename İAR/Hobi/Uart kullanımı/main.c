
#include "io430.h"


//foksiyon prototipleri
void UARTInit(void);
void TXData(unsigned char c);

void main(void){
	WDTCTL = WDTPW + WDTHOLD;// watchdog timer kapali
	//DCO clock frekansi ayarlari
	BCSCTL1 = CALBC1_8MHZ;
	DCOCTL = CALDCO_8MHZ;

	UARTInit();//Uart ayarlari cagirildi

	P1DIR = 0x41;//P1.3 input, P1.0 & P1.6 output
	P1OUT = 0x00; // output bitleri temizleme

	__bis_SR_register(GIE);//kesmeler aktif

	while(1){
		if(!(P1IN&BIT3)){
			P1OUT = 0x01;
			TXData('M');
			__delay_cycles(16000000);//8MHZ de calistigi icin 2 sn lik bir bekleme
		}
		else
			P1OUT = 0x40;
	}
}
//Uart Ayarlari
void UARTInit(void){
	P1SEL |= (BIT1 + BIT2);//P1.1 = RXD P1.2 = TXD
	P1SEL2 |= (BIT1 + BIT2);//P1.1 = RXD P1.2 = TXD
	UCA0CTL1 = UCSSEL_2; // clock source SMCLK
	//UCOS16 = 1 icin Baudrate ayarlari
	UCA0BR0 = 52;
	UCA0BR1 = 0;
	//Modulator ayari
	UCA0MCTL = 0x11;//formule gore bkz.

	UCA0CTL1 &= ~UCSWRST; // USCI islemleri icin UCSWRST registeri temizlendi

	IE2 |= UCA0RXIE; // RXD kesmesi aktif edildi
}

void TXData(unsigned char c){
	while(!(IFG2&UCA0TXIFG));//UCA0TXIFG UCA0TXBUF bos iken set edilir.
	UCA0TXBUF = c; // data gonderiliyor
}
