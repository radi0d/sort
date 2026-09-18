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

	l->buf = (wchar_t **) calloc(1, sizeof(wchar_t *));
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

	for (size_t i = 0; i < l->len; i++)
		free(L(l)[i]);

	free(l->buf);
	free(l);
}

int
list_append(list_t *l, wchar_t *line)
{
	assert(l);
	assert(l->buf);
	assert(line);

	size_t tcap = 1;
	size_t tlen = l->len + 1;
	while (tcap < tlen)
		tcap *= 2;

	wchar_t **tbuf = (wchar_t **) calloc(tcap, sizeof(wchar_t *));
	if (!tbuf)
		return -1;

	memcpy(tbuf, l->buf, l->len * sizeof(wchar_t *));
	free(l->buf);
	tbuf[l->len] = line;

	l->buf = tbuf;
	l->cap = tcap;
	l->len = tlen;

	return 0;
}
