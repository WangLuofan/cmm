#ifndef __HASHMAP_H__
#define __HASHMAP_H__

typedef struct HashEntry {
    char *key;
    int keylen;
    void *val;
}HashEntry;

typedef struct HashMap {
    struct HashEntry *buckets;
    int capacity;
    int used;
}HashMap;

void *hashmap_get(struct HashMap *, char *);
void hashmap_put(struct HashMap *, char *, void *);
void hashmap_delete(struct HashMap *, char *);

#endif