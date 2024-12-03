#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "utils.h"

void error(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	printf("\n");
	exit(EXIT_FAILURE);
}

void debug(const char *fmt, ...)
{
#ifdef DEBUG
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	printf("\n");
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

void parseln_intnode(char *buf, char *sep, struct list_head *head)
{
	debug("parseln_intnode");
	char tmp[BUFMAX], c;
	int i = 0;

	while ((c = *buf++)) {
		if (c == *sep) {
			tmp[i] = 0;
		} else {
			tmp[i] = c;
		}

		if (tmp[i] == 0 || tmp[i] == '\n') {
			i = -1;
			struct int_node *new = malloc(sizeof(struct int_node));
			new->value = atoi(tmp);
			list_add_tail(&new->node, head);
			debug("new = %d", new->value);
			memset(tmp, 0, sizeof(tmp));
		}

		++i;
	}
}
