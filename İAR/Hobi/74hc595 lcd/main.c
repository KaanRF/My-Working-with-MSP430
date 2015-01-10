#include "io430.h"
#include "in430.h"
#include "paralel_lcd.h"
 

 
void main(void)
{
  WDTCTL =  WDTPW + WDTHOLD;
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL =  CALDCO_1MHZ;
 
  P1DIR |= BIT0|BIT1|BIT2|BIT6;
  //P1.0, P1.1, P1.2 ve P1.6 Çýkýþ
 
  P1OUT=0x00;
  lcd_init();
  
  while(1)
  {
  lcd_goto(1,3);
  lcd_puts("KAAN KANDEMÝR");
  lcd_goto(2,2);
  lcd_puts("74hc595-LCD");
 
  delay(2000);
  lcd_temizle();
 
  delay(2000);
  lcd_temizle();
  }
}