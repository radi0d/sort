#include "sort.h"

#include <assert.h>

#define PAR(x) (((x) - 1) / 2)
#define LEFT(x) ((x) * 2 + 1)
#define RIGHT(x) ((x) * 2 + 2)

static void swap(wchar_t **a, wchar_t **b);

void
sort(wchar_t **lines, size_t len, comp_t comp)
{
	assert(lines);
	assert(comp);

	size_t start = len / 2;
	size_t end = len;
	while (end > 1) {
		if (start > 0) {
			start--;
		} else {
			end--;
			swap(&lines[end], &lines[0]);
		}

		size_t root = start;
		bool end_loop = false;
		while (LEFT(root) < end && !end_loop) {
			size_t child = LEFT(root);
			if (child + 1 < end) {
				switch (comp(lines[child], lines[child + 1])) {
				case LESS_THAN:
					child++;
				case EQUAL:
				case GREATER_THAN:
					break;
				default:
					assert("Unreachable" && 0);
				}
			}

			switch (comp(lines[root], lines[child])) {
			case LESS_THAN:
				swap(&lines[root], &lines[child]);
				root = child;
				break;
			case EQUAL:
			case GREATER_THAN:
				end_loop = true;
				break;
			default:
				assert("Unreachable" && 0);
			}
		}
	}
}

static void
swap(wchar_t **a, wchar_t **b)
{
	assert(a);
	assert(b);

	wchar_t *t = *a;
	*a = *b;
	*b = t;
}
