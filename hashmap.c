// This is an implementation of the open-addressing hash table.

#include "hashmap.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define INIT_SIZE 16

#define HIGH_WATERMARK 70

#define LOW_WATERMARK 50

#define TOMBSTONE ((void *)-1)

struct HashMap *hashmap_new(void) {
    struct HashMap *map = (struct HashMap *)malloc(sizeof(struct HashMap));
    map->buckets = NULL;
    map->capacity = 0;
    map->used = 0;

    return map;
}

static void rehash(HashMap *);
static uint64_t fnv_hash(char *s, int len)
{
    uint64_t hash = 0xcbf29ce484222325;
    for (int i = 0; i < len; i++)
    {
        hash *= 0x100000001b3;
        hash ^= (unsigned char)s[i];
    }
    return hash;
}

static int match(HashEntry *ent, char *key, int keylen)
{
    return ent->key && ent->key != TOMBSTONE &&
                   ent->keylen == keylen && memcmp(ent->key, key, keylen) == 0
               ? 1
               : 0;
}

static HashEntry *get_or_insert_entry(HashMap *map, char *key, int keylen)
{
    if (!map->buckets)
    {
        map->buckets = calloc(INIT_SIZE, sizeof(HashEntry));
        map->capacity = INIT_SIZE;
    }
    else if ((map->used * 100) / map->capacity >= HIGH_WATERMARK)
    {
        rehash(map);
    }

    uint64_t hash = fnv_hash(key, keylen);

    for (int i = 0; i < map->capacity; i++)
    {
        HashEntry *ent = &map->buckets[(hash + i) % map->capacity];

        if (match(ent, key, keylen))
            return ent;

        if (ent->key == TOMBSTONE)
        {
            ent->key = key;
            ent->keylen = keylen;
            return ent;
        }

        if (ent->key == NULL)
        {
            ent->key = key;
            ent->keylen = keylen;
            map->used++;
            return ent;
        }
    }

    return NULL;
}

void hashmap_put2(HashMap *map, char *key, int keylen, void *val)
{
    HashEntry *ent = get_or_insert_entry(map, key, keylen);
    ent->val = val;
}

static void rehash(HashMap *map)
{
    int nkeys = 0;
    for (int i = 0; i < map->capacity; i++)
        if (map->buckets[i].key && map->buckets[i].key != TOMBSTONE)
            nkeys++;

    int cap = map->capacity;
    while ((nkeys * 100) / cap >= LOW_WATERMARK)
        cap = cap * 2;

    HashMap map2 = {};
    map2.buckets = (HashEntry *)calloc(cap, sizeof(HashEntry));
    map2.capacity = cap;

    for (int i = 0; i < map->capacity; i++)
    {
        HashEntry *ent = &map->buckets[i];
        if (ent->key && ent->key != TOMBSTONE)
            hashmap_put2(&map2, ent->key, ent->keylen, ent->val);
    }

    *map = map2;
}

static HashEntry *get_entry(HashMap *map, char *key, int keylen)
{
    if (!map->buckets)
        return NULL;

    uint64_t hash = fnv_hash(key, keylen);

    for (int i = 0; i < map->capacity; i++)
    {
        HashEntry *ent = &map->buckets[(hash + i) % map->capacity];
        if (match(ent, key, keylen))
            return ent;
        if (ent->key == NULL)
            return NULL;
    }

    return NULL;
}

void *hashmap_get2(HashMap *map, char *key, int keylen)
{
    HashEntry *ent = get_entry(map, key, keylen);
    return ent ? ent->val : NULL;
}

void *hashmap_get(HashMap *map, char *key)
{
    return hashmap_get2(map, key, strlen(key));
}

void hashmap_put(HashMap *map, char *key, void *val)
{
    hashmap_put2(map, key, strlen(key), val);
}

void hashmap_delete2(HashMap *map, char *key, int keylen)
{
    HashEntry *ent = get_entry(map, key, keylen);
    if (ent)
        ent->key = TOMBSTONE;
}

void hashmap_delete(HashMap *map, char *key)
{
    hashmap_delete2(map, key, strlen(key));
}
