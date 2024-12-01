#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* #define DEBUG */
#define FILENAME "input.txt"
#define LINE_SIZE 13
#define ELEM_SIZE 5
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

int cmpint(const void *a, const void *b)
{
	int ia = *(int *)a;
	int ib = *(int *)b;
	return ia > ib;
}

void parseln(char *buf, int *a, int *b)
{
	char sub[ELEM_SIZE+1];
	int b_start = LINE_SIZE - ELEM_SIZE;
	snprintf(sub, sizeof(sub), "%.*s", ELEM_SIZE, buf);
	*a = atoi(sub);
	snprintf(sub, sizeof(sub), "%.*s", ELEM_SIZE, buf+b_start);
	*b = atoi(sub);
}

void debug(const char *fmt, ...)
{
#ifdef DEBUG
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
#endif
}

int main(void)
{
	FILE *fd;
	int i, j, ret, count, numlines, distance, simil;
	char buf[LINE_SIZE+2]; /* include \n\0 */

	fd = fopen(FILENAME, "r");
	if (!fd) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	numlines = 0;
	while (fgets(buf, sizeof(buf), fd)) {
		++numlines;
	}
	debug("\nNUM_LINES = %d\n", numlines);

	fseek(fd, 0, SEEK_SET);
	int left[numlines], right[numlines];
	i = 0;
	while (fgets(buf, sizeof(buf), fd)) {
		debug("\nLINE: %s\n", buf);
		parseln(buf, &left[i], &right[i]);
		++i;
	}
	fclose(fd);

	qsort(left, numlines, sizeof(int), cmpint);
	qsort(right, numlines, sizeof(int), cmpint);
	distance = 0;
	simil = 0;
	for (i=0; i<numlines; i++) {
		debug("A = %d ", left[i]);
		debug("B = %d\n", right[i]);
		distance += abs(right[i] - left[i]);
		debug("DISTANCE = %d\n", distance);

		count = 0;
		for (j=0; j<numlines; j++)
			if (left[i] == right[j])
				++count;
		simil += left[i] * count;
	}
	printf("DISTANCE = %d\n", distance);
	printf("SIMILARITY = %d\n", simil);
}
