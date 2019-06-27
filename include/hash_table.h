#pragma once

typedef struct hash_table_int HashTable;

// Number of buckets in the hash table, more buckets will decrease collision
// probability. Note: prime numbers are preferable.
#define HASH_TABLE_BUCKETS 7919

HashTable *createHashTable();
int hashTableHas(HashTable *table, char *key);
void *hashTableGet(HashTable *table, char *key);
void hashTableSet(HashTable *table, char *key, void *data);
void hashTableRemove(HashTable *table, char *key);
void destroyHashTable(HashTable *table);

// Hashes the given string using djb2 -- taken from http://www.cse.yorku.ca/~oz/hash.html
unsigned long hash(unsigned char *str);