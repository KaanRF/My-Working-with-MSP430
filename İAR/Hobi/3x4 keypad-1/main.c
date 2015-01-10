#include "io430.h"
#include "in430.h"
#include "paralel_lcd.h"

unsigned char KEY;

#define  row1  P2OUT_bit.P0
#define  row2  P2OUT_bit.P1
#define  row3  P2OUT_bit.P2
#define  row4  P2OUT_bit.P3

#define  column1  P1IN_bit.P3
#define  column2  P1IN_bit.P4
#define  column3  P1IN_bit.P5

void wait_key(void)
{
   unsigned int i=5000;
   while(i--);
}

void Refresh_Keypad (void)
{
   row1=1;
   wait_key();
   if(column1) KEY=1;
   else if(column2) KEY=2;
   else if(column3) KEY=3;
   row1=0;

   row2=1;
   wait_key();
   if(column1) KEY=4;
   else if(column2) KEY=5;
   else if(column3) KEY=6;
   row2=0;

   row3=1;
   wait_key();
   if(column1) KEY=7;
   else if(column2) KEY=8;
   else if(column3) KEY=9;
   row3=0;

   row4=1;
   wait_key();
   if(column2) KEY=0;
   row4=0;
}

void main(void)
{
   WDTCTL = WDTPW + WDTHOLD;

   DCOCTL=CALDCO_1MHZ;
   BCSCTL1=CALBC1_1MHZ;

   P1SEL = 0x00;
   P1OUT = 0x00;
   P1DIR = 0x07;

   P2SEL = 0x00;
   P2OUT = 0x00;
   P2DIR = 0xFF;


   lcd_init();
   lcd_puts("BASILAN TUS = ");

   for(;;)
   {
     lcd_goto(1,15);
     lcd_putch(KEY+48);
     Refresh_Keypad();
   }
}
