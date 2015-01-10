/*
 * Uart_1.h
 *
 *  Created on: 02 Mar 2013
 *      Author: Kaan
 */

#ifndef UART_1_H_
#define UART_1_H_

#define CLK             16000000  // SMCLK deðeri
#define UART0_BAUD_RATE 19200 //BAUDRATE
#define UART0_WORD_LENGTH  0 //1 OLUNCA 7 BÝT 0 OLUNCA 8 BÝT
#define UART0_STOP_BIT     0 //0:STOP BIT=1
#define UART0_PARITY       0 //0:PARITY YOK 1:PARTY VAR
#define UART0_PARITY_SEL   0 //0:ODD ,1:EVENT

extern void uart_init(void);
extern void uart_putch(unsigned char c);
extern char uart_getch(void);
extern void uart_string(const char *st);

#endif /* UART_1_H_ */
