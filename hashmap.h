#ifndef __HASHMAP_H__
#define __HASHMAP_H__

typedef struct HashEntry {
    char *key;
    int keyLen;
    void *value;
}HashEntry;

typedef struct HashMap {
    struct HashEntry *buckets;
    int capacity;
    int used;
}HashMap;

struct HashMap *new_hash_map(void);
void *hash_map_get(struct HashMap *, const char *);
void hash_map_put(struct HashMap *, const char *, void *);
void hash_map_delete(struct HashMap *, const char *);

#endif