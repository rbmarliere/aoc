#include "linux.h"
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

struct int_node {
	struct list_head node;
	int value;
	int trend;
};

int is_int_node_safe(struct int_node *cur)
{
	int delta;
	struct int_node *prev;

	prev = list_prev_entry(cur, node);
	debug("prev = %d", prev->value);

	if (prev->value == cur->value) {
		debug("UNSAFE: no change");
		return 0;
	}

	delta = abs(prev->value - cur->value);
	if (delta > 3) {
		debug("UNSAFE: delta is greater than 3 abs(%d - %d)",
		      prev->value, cur->value);
		return 0;
	}

	cur->trend = prev->value < cur->value;
	debug("cur->trend = %d", cur->trend);

	if (cur->trend == 0 && prev->trend == 1) {
		debug("UNSAFE: found increment while decreasing");
		return 0;
	} else if (cur->trend == 1 && prev->trend == 0) {
		debug("UNSAFE: found decrement while increasing");
		return 0;
	}

	return 1;
}

int is_line_safe(struct list_head *head, int delete)
{
	debug("is_line_safe (delete = %d)", delete);
	int count = 0;
	struct list_head *i, *tmp;
	struct int_node *first, *cur, *prev;
	first = list_first_entry(head, struct int_node, node);

	list_for_each_safe(i, tmp, head) {
		++count;

		cur = list_entry(i, struct int_node, node);
		prev = list_prev_entry(cur, node);
		debug(";cur = %d :: trend = %d", cur->value, cur->trend);

		/* reset trend in case we're deleting */
		cur->trend = -1;

		if (cur == first)
			continue;

		if (!is_int_node_safe(cur)) {
			if (!delete)
				return 0;

			prev->trend = -1;

			debug("removing cur...");
			list_del(i);
			if (is_line_safe(head, 0))
				return 1;
			list_add(i, i->prev);

			debug("removing prev...");
			list_del(i->prev);
			if (is_line_safe(head, 0))
				return 1;
			list_add(&prev->node, i->prev);

			/* if this is the second element, "prev" == "first" */
			if (count == 2)
				continue;

			debug("removing first...");
			list_del(&first->node);
			if (!is_line_safe(head, 0))
				return 0;
		}

		debug("%d", count);
	}

	return 1;
}

void int_node_add(const int value, void *ctx)
{
	struct list_head *head = (struct list_head *)ctx;
	struct int_node *new = malloc(sizeof(struct int_node));
	if (!new)
		error("intnode_add");
	new->value = value;
	new->trend = -1;
	list_add_tail(&new->node, head);
}

int main(int argc, char *argv[])
{
	FILE *fd;
	char buf[BUFMAX];
	int safe, unsafe;

	fd = parse_args(argc, argv);

	safe = 0;
	unsafe = 0;

	struct list_head head;
	while (fgets(buf, sizeof(buf), fd)) {
		INIT_LIST_HEAD(&head);

		parseln(buf, " ", int_node_add, &head);
		if (is_line_safe(&head, 1))
			++safe;
		else
			++unsafe;

		debug("safe (%d) unsafe (%d)", safe, unsafe);
		debug("");
	}

	printf("%d\n", safe);

	fclose(fd);
}
