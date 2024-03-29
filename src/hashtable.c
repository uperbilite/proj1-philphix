#include "hashtable.h"

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * This creates a new hash table of the specified size and with
 * the given hash function and comparison function.
 */
HashTable *createHashTable(int size, unsigned int (*hashFunction)(void *),
                           int (*equalFunction)(void *, void *)) {
    int i = 0;
    HashTable *newTable = malloc(sizeof(HashTable));
    if (NULL == newTable) {
        fprintf(stderr, "malloc failed \n");
        exit(1);
    }
    newTable->size = size;
    newTable->buckets = malloc(sizeof(struct HashBucketEntry *) * size);
    if (NULL == newTable->buckets) {
        fprintf(stderr, "malloc failed \n");
        exit(1);
    }
    for (i = 0; i < size; i++) {
        newTable->buckets[i] = NULL;
    }
    newTable->hashFunction = hashFunction;
    newTable->equalFunction = equalFunction;
    return newTable;
}

/* Task 1.2 */
void insertData(HashTable *table, void *key, void *data) {
    // HINT:
    // 1. Find the right hash bucket location with table->hashFunction.
    // 2. Allocate a new hash bucket entry struct.
    // 3. Append to the linked list or create it if it does not yet exist.
    unsigned int loc = table->hashFunction(key) % table->size;
    HashBucketEntry *newEntry = (HashBucketEntry *) malloc(sizeof(HashBucketEntry));

    if (newEntry == NULL) {
        fprintf(stderr, "malloc failed \n");
        exit(1);
    }

    newEntry->next = table->buckets[loc];
    newEntry->key = key;
    newEntry->data = data;
    table->buckets[loc] = newEntry;
}

/* Task 1.3 */
void *findData(HashTable *table, void *key) {
    // HINT:
    // 1. Find the right hash bucket with table->hashFunction.
    // 2. Walk the linked list and check for equality with table->equalFunction.
    unsigned int loc = table->hashFunction(key) % table->size;
    HashBucketEntry *entry = table->buckets[loc];

    while (entry != NULL) {
        if (table->equalFunction(key, entry->key)) {
            return entry->data;
        }
        entry = entry->next;
    }

    return NULL;
}

/* Task 2.1 */
unsigned int stringHash(void *s) {
    char *string = (char *) s;
    const int M = 1e9 + 7;
    const int B = 31;
    unsigned int res = 0;

    unsigned int len = strlen(string);
    for (size_t i = 0; i < len; i++) {
        res = (res * B + string[i]) % M;
    }

    return res;
}

/* Task 2.2 */
int stringEquals(void *s1, void *s2) {
    return strcmp((char *) s1, (char *) s2) == 0;
}
