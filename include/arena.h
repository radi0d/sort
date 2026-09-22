#ifndef ARENA_INCLUDE_ARENA_H
#define ARENA_INCLUDE_ARENA_H

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef size_t aptr_t;

typedef struct arena {
	size_t cap;    // Buffer capacity
	aptr_t ptr;    // Allocation pointer
	uint8_t *data; // Buffer
} arena_t;

arena_t *arena_new(size_t size);
void arena_free(arena_t *a);

void *arena_alloc(arena_t *a, size_t size);
void *arena_dup(arena_t *a, const void *src, size_t size);

aptr_t arena_ptr(arena_t *a);
void arena_restore(arena_t *a, aptr_t ptr);

#ifndef ARENA_NORUN

#define ARUN(a, f, ...)                         \
	({                                      \
		aptr_t prev = arena_ptr(a);     \
		auto res = f(a, ##__VA_ARGS__); \
		arena_restore(a, prev);         \
		res;                            \
	})

#define ARUNV(a, f, ...)                        \
	do {                                    \
		aptr_t prev = arena_ptr(a);     \
		f(a, ##__VA_ARGS__);            \
		arena_restore(a, prev);         \
	} while(0)

#endif

#ifdef ARENA_IMPLEMENTATION

// Allocate and initialise a new arena.
arena_t *
arena_new(size_t size)
{
	arena_t *res = (arena_t *) calloc(1, sizeof(arena_t));
	if (!res)
		return NULL;

	res->cap = size;
	res->ptr = 0;

	res->data = (uint8_t *) calloc(size, sizeof(uint8_t));
	if (!res->data) {
		free(res);
		return NULL;
	}

	return res;
}

// Deallocate the arena.
void
arena_free(arena_t *a)
{
	assert(a);
	assert(a->data);

	free(a->data);
	free(a);
}

// Allocate a chunk.
void *
arena_alloc(arena_t *a, size_t size)
{
	assert(a);
	assert(a->data);

	if (a->ptr + size > a->cap)
		return NULL;

	void *ret = &a->data[a->ptr];
	a->ptr += size;

	return ret;
}

// Allocate and copy.
void *
arena_dup(arena_t *a, const void *src, size_t size)
{
	assert(a);
	assert(a->data);

	void *dst = arena_alloc(a, size);
	if (!dst)
		return NULL;

	memcpy(dst, src, size);

	return dst;
}

// Get the current allocation pointer.
aptr_t
arena_ptr(arena_t *a)
{
	assert(a);
	assert(a->data);

	return a->ptr;
}

// Set the allocation pointer. Paired with arena_ptr(),
// may be useful for cleaning a calee's allocated memory.
void
arena_restore(arena_t *a, aptr_t ptr)
{
	assert(a);
	assert(a->data);

	assert(ptr <= a->cap);

	a->ptr = ptr;
}

#endif

#endif
