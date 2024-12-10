#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

#define FILENAME "data/2024/1.txt"
#define LINE_SIZE 13
#define ELEM_SIZE 5

int main(void)
{
	FILE *fd;
	int i, j, count, numlines, distance, simil;
	char buf[LINE_SIZE + 2]; /* include \n\0 */

	fd = fopen(FILENAME, "r");
	if (!fd)
		error("fopen");

	numlines = 0;
	while (fgets(buf, sizeof(buf), fd))
		++numlines;
	debug("NUM_LINES = %d", numlines);

	fseek(fd, 0, SEEK_SET);
	int left[numlines], right[numlines];
	i = 0;
	while (fgets(buf, sizeof(buf), fd)) {
		debug("LINE: %s", buf);
		parseln_lr(buf, &left[i], &right[i], LINE_SIZE, ELEM_SIZE);
		++i;
	}
	fclose(fd);

	qsort(left, numlines, sizeof(int), cmpint);
	qsort(right, numlines, sizeof(int), cmpint);
	distance = 0;
	simil = 0;
	for (i = 0; i < numlines; i++) {
		debug("A = %d ", left[i]);
		debug("B = %d", right[i]);
		distance += abs(right[i] - left[i]);
		debug("DISTANCE = %d", distance);

		count = 0;
		for (j = 0; j < numlines; j++)
			if (left[i] == right[j])
				++count;
		simil += left[i] * count;
	}
	printf("DISTANCE = %d", distance);
	printf("SIMILARITY = %d", simil);
}
