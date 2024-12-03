#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

#define FILENAME "data/2024/2.txt"

int main(void)
{
	FILE *fd;
	char buf[BUFMAX];
	int i, prev, delta, increasing, cur, safe, unsafe;

	fd = fopen(FILENAME, "r");
	if (!fd)
		error("fopen");

	fseek(fd, 0, SEEK_SET);
	i = 0;
	cur = 0;
	safe = 0;
	unsafe = 0;

	while (fgets(buf, sizeof(buf), fd)) {
		struct list_head head;
		INIT_LIST_HEAD(&head);

		parseln_intnode(buf, " ", &head);

		increasing = -1;
		prev = -1;
		struct int_node *pos;

		list_for_each_entry(pos, &head, node) {
			debug("value = %d", pos->value);

			if (prev == -1) {
				debug("prev == -1, setting %d", pos->value);
				prev = pos->value;
				continue;
			}

			if (prev == pos->value) {
				debug("UNSAFE: no increment");
				++unsafe;
				break;
			}

			if (increasing == -1) {
				increasing = prev < pos->value;
				debug("increasing? %d", increasing);
			} else if (increasing == 0 && prev < pos->value) {
				debug("UNSAFE: found decrement while increasing");
				++unsafe;
				break;
			} else if (increasing == 1 && prev > pos->value) {
				debug("UNSAFE: found increment while decreasing");
				++unsafe;
				break;
			}

			delta = abs(prev - pos->value);
			if (delta > 3) {
				debug("UNSAFE: delta is greater than 3 abs(%d - %d)", prev, pos->value);
				++unsafe;
				break;
			}

			prev = pos->value;
		}

		if (cur == unsafe) {
			debug("SAFE!");
			++safe;
		}

		cur = unsafe;
		debug("safe (%d) unsafe (%d) cur (%d)", safe, unsafe, cur);

		++i;
	}

	printf("%d\n", safe);

	fclose(fd);
}
