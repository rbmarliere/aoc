#ifndef UTILS_H
#define UTILS_H

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

void debug(const char *fmt, ...);
int cmpint(const void *a, const void *b);
void parseln_int(char *buf, int *a, int *b, int linesz, int width);

#endif
