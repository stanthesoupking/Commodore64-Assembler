#include "hash_table.h"

#include <stdlib.h>

struct hash_table_int
{
    void **buckets;
};

HashTable *createHashTable()
{
    HashTable *table = (HashTable *)malloc(sizeof(struct hash_table_int));

    // Initialise buckets
    table->buckets = (void **)malloc(sizeof(void *) * HASH_TABLE_BUCKETS);

    return table;
}

int hashTableHas(HashTable *table, char *key)
{
    unsigned long hKey = hash(key);
    return (table->buckets[hKey] != NULL);
}

void *hashTableGet(HashTable *table, char *key)
{
    unsigned long hKey = hash(key);
    return table->buckets[hKey];
}

void hashTableSet(HashTable *table, char *key, void *data)
{
    unsigned long hKey = hash(key);
    table->buckets[hKey] = data;
}

void hashTableRemove(HashTable *table, char *key)
{
    unsigned long hKey = hash(key);
    table->buckets[hKey] = NULL;
}

void destroyHashTable(HashTable* table)
{
    free(table->buckets);
    free(table);
    table = NULL;
}

// Hashes the given string using djb2 -- taken from http://www.cse.yorku.ca/~oz/hash.html
unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % HASH_TABLE_BUCKETS;
}