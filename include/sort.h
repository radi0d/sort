#pragma once

#include "list.h"

#include <stddef.h>
#include <wchar.h>

typedef enum comp_result {
	LESS_THAN,
	EQUAL,
	GREATER_THAN,
} comp_res_t;

typedef comp_res_t (*comp_t)(wstr_t a, wstr_t b);

void sort(list_t *l, comp_t comp);
