#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <ctype.h>
#include "util.h"

static char const *digits = "0123456789ABCDEF";

uint64_t
strint(char const **sp, uint64_t max, uint8_t b)
{
	uint64_t n;
	char const *s, *p;
	char c;

	n = 0;
	for (s = *sp ;; s++) {
		c = toupper(*s);
		p = strchr(digits, c);
		if (p == NULL || p >= digits + b)
			break;

		if (n > max / b)
			goto err;
		n *= b;

		if (n > max - (p - digits))
			goto err;
		n += p - digits;
	}
	if (*sp == s)
		goto err;
	*sp = s;
	return n;
err:
	*sp = NULL;
	return 0;
}

char *
intstr(char *s, size_t sz, int64_t i64, uint8_t b)
{
	assert(b <= strlen(digits));

	s += sz;
	*--s = '\0';

	if (i64 == 0) {
		*--s = digits[0];
		return s;
	}

	for (uint64_t u64 = ABS(i64); u64 > 0; u64 /= b)
		*--s = digits[u64 % b];
	if (i64 < 0)
		*--s = '-';
	return s;
}

char *
strsep(char **sp, char const *sep)
{
	char *s, *token;

	if (*sp == NULL)
		return NULL;

	token = *sp;
	for (s = *sp; strchr(sep, *s) == NULL; s++);

	if (*s == '\0') {
		*sp = NULL;
	}else{
		*s = '\0';
		*sp = s + 1;
	}
	return token;
}

int
put_(char const *s, ...)
{
	va_list ap;

	va_start(ap, s);
	do {
		if (print(s) == -1)
			return -1;
	} while((s = va_arg(ap, char *)) != NULL);
	return 0;
}
