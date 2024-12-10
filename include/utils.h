#ifndef UTILS_H
#define UTILS_H

#include "linux.h"

#define BUFMAX 512
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

void error(const char *fmt, ...);
void debug(const char *fmt, ...);
int cmpint(const void *a, const void *b);
void parseln_lr(char *buf, int *a, int *b, int linesz, int width);

typedef void (*parseln_handler)(const int value, void *ctx);
int parseln(char *buf, char *sep, parseln_handler handler, void *ctx);

#endif
