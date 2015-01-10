/*
 * Uart_1.c
 *
 *  Created on: 03 Mar 2013
 *      Author: Kaan
 */

#include "msp430.h"
#include "Uart_1.h"

void uart_init(void) //uart ayarlamalar�n� yap�yoruz
{
	P1SEL |= (BIT1 + BIT2);//P1.1 = RXD P1.2 = TXD
	P1SEL2 |= (BIT1 + BIT2);//P1.1 = RXD P1.2 = TXD
	UCA0CTL0|=0X00; //UART MODE SECILDI USER GUIDE S:440
	UCA0CTL0=(UART0_PARITY<<7 )|( UART0_PARITY_SEL<<6) | (UART0_WORD_LENGTH<<4)|(UART0_STOP_BIT<<3);
	//ASENKRON MODU ICIN AYARLAMALAR YAPILDI

	//CLOCK AYARLARIMIZI YAPALIM SMCLK KULLANICAZ
	UCA0CTL1|=UCSSEL_2; // SMCLK SECILDI
	UCA0BR1=(CLK/UART0_BAUD_RATE)/256; // BAUD RATE SETT�NG
	UCA0BR0=(CLK/UART0_BAUD_RATE)/256; //"	 	"		"

	UCA0MCTL = UCBRS0; //BAUD RATE HESABINA G�RE DEGISIR USER GUIDE S:435DEN BAKAB�L�RS�N�Z

	UCA0CTL1 &=0XFE; //RESET DURUMU KALDIRILIYOR YAN� UCSWRST 0 Disabled. USCI reset released for operation.

}

void uart_putch(unsigned char c)
{
	while(!(IFG2 & 0X02)); // G�NDERIM FIFO DENETIM
	UCA0TXBUF=c; // ALICI BUFFERA C KAYDEDILIYOR

}

char uart_getch(void)
{
  while(!(IFG2 & 0x01));        // Receive al�m FIFO'su bo� mu?
    return UCA0RXBUF;
}

void uart_string(const char *st)
{
	while(*st)
		uart_putch(*st++);

}


