/** @file
 *
 */

#ifndef DROGI_HASHTABLE_H
#define DROGI_HASHTABLE_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct HashTable HashTable;

HashTable *initializeHashTable(size_t initialSize);

void *searchHashTable(HashTable *hashTable, const char *name);

bool insertHashTable(HashTable *hashTable, const char *name, void *value);

void iterate(HashTable *hashTable, void fun(void *));

HashTable *resizeHashTable(HashTable *hashTable, size_t newSize);

void deleteHashTable(HashTable *hashTable, void deleteValue(void *));

#endif /* DROGI_HASHTABLE_H */
