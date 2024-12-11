#include <regex.h>
#include <stdio.h>
#include "utils.h"
#include <stdlib.h>

struct mul_node {
	struct list_head node;
	int v1;
	int v2;
	int mul;
};

char *get_regerror(int err, regex_t *comp)
{
	int len = regerror(err, comp, NULL, 0);
	char *buf = malloc(len);
	regerror(err, comp, buf, len);
	return buf;
}

int main(int argc, char *argv[])
{
	FILE *fd;
	regex_t regex;
	regmatch_t pmatch[3];
	regoff_t len;
	int ret, total = 0;
	char buf[BUFMAX], number_buf[32];
	char *s;
	struct list_head mul_list;
	struct mul_node *i, *tmp;

	fd = parse_args(argc, argv);

	ret = regcomp(&regex, "mul\\(([0-9]+),([0-9]+)\\)", REG_EXTENDED);
	if (ret)
		error("regcomp: %s", get_regerror(ret, &regex));

	INIT_LIST_HEAD(&mul_list);

	while (fgets(buf, sizeof(buf), fd)) {
		s = buf;
		while (1) {
			/* go through each line's matches */
			ret = regexec(&regex, s, 3, pmatch, 0);
			if (ret) {
				debug("regexec: %s", get_regerror(ret, &regex));
				break;
			}

			struct mul_node *new = malloc(sizeof(struct mul_node));
			if (!new)
				error("mul_node malloc");

			len = pmatch[0].rm_eo - pmatch[0].rm_so;
			debug("\n%.*s", len, s + pmatch[0].rm_so);

			len = pmatch[1].rm_eo - pmatch[1].rm_so;
			snprintf(number_buf, sizeof(number_buf), "%.*s", len,
				 s + pmatch[1].rm_so);
			debug("%s", number_buf);
			new->v1 = atoi(number_buf);

			len = pmatch[2].rm_eo - pmatch[2].rm_so;
			snprintf(number_buf, sizeof(number_buf), "%.*s", len,
				 s + pmatch[2].rm_so);
			debug("%s", number_buf);
			new->v2 = atoi(number_buf);

			new->mul = new->v1 *new->v2;
			debug("%d", new->mul);

			list_add_tail(&new->node, &mul_list);
			s += pmatch[0].rm_eo;
		}
	}

	list_for_each_entry_safe(i, tmp, &mul_list, node) {
		total += i->mul;
		list_del(&i->node);
		free(i);
	}

	printf("%d\n", total);

	fclose(fd);
	regfree(&regex);
}
