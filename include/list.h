#pragma once

#include <stddef.h>
#include <wchar.h>

#define L(x) ((x)->buf)

typedef struct wide_string {
	wchar_t *str;
	size_t len;
} wstr_t;

typedef struct list {
	wstr_t *buf;
	size_t cap;
	size_t len;
} list_t;

list_t *list_new(void);
void list_free(list_t *l);

int list_append(list_t *l, wchar_t *line, size_t len);
