#define Clock P1OUT_bit.P0
#define Data P1OUT_bit.P1
#define Storage P1OUT_bit.P2
#define nop() asm("nop")
 
void delay(unsigned long int);
void hc595_yaz(unsigned char);
void lcd_write(unsigned char);
void lcd_putch(unsigned char);
void lcd_puts(const char*);
void lcd_temizle(void);
void lcd_goto(char,char);
void lcd_init(void);