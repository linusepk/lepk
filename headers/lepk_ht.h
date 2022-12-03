/* Version: 1.0 */

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
 * Hash table.
 *
 * Add:
 *     #define LEPK_HT_IMPLEMENTATION
 * in one C or C++ file, before #include "lepk_ht.h", to create the implementation.
 *
 * If LEPK_HT_STATIS is defined the implementation will be local to a single file only.
 */

/*
 * TODO: ACTUALLY MAKE THE THING WORK.
 * Currently, when getting an entry, if it collided and the object at the original collision point has been removed a dead entry is returned instead of the actual entry.
 */

#ifndef LEPK_HT_H
#define LEPK_HT_H

#ifndef LEPK_HT_STATIC
#define LEPKHT extern
#else /* LEPK_HT_STATIC */
#define LEPKHT static
#endif /* LEPK_HT_STATIC */

/* Hash Table. */
typedef struct LepkHt LepkHt;
/* Hasing function. */
typedef unsigned long (*LepkHtHash)(const void *key, unsigned long size);
/* Compare funciton. */
typedef int (*LepkHtCompare)(const void *a, const void *b, unsigned long size);

/* Create a hash table. */
LEPKHT LepkHt *lepk_ht_create(LepkHtHash hash, LepkHtCompare compare, unsigned long key_size, unsigned long data_size);
/* Destroy a hash table. */
LEPKHT void lepk_ht_destroy(LepkHt *table);

/* Retrieve item count from hash table. */
LEPKHT unsigned long lepk_ht_count(const LepkHt *table);

/* Set the pair in hash table. */
LEPKHT void lepk__ht_set(LepkHt *table, const void *key, const void *data);
/* Get pair from hash table. */
LEPKHT void lepk__ht_get(LepkHt *table, const void *key, void *output);
/* Remove pair from hash table. */
LEPKHT void lepk__ht_remove(LepkHt *table, const void *key, void *output);

/* Pre-written hashing function for strings. */
LEPKHT unsigned long lepk_ht_hash_string(const void *key, unsigned long size);
/* Pre-written generic hashing function for any type of data structure. */
LEPKHT unsigned long lepk_ht_hash_generic(const void *key, unsigned long size);
/* Pre-written compare function for strings. */
LEPKHT int lepk_ht_compare_string(const void *a, const void *b, unsigned long size);
/* Pre-written generic compare function for any type of data structure. */
LEPKHT int lepk_ht_compare_generic(const void *a, const void *b, unsigned long size);

#define lepk_ht_set(table, key, data) do { __typeof__(key) lepk__ht_temp_key = key; __typeof__(data) lepk__ht_temp_data = data; lepk__ht_set(table, &lepk__ht_temp_key, &lepk__ht_temp_data); } while (0)
#define lepk_ht_get(table, key, output) do { __typeof__(key) lepk__ht_temp_key = key; lepk__ht_get(table, &lepk__ht_temp_key, output); } while (0)
#define lepk_ht_remove(table, key, output) do { __typeof__(key) lepk__ht_temp_key = key; lepk__ht_remove(table, &lepk__ht_temp_key, output); } while (0)

#ifdef LEPK_HT_TEST

static void lepk_ht_test(void) {
	LepkHt *table = lepk_ht_create(lepk_ht_hash_string, lepk_ht_compare_string, sizeof(const char *), sizeof(int));
	lepk_ht_set(table, "key", 8);
	int output = 0;
	lepk_ht_get(table, "key", &output);
	assert(output == 8 && "lepk_ht failed.");
	lepk_ht_destroy(table);
}

#endif /* LEPK_HT_TEST */

#endif /* LEPK_HT_H */
