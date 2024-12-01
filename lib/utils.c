#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void debug(const char *fmt, ...)
{
#ifdef DEBUG
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
#else
	(void)fmt;
#endif
}

int cmpint(const void *a, const void *b)
{
	int ia = *(const int *)a;
	int ib = *(const int *)b;
	return ia > ib;
}

void parseln_int(char *buf, int *a, int *b, int linesz, int width)
{
	debug("parseln_int");
	char sub[width + 1]; /* + \0 */
	int b_start = linesz - width;
	snprintf(sub, sizeof(sub), "%.*s", width, buf);
	*a = atoi(sub);
	debug("a = %d", a);
	snprintf(sub, sizeof(sub), "%.*s", width, buf + b_start);
	*b = atoi(sub);
	debug("b = %d", b);
}
