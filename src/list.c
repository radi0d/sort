#include "list.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

list_t *
list_new(void)
{
	list_t *l = (list_t *) calloc(1, sizeof(list_t));
	if (!l)
		return NULL;

	l->buf = (wstr_t *) calloc(1, sizeof(wstr_t));
	if (!l->buf) {
		free(l);
		return NULL;
	}

	l->cap = 1;
	l->len = 0;

	return l;
}

void
list_free(list_t *l)
{
	assert(l);
	assert(l->buf);
	assert(l->cap >= l->len);

	free(l->buf);
	free(l);
}

int
list_append(list_t *l, wchar_t *line, size_t len)
{
	assert(l);
	assert(l->buf);
	assert(l->cap >= l->len);
	assert(line);

	size_t tcap = 1;
	size_t tlen = l->len + 1;
	while (tcap < tlen)
		tcap *= 2;

	wstr_t *tbuf = (wstr_t *) calloc(tcap, sizeof(wstr_t));
	if (!tbuf)
		return -1;

	memcpy(tbuf, l->buf, l->len * sizeof(wstr_t));
	free(l->buf);

	tbuf[l->len].str = line;
	tbuf[l->len].len = len;

	l->buf = tbuf;
	l->cap = tcap;
	l->len = tlen;

	return 0;
}
