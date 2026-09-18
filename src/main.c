#include "list.h"
#include "sort.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <wctype.h>

#define LINE_BUF 1024

void print_usage();
static comp_res_t comp_lines(const wchar_t *a, const wchar_t *b);

int
main(int argc, char *argv[])
{
	if (2 != argc) {
		print_usage();
		return 1;
	}

	setlocale(LC_ALL, "");

	FILE *f = fopen(argv[1], "r");
	if (!f) {
		perror("open()");
		return 1;
	}

	list_t *l = list_new();
	if (!l) {
		fprintf(stderr, "[ERR] Allocation error\n");
		return 1;
	}

	while (true) {
		wchar_t *line = (wchar_t *) calloc(LINE_BUF, sizeof(wchar_t));
		if (!fgetws(line, LINE_BUF, f))
			break;

		// empty line
		if (1 == wcslen(line)) {
			free(line);
			continue;
		}

		if (list_append(l, line)) {
			fprintf(stderr, "[ERR] Allocation error\n");
			return 1;
		}
	}

	sort(L(l), l->len, comp_lines);

	for (size_t i = 0; i < l->len; i++) {
		wprintf(L"%ls", L(l)[i]);
	}

	list_free(l);
	fclose(f);

	return 0;
}

void
print_usage()
{
	printf("Usage: sort <file>\n");
}

static comp_res_t
comp_lines(const wchar_t *a, const wchar_t *b)
{
	assert(a);
	assert(b);

	const size_t len_a = wcslen(a);
	const size_t len_b = wcslen(b);

	size_t ptr_a = 0, ptr_b = 0;
	while (ptr_a < len_a && ptr_b < len_b) {
		if (iswpunct((wint_t) a[ptr_a]) || iswspace((wint_t) a[ptr_a])) {
			ptr_a++;
			continue;
		}

		if (iswpunct((wint_t) b[ptr_b]) || iswspace((wint_t) b[ptr_b])) {
			ptr_b++;
			continue;
		}

		if (towlower((wint_t) a[ptr_a]) < towlower((wint_t) b[ptr_b]))
			return LESS_THAN;
		if (towlower((wint_t) a[ptr_a]) > towlower((wint_t) b[ptr_b]))
			return GREATER_THAN;

		ptr_a++;
		ptr_b++;
	}

	if (ptr_a < len_a && ptr_b == len_b)
		for (; ptr_a < len_a; ptr_a++)
			if (!iswpunct((wint_t) a[ptr_a]) && !iswspace((wint_t) a[ptr_a]))
				return GREATER_THAN;
	if (ptr_a == len_a && ptr_b < len_b)
		for (; ptr_b < len_b; ptr_b++)
			if (!iswpunct((wint_t) b[ptr_b]) && !iswspace((wint_t) b[ptr_b]))
				return LESS_THAN;

	return EQUAL;
}
