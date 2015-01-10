#include <msp430.h> 
#include "in430.h"

void terminal_yaz(char *bilgi);


void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    BCSCTL1=CALBC1_1MHZ;
    DCOCTL=CALDCO_1MHZ;

    P1SEL|=0X06; //1.2 VE 1.1 USCI_A0 ICIN SECILDI
    P1SEL2|=0X06; //"	"	"	"	"	"	"
    P1DIR=0X04; //P1.2 CIKIS TANIMLANDI digerleri giriþ

    //UART AYARLARI

    UCA0CTL1=UCSSEL_2; //SMCLK SECILDI
    UCA0BR0=104;   //1MHZ-->> 9600 BAUD VE N=CLOCK FREKANSI/BAUD HIZI
    UCA0BR1=0;  // BAUD 9600;
    UCA0MCTL=UCBRS1;  //MODULASYON UCBRSX=1;
    UCA0CTL1&=~UCSWRST;// UCSWRST DE AKTIF EDILIYOR RESETTE KALMAMASI ICIN

    		_BIS_SR(GIE);  //TUM KESMELER AKTIF

    while(1)
    {
    	if(P1IN & BIT3)
    	{
    	 P1OUT=0X01; //KIRMIZI LED YANSIN BUTONA BASILDIGINDA
    	 __delay_cycles(500000);
    	terminal_yaz("Kaan Kandemir\r\n");
    	__delay_cycles(500000);
         }
    }

}

void terminal_yaz(char *bilgi)
{
	P1OUT|=BIT6;

	unsigned int i;

	unsigned int boyut=strlen(bilgi); //gönderilecek bilgi boyutu olusturuluyor.
	for(i=0;i<boyut;i++)
	{
		while(!(IFG2 & UCA0TXIFG));
		UCA0TXBUF=bilgi[i];

	}

}
