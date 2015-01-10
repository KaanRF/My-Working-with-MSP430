#include "common.h"
#include "console.h"
#include "parse.h"
#include "shell.h"
#include "i2c.h"

enum
{
    STATE_SEARCHING,
    STATE_IN_TOKEN,
};

static void bus_i2c_start(void)
{
    i2c_start();

    console_puts("i2c START");
    console_newline();
}

static void bus_i2c_stop(void)
{
    i2c_stop();

    console_puts("i2c STOP");
    console_newline();
}

static void bus_i2c_write(uint8_t c)
{
    uint8_t ack;
    ack = i2c_write8(c);
    console_puts("WRITE: 0x");
    console_puthex8(c);
    if ((ack & 0x01) == 0x00)
        console_puts(" Ack");
    else
        console_puts(" Nack!");
    console_newline();
}

static void bus_i2c_read(uint8_t acknack)
{
    uint8_t c;
    c = i2c_read8(acknack);
    console_puts("READ: 0x");
    console_puthex8(c);
    if ((acknack & 0x01) == 0x00)
        console_puts(" Ack");
    else
        console_puts(" Nack!");
    console_newline();
}


static BOOL handle_command(const uint8_t *start, const uint8_t *end, BOOL firstToken, const uint8_t *line_end)
{
    uint32_t num;
    const uint8_t *ptr = start;
    uint32_t repeat = 1;

    // handle repeat commands, A:5
    while(ptr != end)
    {
        if (*ptr == ':' && ptr-start > 0)   // found at least one character followed by ':'
        {
            if (parse_number(ptr+1, end - (ptr+1), &repeat))
            {
                end = ptr;    // discard the ":X" part
                break;
            }
            else
                repeat = 0; // parse_number is destructive
        }
        ptr++;
    }

    // handle write command (a number)
    if (parse_number(start, end-start, &num))
    {
        while(repeat--)
            bus_i2c_write((uint8_t)num);
        return FALSE;
    }
    else
    {
        if (end-start == 1 && (start[0] == 'r' || start[0] == 'R'))
        {   // handle read command with ACK
            while(repeat--)
                bus_i2c_read(0x00);
            return FALSE;
        }
        else if (end-start == 1 && (start[0] == 'n' || start[0] == 'N'))
        {   // handle read command with NACK
            while(repeat--)
                bus_i2c_read(0xFF);
            return FALSE;
        }
        else if (end-start == 1 && (start[0] == 's' || start[0] == 'S'))
        {   // scan all 0xFF addresses for slaves
             uint8_t i;
            for (i=0;i<255;i++)
            {    i2c_start();
                 bus_i2c_write(i);
                 i2c_stop();
            }
                 bus_i2c_write(0xFF);

/*volatile unsigned int hick;
    i = 10000;                          // SW Delay
    do i--;
    while (i != 0);*/
            return FALSE;
        }
        else
        {   // handle other commands
            // ignore
        }
    }

    console_puts("BadCmd");
    console_newline();

    return FALSE;
}


void shell_eval(const uint8_t *str, uint32_t len)
{
    uint8_t state = STATE_SEARCHING;
    uint8_t c;
    const uint8_t *end = str+len;
    const uint8_t *token_start = str;
    BOOL firstToken = TRUE;

    while(str != end)
    {
        c = *str;

        switch(state)
        {
            case STATE_SEARCHING:
                switch(c)
                {
                    case '#':   // comment
                        return; // ignore the rest of the line

                    case ' ':   // whitespace
                    case '\t':
                    case ',':
                        break;

                    case '[':
                    case '{':
                        firstToken = FALSE;
                        bus_i2c_start();
                        break;

                    case ']':
                    case '}':
                        firstToken = FALSE;
                        bus_i2c_stop();
                        break;

                    default:    // start of token
                        state = STATE_IN_TOKEN;
                        token_start = str;
                        break;
                }
                str++;  // next character
                break;  // end of case STATE_SEARCHING

            case STATE_IN_TOKEN:
                switch(c)
                {
                    case ' ':   // end of token
                    case '\t':
                    case ',':
                    case '[':
                    case '{':
                    case ']':
                    case '}':
                    case '#':
                        if (handle_command(token_start, str, firstToken, end))
                            return;
                        firstToken = FALSE;
                        state = STATE_SEARCHING;
                        break;

                    default:
                        // keep scanning token
                        str++;
                        break;
                }
        }
    }

    if (STATE_IN_TOKEN == state)
        handle_command(token_start, end, firstToken, end);
}
