#include <msp430g2553.h>
void Uartinit(void);
void TXdata(unsigned char c);
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	BCSCTL1=CALBC1_8MHZ;
	DCOCTL=CALDCO_8MHZ;

	Uartinit();
	P1OUT=0X41; //P1.3 GÝRÝS ,P1.0 VE P1.6 CIKIS
	P1OUT=0X00;

	__bis_SR_register(GIE);//kesmeler aktif
	while(1)
	{
		if(!(P1IN&BIT3))
		{
			P1OUT=0X01;
			TXdata('m');
			__delay_cycles(16000000);//8MHZ de calistigi icin 2 sn lik bir bekleme
		}
		else
					P1OUT = 0x40;
	}


}

void Uartinit(void)
{
	P1SEL|=(BIT1+BIT2); //P1.1=RX, P1.2 TX
	P1SEL2|=(BIT1+BIT2);// "        "
	
	UCA0CTL1=UCSSEL_2;// SMCLK
	UCA0BR0=52;
	UCA0BR1=0;
	//Modulator ayari
		UCA0MCTL = 0x11;//formule gore

		UCA0CTL1 &= ~UCSWRST; // USCI islemleri icin UCSWRST registeri temizlendi
		IE2 |= UCA0RXIE; // RXD kesmesi aktif edildi

}

void TXdata(unsigned char c )
{
	while(!(IFG2&UCA0TXIFG)); //UCA0TXIFG UCA0TX BUF BOSKEN SET EDILIR
	UCA0TXBUF=c; //data gönderiliyor
}
