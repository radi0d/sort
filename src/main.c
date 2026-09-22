#define ARENA_IMPLEMENTATION
#include "arena.h"

#include "list.h"
#include "sort.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <wctype.h>

#include <sys/stat.h>

void print_usage();
static comp_res_t comp_lines(wstr_t a, wstr_t b);

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

	struct stat st = {};
	if (fstat(fileno(f), &st)) {
		fclose(f);
		perror("stat()");
		return 1;
	}
	const size_t fsize = (size_t) st.st_size;

	arena_t *a = arena_new((fsize + 1) * sizeof(wchar_t));

	list_t *l = list_new();
	if (!l) {
		arena_free(a);
		fclose(f);
		fwprintf(stderr, L"[ERR] Allocation error\n");
		return 1;
	}

	while (true) {
		aptr_t p = arena_ptr(a);
		const size_t taken = p / sizeof(wchar_t);

		wchar_t *line = (wchar_t *) arena_alloc(a, (fsize - taken) *
		                                           sizeof(wchar_t));
		if (!line) {
			list_free(l);
			arena_free(a);
			fclose(f);
			fwprintf(stderr, L"[ERR] Allocation error\n");
			return 1;
		}

		if (!fgetws(line, (int) (fsize - taken), f))
			break;

		const size_t len = wcslen(line);

		// empty line
		if (1 == len) {
			arena_restore(a, p);
			continue;
		}

		arena_restore(a, p);
		if (!arena_alloc(a, len * sizeof(wchar_t))) {
			list_free(l);
			arena_free(a);
			fclose(f);
			fwprintf(stderr, L"[ERR] Allocation error\n");
			return 1;
		}

		if (list_append(l, line, len)) {
			list_free(l);
			arena_free(a);
			fclose(f);
			fwprintf(stderr, L"[ERR] Allocation error\n");
			return 1;
		}
	}

	sort(l, comp_lines);

	for (size_t i = 0; i < l->len; i++)
		wprintf(L"%.*ls", L(l)[i].len, L(l)[i].str);

	list_free(l);
	arena_free(a);
	fclose(f);

	return 0;
}

void
print_usage()
{
	printf("Usage: sort <file>\n");
}

static comp_res_t
comp_lines(wstr_t a, wstr_t b)
{
	assert(a.str);
	assert(b.str);

	const size_t len_a = a.len;
	const size_t len_b = b.len;

	const wchar_t *as = a.str;
	const wchar_t *bs = b.str;

	size_t ptr_a = 0, ptr_b = 0;
	while (ptr_a < len_a && ptr_b < len_b) {
		if (iswpunct((wint_t) as[ptr_a]) ||
		    iswspace((wint_t) as[ptr_a])) {
			ptr_a++;
			continue;
		}

		if (iswpunct((wint_t) bs[ptr_b]) ||
		    iswspace((wint_t) bs[ptr_b])) {
			ptr_b++;
			continue;
		}

		if (towlower((wint_t) as[ptr_a]) <
		    towlower((wint_t) bs[ptr_b]))
			return LESS_THAN;
		if (towlower((wint_t) as[ptr_a]) >
		    towlower((wint_t) bs[ptr_b]))
			return GREATER_THAN;

		ptr_a++;
		ptr_b++;
	}

	if (ptr_a < len_a && ptr_b == len_b)
		for (; ptr_a < len_a; ptr_a++)
			if (!iswpunct((wint_t) as[ptr_a]) &&
			    !iswspace((wint_t) as[ptr_a]))
				return GREATER_THAN;
	if (ptr_a == len_a && ptr_b < len_b)
		for (; ptr_b < len_b; ptr_b++)
			if (!iswpunct((wint_t) bs[ptr_b]) &&
			    !iswspace((wint_t) bs[ptr_b]))
				return LESS_THAN;

	return EQUAL;
}
