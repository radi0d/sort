#pragma once

#include <stddef.h>
#include <wchar.h>

#define L(x) ((x)->buf)

typedef struct list {
	wchar_t **buf;
	size_t cap;
	size_t len;
} list_t;

list_t *list_new(void);
void list_free(list_t *l);

int list_append(list_t *l, wchar_t *line);
