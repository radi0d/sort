#pragma once

#include <stddef.h>
#include <wchar.h>

typedef enum comp_result {
	LESS_THAN,
	EQUAL,
	GREATER_THAN,
} comp_res_t;

typedef comp_res_t (*comp_t)(const wchar_t *a, const wchar_t *b);

void sort(wchar_t **lines, size_t len, comp_t comp);
