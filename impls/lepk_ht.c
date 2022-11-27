#include "lepk_ht.h"

#include <malloc.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#undef LEPKHT
#ifndef LEPK_HT_STATIC
#define LEPKHT
#else /* LEPK_HT_STATIC */
#define LEPKHT static
#endif /* LEPK_HT_STATIC */

#define LEPK_HT_MAX_LOAD 0.75f

typedef struct Lepk__HtEntry {
	void *key;
	void *data;
	size_t hash;
	bool dead;
} Lepk__HtEntry;

struct LepkHt {
	LepkHtHash hash;
	LepkHtCompare compare;

	size_t key_size;
	size_t data_size;
	size_t cap;
	size_t count;
	Lepk__HtEntry *entires;
};

static Lepk__HtEntry *lepk__ht_find_entry(Lepk__HtEntry *entires, LepkHtCompare compare, unsigned long hash, unsigned long cap, const void *key) {
	unsigned long index = hash % cap;
	Lepk__HtEntry *entry = &entires[index];

	/* No entry at index. */
	if (entry->dead) {
		return entry;
	}

	/* Entry at current location. */
	for (;;) {
		entry = &entires[index];

		/* Found entry with same key or an empty slot. */
		if (entry->dead || compare(key, entry->key) == 0) {
			return entry;
		}

		index += (index + 1) % cap;
	}
}

LEPKHT LepkHt *lepk_ht_create(LepkHtHash hash, LepkHtCompare compare, unsigned long key_size, unsigned long data_size) {
	LepkHt *table = malloc(sizeof(LepkHt));

	table->hash = hash;
	table->compare = compare;

	table->key_size = key_size;
	table->data_size = data_size;
	table->cap = 8;
	table->count = 0;
	table->entires = malloc(table->cap * sizeof(Lepk__HtEntry));

	for (size_t i = 0; i < table->cap; i++) {
		table->entires[i].key = NULL;
		table->entires[i].data = NULL;
		table->entires[i].hash = 0;
		table->entires[i].dead = true;
	}

	return table;
}

LEPKHT void lepk_ht_destroy(LepkHt *table) {
	for (size_t i = 0; i < table->cap; i++) {
		Lepk__HtEntry *entry = &table->entires[i];
		if (entry->key  != NULL) { free(entry->key);  }
		if (entry->data != NULL) { free(entry->data); }
	}
	free(table->entires);
	free(table);
}

LEPKHT unsigned long lepk_ht_count(const LepkHt *table) {
	return table->count;
}

LEPKHT void lepk__ht_set(LepkHt *table, const void *key, const void *data) {
	/* Resize table if needed. */
	if (table->count >= (size_t) (table->cap * LEPK_HT_MAX_LOAD)) {
		size_t new_cap = table->cap * 2;

		Lepk__HtEntry *new_entires = malloc(new_cap * sizeof(Lepk__HtEntry));
		for (size_t i = 0; i < new_cap; i++) {
			new_entires[i].key = NULL;
			new_entires[i].data = NULL;
			new_entires[i].hash = 0;
			new_entires[i].dead = true;
		}
		/* Loop through old entires and place then in the new list. */
		for (size_t i = 0; i < table->cap; i++) {
			Lepk__HtEntry *entry = &table->entires[i];
			if (!entry->dead) {
				Lepk__HtEntry *new_entry = lepk__ht_find_entry(new_entires, table->compare, entry->hash, new_cap, entry->key);
				memcpy(new_entry, entry, sizeof(Lepk__HtEntry));
			}
		}

		table->cap = new_cap;
		free(table->entires);
		table->entires = new_entires;
	}

	unsigned long hash = table->hash(key);
	Lepk__HtEntry *entry = lepk__ht_find_entry(table->entires, table->compare, hash, table->cap, key);
	if (entry->dead) {
		table->count++;
	}

	entry->data = malloc(table->data_size);
	memcpy(entry->data, data, table->data_size);
	entry->key = malloc(table->key_size);
	memcpy(entry->key, key, table->key_size);
	entry->hash = hash;
	entry->dead = false;
}

LEPKHT void lepk__ht_get(LepkHt *table, const void *key, void *output) {
	assert(output != NULL && "Output pointer can't be NULL.");
	Lepk__HtEntry *entry = lepk__ht_find_entry(table->entires, table->compare, table->hash(key), table->cap, key);
	if (entry->dead) {
		return;
	}
	memcpy(output, entry->data, table->data_size);
}

LEPKHT void lepk__ht_remove(LepkHt *table, const void *key, void *output) {
	Lepk__HtEntry *entry = lepk__ht_find_entry(table->entires, table->compare, table->hash(key), table->cap, key);
	if (entry->dead) {
		return;
	}
	if (output != NULL) {
		memcpy(output, entry->data, table->data_size);
	}
	entry->dead = true;
}

unsigned long lepk_ht_hash_string(const void *key) {
	const char *_key = key;
	unsigned long len = strlen(_key);
	unsigned long hash = 2166136261lu;
	for (unsigned long i = 0; i < len; i++) {
		hash ^= (unsigned char) _key[i];
		hash *= 16777619;
	}
	return hash;
}

int lepk_ht_compare_string(const void *a, const void *b) {
	return strcmp((const char *) a, (const char *) b);
}
