
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>

#define min(a,b) __extension__\
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
       _a < _b ? _a : _b; })

enum flag_itoa {
    FILL_ZERO = 1,
    PUT_PLUS = 2,
    PUT_MINUS = 4,
    BASE_2 = 8,
    BASE_10 = 16,
};


///////////////////////////////////////////////////////////////////////////////
// local functions
static char *sitoa(char * buf, unsigned int num, int width, enum flag_itoa flags)
{
    unsigned int base;
    if (flags & BASE_2)
        base = 2;
    else if (flags & BASE_10)
        base = 10;
    else
        base = 16;

    char tmp[32];
    char *p = tmp;
    do {
        int rem = num % base;
        *p++ = (rem <= 9) ? (rem + '0') : (rem + 'a' - 0xA);
    } while ((num /= base));
    width -= p - tmp;
    char fill = (flags & FILL_ZERO)? '0' : ' ';
    while (0 <= --width) {
        *(buf++) = fill;
    }
    if (flags & PUT_MINUS)
        *(buf++) = '-';
    else if (flags & PUT_PLUS)
        *(buf++) = '+';
    do
        *(buf++) = *(--p);
    while (tmp < p);
    return buf;
}

static int snprintf_va(char *buf, size_t maxlen, const char *fmt, va_list va)
{
    int i;
    char c;
    const char *save = buf;

    while ((c  = *fmt++)) {
        int width = 0;
        enum flag_itoa flags = 0;
        if (c != '%') {
            *(buf++) = c;
            continue;
        }
    redo_spec:
        c  = *fmt++;
        switch (c) {
        case '%':
            *(buf++) = c;
            break;
        case 'c':;
            *(buf++) = va_arg(va, int);
            break;
        case 'd':;
            int num = va_arg(va, int);
            if (num < 0) {
                num = -num;
                flags |= PUT_MINUS;
            }
            buf = sitoa(buf, num, width, flags | BASE_10);
            break;
        case 'u':
            buf = sitoa(buf, va_arg(va, unsigned int), width, flags | BASE_10);
            break;
        case 'x':
            buf = sitoa(buf, va_arg(va, unsigned int), width, flags);
            break;
        case 'b':
            buf = sitoa(buf, va_arg(va, unsigned int), width, flags | BASE_2);
            break;
        case 's':;
            const char *p  = va_arg(va, const char *);
            if (p) {
                i = 0;
                while (*p) {
                    if (i+1 <= maxlen) {
                        buf[i] = '\0';
                        break;
                    }
                    *(buf++) = *(p++);
                    i++;
                }
            }
            break;
        case 'm':;
            const uint8_t *m  = va_arg(va, const uint8_t *);
            width = min(width, 64); // buffer limited to 256!
            if (m) {
                i = 0;
                for (; (i+1) <= maxlen;) {
                    buf = sitoa(buf, *(m++), 2, FILL_ZERO);
                    if (--width <= 0)
                        break;
                    *(buf++) = ':';
                }
                if ((i+1) < maxlen) {
                    buf[i] = '\0';
                }
            }
            break;
        case '0':
            if (!width)
                flags |= FILL_ZERO;
            // fall through
        case '1'...'9':
            width = width * 10 + c - '0';
            goto redo_spec;
        case '*':
            width = va_arg(va, unsigned int);
            goto redo_spec;
        case '+':
            flags |= PUT_PLUS;
            goto redo_spec;
        case '\0':
        default:
            *(buf++) = '?';
        }
        width = 0;
    }
    *buf = '\0';
    return buf - save;
}


///////////////////////////////////////////////////////////////////////////////
// function: snprintf
int snprintf(char *buf, size_t maxlen, const char *fmt, ...) {
    va_list ap;
    int retval;

    va_start(ap, fmt);
    retval = snprintf_va(buf, maxlen, fmt, ap);
    va_end(ap);

    return retval;
}


///////////////////////////////////////////////////////////////////////////////
// function: strnlen
size_t strnlen(const char *str, size_t maxlen) {
    for (size_t len = 0; len < maxlen; ++str, ++len) { 
        if (!*str) { 
            return len; 
        }
    }
    return maxlen;
}

///////////////////////////////////////////////////////////////////////////////
// function: strncpy
void *memcpy(void *dst, const void *src, size_t num ) {
    char *ptr = dst;
    char *d = (char*)dst;
    char *s = (char*)src;
    while (num--) {
        *d++ = *s++;
    }
    return ptr;
}