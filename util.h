#ifndef UTIL_H
#define UTIL_H

/* helper macros to improve readability and avoid mistakes */
#define LEN(x) (sizeof(x) / sizeof(*x))
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define ABS(x) ((x) > 0 ? (x) : -(x))
#define SIGN(x) (((x) > 0) - ((x) < 0))

/* to be set to a function that write `s` to the user, such as over an UART */
extern int print(char const *s);

/* parse an unsigned integer up to `max` (included) in base `b` (up to 16)
 * from (*`sp`) and shift the pointer to the end, or set it to NULL on error */
uint64_t strint(char const **sp, uint64_t max, uint8_t b);

/* format a signed integer `i64` at the end of `s` of size `sz` using base `b`
 * and return the first printable byte of `s` */
char *intstr(char *s, size_t sz, int64_t i64, uint8_t b);

/* print each string argument until NULL is encountered */
int put_(char const *s, ...);

/* safer and easier wrapper around put_() */
#define put(...) put_(__VA_ARGS__, NULL)

/* macros to format integers and print them B(bin), D(dec), X(hex) */
#define B(n) intstr((char[65]){0}, 65, n, 2)
#define D(n) intstr((char[21]){0}, 21, n, 10)
#define X(n) intstr((char[17]){0}, 17, n, 16)
#define C(c) (char[2]){ c, '\0' }

#endif
