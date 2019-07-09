//
// Created by alicja on 07.07.19.
//

#include "hashTable.h"
#include "vector.h"

typedef struct HashTable HashTable;



HashTable *initializeHashTable() {

}

void *searchHashTable(HashTable *hashTable, const char *name);

bool insertHashTable(HashTable *hashTable, const char *name, void *content);

void freeHashTable(HashTable *hashTable, void *freeContent(void *));