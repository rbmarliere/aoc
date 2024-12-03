#ifndef UTILS_H
#define UTILS_H

#include "linux.h"

#define BUFMAX 512
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

struct int_node {
	struct list_head node;
	int value;
};

void error(const char *fmt, ...);
void debug(const char *fmt, ...);
int cmpint(const void *a, const void *b);
void parseln_int(char *buf, int *a, int *b, int linesz, int width);
void parseln_intnode(char *buf, char *sep, struct list_head *head);

#endif
