//
// Created by alicja on 07.07.19.
//

#ifndef DROGI_HASHTABLE_H
#define DROGI_HASHTABLE_H

typedef struct HashTable HashTable;

HashTable *initializeHashTable();

void *searchHashTable(HashTable *hashTable, const char *name);

bool insertHashTable(HashTable *hashTable, const char *name, void *content);

void freeHashTable(HashTable *hashTable, void *freeContent(void *));

#endif //DROGI_HASHTABLE_H
