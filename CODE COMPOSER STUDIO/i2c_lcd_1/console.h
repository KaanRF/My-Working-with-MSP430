#ifndef CONSOLE_H
#define CONSOLE_H 1

#include "common.h"

void console_init(void);
void console_tick(void);

void console_putc(uint8_t c);
void console_newline(void);
void console_puts(const char *str);
void console_puthex8(uint8_t h);

//void console_putbin(uint8_t b);

#endif
