#include "io430.h"
#include "paralel_lcd.h"
 
#define E_1     0x08
#define RS_E_1  0x0C
 
void delay(unsigned long int d)
{
  d*=100;
  for(;d>0;d--);
}
 
void hc595_yaz(unsigned char gelen)
{
  for(char i=8;i>0;i--)
  {
    Data=0;
    if(gelen & 0x80)
    Data=1;
    Clock=1;
    Clock=0;
    gelen*=2;
  }
  Storage=1;
  Storage=0;
}
 
void lcd_write(unsigned char port)
{
  //ilk 4 bit gönderiliyor...
  hc595_yaz(E_1);
  hc595_yaz(( port & 0xF0) | E_1);
  hc595_yaz(((port & 0xF0) | E_1) & 0xF0);
  //Son 4 bit gönderiliyor...
  hc595_yaz(E_1);
  hc595_yaz(( port<<4 ) | E_1);
  hc595_yaz(((port<<4 ) | E_1) & 0xF0);
}
 
void lcd_putch(unsigned char port)
{
  //ilk 4 bit gönderiliyor...
  hc595_yaz(RS_E_1);
  hc595_yaz(( port & 0xF0 ) | RS_E_1);
  hc595_yaz(((port & 0xF0 ) | RS_E_1) & 0xF4);
  //Son 4 bit gönderiliyor...
  hc595_yaz(RS_E_1);
  hc595_yaz(( port<<4) | RS_E_1);
  hc595_yaz(((port<<4) | RS_E_1) & 0xF4);
}
 
void lcd_puts(const char * s)
{
  nop();
  while(*s)
    lcd_putch(*s++);
}
 
void lcd_temizle(void)
{
  lcd_write(0x1);
  delay(2);
}
 
void lcd_goto(char x, char y)
{
  if(x==1)
    lcd_write(0x80+((y-1)%16));
  else
    lcd_write(0xC0+((y-1)%16));
}
 
void lcd_init(void)
{
  hc595_yaz(0x00);
  delay(15);
  hc595_yaz(0x08);
  lcd_write(0x02);  // Ýmleç 0x0 konumunda
  delay(2);
 
  lcd_write(0x28);  // 4 Bit , Çift Satýr LCD
  lcd_write(0x0C);  // Ýmleç Gizleniyor
  lcd_temizle();    // Ekran Temizleniyor
  lcd_write(0x06);  // Saða doðru yazma aktif
  lcd_write(0x80);  // LCD Birinci Satýr Konumunda
}