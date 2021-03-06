#include "common.h"
#include "console.h"
#include "shell.h"
#include "uart.h"

#define CMDBUF_SIZ 64

#if CMDBUF_SIZ > 255
#error Commands over 255 bytes not supported, change type of cmdbuf_len
#endif

/**************************************************************/

static uint8_t cmdbuf_len; // number of bytes in command buf
static uint8_t cmdbuf[CMDBUF_SIZ];
static BOOL got_line = FALSE;

/**************************************************************/

static void prompt(void)
{
    console_puts("i2c> ");
}

void console_init(void)
{
    cmdbuf_len = 0;
    got_line = FALSE;
}


static void console_rx(uint8_t c)
{
    if (got_line)   // throw away chars until the line is handled
        return;

    switch(c)
    {
        case 0x0D: // \r
            got_line = TRUE;
            console_newline();
            break;
        case '\b':  // backspace
        case 0x7F:  // del
            if (cmdbuf_len > 0)
            {
                cmdbuf_len--;
                console_puts("\b \b");
            }
            break;
        default:
            if (cmdbuf_len < sizeof(cmdbuf)-1)
            {
                console_putc(c);
                cmdbuf[cmdbuf_len++] = c;
            }
            else
                console_putc('\a');  // bell
            break;
    }
}

void console_tick(void)
{
    if (!got_line)
    {
        uint8_t c;
        if (uart_getc(&c))
            console_rx(c);
    }

    if (got_line)
    {
        if (cmdbuf_len > 0)
            shell_eval(cmdbuf, cmdbuf_len);
        cmdbuf_len = 0;
        console_newline();
        prompt();
        got_line = FALSE;
    }
}

/**************************************************************/

void console_putc(uint8_t c)
{
    uart_putc(c);
}

void console_newline(void)
{
    console_putc('\r');
    console_putc('\n');
}

void console_puts(const char *str)
{
    while(*str!=0)
        console_putc(*str++);
}

static char nibble_to_char(uint8_t nibble)
{
    if (nibble < 0xA)
        return nibble + '0';
    return nibble - 0xA + 'A';
}

void console_puthex8(uint8_t h)
{
    console_putc(nibble_to_char((h & 0xF0)>>4));
    console_putc(nibble_to_char(h & 0x0F));
}

/*void console_putbin(uint8_t b)
{
    console_putc('b');
    (b & 0x80) ? console_putc('1') : console_putc('0');
    (b & 0x40) ? console_putc('1') : console_putc('0');
    (b & 0x20) ? console_putc('1') : console_putc('0');
    (b & 0x10) ? console_putc('1') : console_putc('0');
    (b & 0x08) ? console_putc('1') : console_putc('0');
    (b & 0x04) ? console_putc('1') : console_putc('0');
    (b & 0x02) ? console_putc('1') : console_putc('0');
    (b & 0x01) ? console_putc('1') : console_putc('0');
}*/
