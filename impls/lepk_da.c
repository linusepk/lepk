#include "lepk_da.h"

#include <stddef.h>
#include <malloc.h>
#include <assert.h>
#include <string.h> 

typedef unsigned char Lepk__U8;

#define LEPK__HEAD_FROM_DA(da) ((Lepk__DaHeader *) ((Lepk__U8 *) da - sizeof(Lepk__DaHeader)))
#define LEPK__DA_FROM_HEAD(head) ((void *) ((Lepk__U8 *) head + sizeof(Lepk__DaHeader)))
#ifndef LEPK_DA_START_CAP
#define LEPK_DA_START_CAP 8
#endif /* LEPK_DA_START_CAP */

LEPKDAIMPL void *lepk_da_create(unsigned long size) {
	assert(size != 0 && "Size can't be 0.");

	Lepk__DaHeader *head = head = malloc(size + sizeof(Lepk__DaHeader) * LEPK_DA_START_CAP);
	head->count = 0;
	head->cap = LEPK_DA_START_CAP;
	head->size = size;

	return LEPK__DA_FROM_HEAD(head);
}

LEPKDAIMPL void lepk_da_destroy(void *da) {
	assert(da != NULL && "Dynamic array can't be NULL.");
	free(LEPK__HEAD_FROM_DA(da));
}

LEPKDAIMPL unsigned long lepk_da_count(void *da) {
	assert(da != NULL && "Dyanmic array can't be NULL.");
	return LEPK__HEAD_FROM_DA(da)->count;
}

LEPKDAIMPL void lepk__da_insert(void **da, const void *data, unsigned int index) {
	assert(da != NULL && "Dynamic array pointer can't be NULL.");
	assert(*da != NULL && "Dynamic array can't be NULL.");

	Lepk__DaHeader *head = LEPK__HEAD_FROM_DA(*da);

	/* Correction for out of bound. */
	if (index > head->count) {
		index = head->count;
	}

	/* Resize */
	if (head->count == head->cap) {
		head->cap *= 2;
		Lepk__DaHeader *realloced_head = realloc(head, head->cap * head->size + sizeof(Lepk__DaHeader));
		if (realloced_head == NULL) {
			free(head);
			*da = NULL;
			return;
		}
		head = realloced_head;
		*da = LEPK__DA_FROM_HEAD(head);
	}

	Lepk__U8 *ptr_da = *da;

	/* Move everything one block back. */
	memmove(ptr_da + (index + 1) * head->size, ptr_da + index * head->size, (head->count - index + 1) * head->size);
	memcpy(ptr_da + index * head->size, data, head->size);

	head->count++;
}

LEPKDAIMPL void lepk__da_remove(void **da, unsigned int index) {
	assert(da != NULL && "Dynamic array pointer can't be NULL.");
	assert(*da != NULL && "Dynamic array can't be NULL.");

	Lepk__DaHeader *head = LEPK__HEAD_FROM_DA(*da);

	/* Correction for out of bound. */
	if (index > head->count) {
		index = head->count;
	}

	Lepk__U8 *ptr_da = *da;

	/* Resize */
	if (head->count == head->cap / 2 && head->cap != LEPK_DA_START_CAP) {
		head->cap /= 2;
		Lepk__DaHeader *realloced_head = realloc(head, head->cap * head->size + sizeof(Lepk__DaHeader));
		if (realloced_head == NULL) {
			free(head);
			*da = NULL;
			return;
		}
		head = realloced_head;
		*da = LEPK__DA_FROM_HEAD(head);
	}

	memmove(ptr_da + (index) * head->size, ptr_da + (index + 1) * head->size, (head->count - index + 1) * head->size);

	head->count--;
}

LEPKDAIMPL void lepk__da_insert_fast(void **da, const void *data, unsigned int index) {
	assert(da != NULL && "Dynamic array pointer can't be NULL.");
	assert(*da != NULL && "Dynamic array can't be NULL.");

	Lepk__DaHeader *head = LEPK__HEAD_FROM_DA(*da);

	/* Correction for out of bound. */
	if (index > head->count) {
		index = head->count;
	}

	/* Resize */
	if (head->count == head->cap) {
		head->cap *= 2;
		Lepk__DaHeader *realloced_head = realloc(head, head->cap * head->size + sizeof(Lepk__DaHeader));
		if (realloced_head == NULL) {
			free(head);
			*da = NULL;
			return;
		}
		head = realloced_head;
		*da = LEPK__DA_FROM_HEAD(head);
	}

	Lepk__U8 *ptr_da = *da;

	/* Put current item at index at the end. */
	memcpy(ptr_da + head->count * head->size, ptr_da + index * head->size, head->size);
	memcpy(ptr_da + index * head->size, data, head->size);

	head->count++;
}

LEPKDAIMPL void lepk__da_remove_fast(void **da, unsigned int index) {
	assert(da != NULL && "Dynamic array pointer can't be NULL.");
	assert(*da != NULL && "Dynamic array can't be NULL.");

	Lepk__DaHeader *head = LEPK__HEAD_FROM_DA(*da);

	/* Correction for out of bound. */
	if (index > head->count) {
		index = head->count;
	}

	Lepk__U8 *ptr_da = *da;

	/* Resize */
	if (head->count == head->cap / 2 && head->cap != LEPK_DA_START_CAP) {
		head->cap /= 2;
		Lepk__DaHeader *realloced_head = realloc(head, head->cap * head->size + sizeof(Lepk__DaHeader));
		if (realloced_head == NULL) {
			free(head);
			*da = NULL;
			return;
		}
		head = realloced_head;
		*da = LEPK__DA_FROM_HEAD(head);
	}

	memcpy(ptr_da + index * head->size, ptr_da + (head->count - 1) * head->size, head->size);

	head->count--;
}

LEPKDAIMPL void lepk__da_insert_array(void **da, const void *array, unsigned int array_length, unsigned int index) {
	assert(da != NULL && "Dynamic array pointer can't be NULL.");
	assert(*da != NULL && "Dynamic array can't be NULL.");
	assert(array != NULL && "Array can't be NULL.");
	if (array_length == 0) {
		return;
	}

	Lepk__DaHeader *head = LEPK__HEAD_FROM_DA(*da);
	for (unsigned int i = 0; i < array_length; i++) {
		lepk__da_insert(da, (unsigned char *) array + i * head->size, index + i);
	}
}

LEPKDAIMPL void lepk__da_push_array(void **da, const void *array, unsigned int array_length) {
	assert(da != NULL && "Dynamic array pointer can't be NULL.");
	assert(*da != NULL && "Dynamic array can't be NULL.");
	assert(array != NULL && "Array can't be NULL.");
	if (array_length == 0) {
		return;
	}

	Lepk__DaHeader *head = LEPK__HEAD_FROM_DA(*da);
	for (Lepk__U8 i = 0; i < array_length; i++) {
		lepk__da_insert_fast(da, (Lepk__U8 *) array + i * head->size, head->count);
	}
}
