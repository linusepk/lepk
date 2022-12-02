/* Version: 1.1 */

/*
 * MIT License
 * 
 * Copyright (c) 2022 Linus Erik Pontus Kåreblom
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * Dynamic array, single header library.
 * Add:
 *     #define LEPK_DA_IMPLEMENTATION
 * in one C or C++ file, before #include "lepk_da.h", to create the implementation.
 *
 * If LEPK_DA_STATIC is defined the implementation will be local to a single file only.
 *
 * Use:
 *     #define LEPK_DA_START_CAP [int]
 *  to define starting capacity of dynamic arrays.
 */

/*
 * === Documentation ===
 * Usage:
 * int *da = lepk_da_create(sizeof(int));
 * lepk_da_push(da, 8);
 * lepk_da_insert(da, 8, 0);
 * for (unsigned long i = 0; i < lepk_da_count(da); i++) {
 *     printf("%d\n", da[i]);
 * }
 * lepk_da_destroy(da);
 */

#ifndef LEPK_DA_H
#define LEPK_DA_H

#ifdef LEPK_DA_STATIC
#define LEPKDA static
#define LEPKDAIMPL static
#else /* LEPK_DA_STATIC */
#define LEPKDA extern
#define LEPKDAIMPL
#endif /* LEPK_DA_STATIC */

/*
 * Data needed for dynamic array operations.
 * Stored before dynamic array returned to user.
 */
typedef struct Lepk__DaHeader Lepk__DaHeader;
struct Lepk__DaHeader {
	/* Current amount of items stored. */
	unsigned long count;
	/* Max amount of itmes stored. */
	unsigned long cap;
	/* Size of an item. */
	unsigned long size;
};

/* Create a dynamic array. */
LEPKDA void *lepk_da_create(unsigned long size);
/* Free dynamic array. */
LEPKDA void lepk_da_destroy(void *da);
/* Get current amount of items stored in dynamic array. */
LEPKDA unsigned long lepk_da_count(void *da);
/* Insert data into dynamic array at index, preserving insertion order. */
LEPKDA void lepk__da_insert(void **da, const void *data, unsigned int index);
/* Remove item from dynamic array at index, preserving insertion order. */
LEPKDA void lepk__da_remove(void **da, unsigned int index, void *output);
/* Insert data into dynamic array at index, destroying insertion order. Copy data from index to output. */
LEPKDA void lepk__da_insert_fast(void **da, const void *data, unsigned int index);
/* Remove item from dynamic array at index, destroying insertion order. Copy data from index to output. */
LEPKDA void lepk__da_remove_fast(void **da, unsigned int index, void *output);
/* Insert a whole array at a time. */
LEPKDA void lepk__da_insert_array(void **da, const void *array, unsigned int array_length, unsigned int index);
/* Push a whole array at a time to the end of the dyanmic array. */
LEPKDA void lepk__da_push_array(void **da, const void *array, unsigned int array_length);

#define lepk_da_insert(da, data, index) do {__typeof__((data)) lepk__temp_data = (data); lepk__da_insert((void **) &(da), &lepk__temp_data, (index));} while (0)
#define lepk_da_remove(da, index, output) do {lepk__da_remove((void **) &(da), (index), (output));} while (0)
#define lepk_da_insert_fast(da, data, index) do {__typeof__((data)) lepk__temp_data = (data); lepk__da_insert_fast((void **) &(da), &lepk__temp_data, (index));} while (0)
#define lepk_da_remove_fast(da, index, output) do {lepk__da_remove_fast((void **) &(da), (index), (output));} while (0)
/* Insert data at end of dynamic array. */
#define lepk_da_push(da, data) lepk_da_insert_fast((da), (data), lepk_da_count((da)))
/* Remove last item of dynamic array. */
#define lepk_da_pop(da, output) lepk_da_remove_fast((da), lepk_da_count((da)) - 1, (output))
#define lepk_da_insert_array(da, array, array_length, index) do {lepk__da_insert_array((void **) &(da), (array), (array_length), (index));} while (0)
#define lepk_da_push_array(da, array, array_length) do {lepk__da_push_array((void **) &(da), (array), (array_length));} while (0)

