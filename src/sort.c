#include "sort.h"

#include <assert.h>

#define PAR(x) (((x) - 1) / 2)
#define LEFT(x) ((x) * 2 + 1)
#define RIGHT(x) ((x) * 2 + 2)

static void swap(wstr_t *a, wstr_t *b);

void
sort(list_t *l, comp_t comp)
{
	assert(l);
	assert(l->buf);
	assert(l->cap >= l->len);

	size_t start = l->len / 2;
	size_t end = l->len;
	while (end > 1) {
		if (start > 0) {
			start--;
		} else {
			end--;
			swap(&L(l)[end], &L(l)[0]);
		}

		size_t root = start;
		bool end_loop = false;
		while (LEFT(root) < end && !end_loop) {
			size_t child = LEFT(root);
			if (child + 1 < end) {
				switch (comp(L(l)[child], L(l)[child + 1])) {
				case LESS_THAN:
					child++;
				case EQUAL:
				case GREATER_THAN:
					break;
				default:
					assert("Unreachable" && 0);
				}
			}

			switch (comp(L(l)[root], L(l)[child])) {
			case LESS_THAN:
				swap(&L(l)[root], &L(l)[child]);
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
swap(wstr_t *a, wstr_t *b)
{
	assert(a);
	assert(b);

	wstr_t t = *a;
	*a = *b;
	*b = t;
}
