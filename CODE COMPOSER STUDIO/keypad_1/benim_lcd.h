/*
 * benim_lcd.h
 *
 *  Created on: 26 Þub 2013
 *      Author: Kaan
 */

#ifndef BENIM_LCD_H_
#define BENIM_LCD_H_


#define delay_us(x)     __delay_cycles(x)
#define delay_ms(x)     __delay_cycles((x * 1000L))

#define LCD_DATA        P1OUT
#define LCD_CONTROL     P2OUT
#define EN_ACKAPA()     LCD_EN(1),LCD_EN(0)

#define LCD_RS(x)       ( (x) ? (LCD_CONTROL |= BIT7) : (LCD_CONTROL &= ~BIT7) )
#define LCD_EN(x)       ( (x) ? (LCD_CONTROL |= BIT6) : (LCD_CONTROL &= ~BIT6) )

void lcd_komut(unsigned char); // LCD ye komut göndermeye yarar
void lcd_temizle(void);        // LCD ekraný temizler
void lcd_puts(const char*);    // LCD ye string ifade yazar
void lcd_goto(char,char);      // LCD de satýr ve stün olarak istenilen yere gider
void lcd_init(void);           // LCD baþlangýç ayarlarý yapýlýr
void lcd_putch(char);          // LCD ye tek karakter yazmak için kullanýlýr.



#endif /* BENIM_LCD_H_ */
