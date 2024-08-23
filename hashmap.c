#include "hashmap.h"
#include "symbol.h"

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define INIT_CAPACITY 16 

#define HIGH_WATERMARK 70

#define LOW_WATERMARK 50

#define TOMBSTONE ((void *)-1)

uint64_t fnv_hash(const char *str, int length) {
    uint64_t hash = 0xcbf29ce484222325;
    for (int i = 0; i != length; ++i) {
        hash *= 0x100000001b3;
        hash ^= (unsigned char)str[i];
    }

    return hash;
}

int match(struct HashEntry *entry, const char *key, int keylen) {
    return entry->keyLen == keylen &&
        entry->key && entry->key != TOMBSTONE && memcmp(entry->key, key, keylen) == 0 ? 0 : 1;
}

struct HashEntry *get_or_insert_entry(struct HashMap *, const char *, int);

struct HashEntry *get_entry(struct HashMap *hash_map, const char *key, int keylen) {
    if (!hash_map || !hash_map->buckets) {
        return NULL;
    }

    uint64_t hash = fnv_hash(key, keylen);
    for (int i = 0; i != hash_map->capacity; ++i) {   
        struct HashEntry *entry = &hash_map->buckets[(i + hash) % hash_map->capacity];
        if (match(entry, key, keylen)) {
            return entry;
        }
    }

    return NULL;
}

void hash_map_put2(struct HashMap *hash_map, const char *key, int keylen, void *value) {
    struct HashEntry *entry = get_or_insert_entry(hash_map, key, keylen);
    if (entry) {
        entry->value = value;
    }
}

void hash_map_put(struct HashMap *hash_map, const char *key, void *value) {
    return hash_map_put2(hash_map, key, strlen(key), value);
}

void rehash(struct HashMap *hash_map) {
    int nkeys = 0;
    for (int i = 0; i != hash_map->capacity; ++i) {
        if (hash_map->buckets[i].key != TOMBSTONE) {
            ++nkeys;
        }
    }

    int cap = hash_map->capacity;
    while (nkeys * 100 > cap * LOW_WATERMARK) {
        cap <<= 2;
    }

    struct HashMap *new_hash_map = (struct HashMap *)malloc(sizeof(struct HashMap) * cap);
    new_hash_map->capacity = cap;
    new_hash_map->buckets = (struct HashEntry *)malloc(sizeof(struct HashEntry) * cap);

    for (int i = 0; i != hash_map->capacity; ++i) {
        if (hash_map->buckets[i].key && hash_map->buckets[i].key != TOMBSTONE) {
            hash_map_put2(new_hash_map, hash_map->buckets[i].key, hash_map->buckets[i].keyLen, hash_map->buckets[i].value);
        }
    }

    hash_map = new_hash_map;
}

struct HashEntry *get_or_insert_entry(struct HashMap *hash_map, const char *key, int keylen) {
    if (!hash_map) {
        return NULL;
    }

    if (!hash_map->buckets) {
        hash_map->buckets = (struct HashEntry *)malloc(sizeof(struct HashEntry) * INIT_CAPACITY);
        hash_map->capacity = INIT_CAPACITY;
        hash_map->used = 0;
    } else if ((hash_map->used * 100) > (hash_map->capacity * HIGH_WATERMARK)) {
        rehash(hash_map);
    }

    uint64_t hash = fnv_hash(key, keylen);
    for (int i = 0; i != hash_map->capacity; ++i) {
        struct HashEntry *entry = &hash_map->buckets[(hash + i) % hash_map->capacity];
        if (match(entry, key, keylen)) {
            return entry;
        }

        if (entry->key == TOMBSTONE) {
            entry->key = key;
            entry->keyLen = keylen;

            return entry;
        }

        if (entry->key == NULL) {
            entry->key = key;
            entry->keyLen = keylen;
            ++hash_map->used;

            return entry;
        }
    }

    return NULL;
}

void *hash_map_get2(struct HashMap *hash_map, const char *key, int keylen) {
    struct HashEntry *entry = get_entry(hash_map, key, keylen);
    if (!entry) {
        return NULL;
    }

    return entry->value;
}

void *hash_map_get(struct HashMap *hash_map, const char *key) {
    return hash_map_get2(hash_map, key, strlen(key));
}

void hash_map_delete2(struct HashMap *hash_map, const char *key, int keylen) {
    struct HashEntry *entry = get_entry(hash_map, key, keylen);
    if (!entry) {
        return ;
    }

    entry->key = TOMBSTONE;
}

void hash_map_delete(struct HashMap *hash_map, const char *key) {
    return hash_map_delete2(hash_map, key, (int)strlen(key));
}