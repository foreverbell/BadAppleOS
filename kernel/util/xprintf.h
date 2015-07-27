/*------------------------------------------------------------------------*/
/* Universal string handler for user console interface  (C)ChaN, 2011     */
/*------------------------------------------------------------------------*/

#ifndef _STRFUNC
#define _STRFUNC

#define va_start(v, l)     __builtin_va_start(v, l)
#define va_arg(v, l)       __builtin_va_arg(v, l)
#define va_end(v)          __builtin_va_end(v)
#define va_copy(d, s)      __builtin_va_copy(d, s)
typedef __builtin_va_list  va_list;

void putc (char c);
void puts (const char* str);
void printf (const char* fmt, ...);
void sprintf (char* buff, const char* fmt, ...);

#endif