#ifdef LEPK_DA_TEST

#include <string.h>
#include <assert.h>
#include <stdio.h>

static void lepk_da_test(void) {
	int *da = lepk_da_create(sizeof(int));
	assert(da != NULL && "lepk_da_create failed.");
	int out;

	{
		int expected[2] = { 4, 3 };
		lepk_da_push(da, 4);
		lepk_da_push(da, 3);
		assert(memcmp(da, expected, 2 * sizeof(int)) == 0 && "lepk_da_push failed.");
	}
	{
		int expected[3] = { 1, 4, 3 };
		lepk_da_insert(da, 1, 0);
		assert(memcmp(da, expected, 3 * sizeof(int)) == 0 && "lepk_da_insert failed.");
	}
	{
		int expected[4] = { 2, 4, 3, 1 };
		lepk_da_insert_fast(da, 2, 0);
		assert(memcmp(da, expected, 4 * sizeof(int)) == 0 && "lepk_da_insert_fast failed.");
	}
	{
		int expected[3] = { 4, 3, 1 };
		lepk_da_remove(da, 0, &out);
		assert(memcmp(da, expected, 3 * sizeof(int)) == 0 && "lepk_da_remove failed.");
		assert(out == 2 && "lepk_da_remove output failed.");
	}
	{
		int expected[2] = { 1, 3 };
		lepk_da_remove_fast(da, 0, &out);
		assert(memcmp(da, expected, 2 * sizeof(int)) == 0 && "lepk_da_remove_fast failed.");
		assert(out == 4 && "lepk_da_remove_fast output failed.");
	}
	{
		int expected[1] = { 1 };
		lepk_da_pop(da, &out);
		assert(memcmp(da, expected, 1 * sizeof(int)) == 0 && "lepk_da_pop failed.");
		assert(out == 3 && "lepk_da_pop output failed.");
	}
	{
		int expected[3] = { 2, 3, 1 };
		int arr[2] = { 2, 3 };
		lepk_da_insert_array(da, arr, 2, 0);
		assert(memcmp(da, expected, 3 * sizeof(int)) == 0 && "lepk_da_insert_array failed.");
	}
	{
		int expected[6] = { 2, 3, 1, 4, 5, 6 };
		int arr[3] = { 4, 5, 6 };
		lepk_da_push_array(da, arr, 3);
		assert(memcmp(da, expected, 6 * sizeof(int)) == 0 && "lepk_da_push_array failed.");
	}

	assert(lepk_da_count(da) == 6 && "lepk_da_count failed.");
	lepk_da_destroy(da);
}

#endif /* LEPK_DA_TEST */
#ifdef LEPK_DA_IMPLEMENTATION
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
	if (head->count + 1 == head->cap) {
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

LEPKDAIMPL void lepk__da_remove(void **da, unsigned int index, void *output) {
	assert(da != NULL && "Dynamic array pointer can't be NULL.");
	assert(*da != NULL && "Dynamic array can't be NULL.");

	Lepk__DaHeader *head = LEPK__HEAD_FROM_DA(*da);

	/* Correction for out of bound. */
	if (index > head->count) {
		index = head->count;
	}

	Lepk__U8 *ptr_da = *da;

	/* Copy removed data into output. */
	if (output != NULL) {
		memcpy(output, ptr_da + index * head->size, head->size);
	}

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

LEPKDAIMPL void lepk__da_remove_fast(void **da, unsigned int index, void *output) {
	assert(da != NULL && "Dynamic array pointer can't be NULL.");
	assert(*da != NULL && "Dynamic array can't be NULL.");

	Lepk__DaHeader *head = LEPK__HEAD_FROM_DA(*da);

	/* Correction for out of bound. */
	if (index > head->count) {
		index = head->count;
	}

	Lepk__U8 *ptr_da = *da;

	/* Copy removed data into output. */
	if (output != NULL) {
		memcpy(output, ptr_da + (index) * head->size, head->size);
	}

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
#endif /*LEPK_DA_IMPLEMENTATION*/
#endif /* LEPK_DA_H */
