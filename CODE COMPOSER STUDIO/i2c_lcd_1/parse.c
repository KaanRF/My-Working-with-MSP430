#include "common.h"
#include "parse.h"
#include <string.h>

// hex nibble to int
static uint8_t digit_to_int(uint8_t ch)
{
    uint8_t r;
    if (ch >= 'a')
        r = 'a' - 10;
    else
    {
        if (ch >= 'A')
            r = 'A' - 10;
        else
        {
            if (ch <= '9')
                r = '0';
            else
                r = 0;
        }
    }

    return ch - r;
}

// parse binary, hex and decimal numbers
BOOL parse_number(const uint8_t *str, uint8_t len, uint32_t *result)
{
    uint8_t base = 10;
    uint8_t i;
    uint8_t c;
    uint8_t digit;

    *result = 0;

    for (i=0;i<len;i++)
    {
        c = str[i];

        if (*result == 0)
        {
            if ((i == 0 || i == 1) && c == 'b')     // 0b/b for binary
            {
                if (len < 2)
                    return FALSE;
                base = 2;
                continue;
            }
            else
            if (i==0 && c == 'h')       // h for hex
            {
                if (len < 2)
                    return FALSE;
                base = 16;
                continue;
            }
            else
            if (i == 1 && c == 'x')     // 0x for hex
            {
                if (len < 3)
                    return FALSE;
                base = 16;
                continue;
            }
        }

        digit = digit_to_int(c);

        if (digit < base)
            *result = (*result) * base + digit;
        else
            return FALSE;
    }
    return TRUE;
}
