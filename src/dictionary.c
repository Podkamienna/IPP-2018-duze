//
// Created by alicja on 03.07.19.
//

#include "dictionary.h"
#include "hashTable.h"
#include "definitions.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

static const size_t INITIAL_HASH_TABLE_SIZE = 128;
static const size_t LOAD_FACTOR_MULTIPLIER = 3;
static const size_t LOAD_FACTOR_DIVIDER = 4;


typedef struct Dictionary Dictionary;
typedef struct HashTable HashTable;
typedef struct Entry Entry;

struct Dictionary {
    size_t id;
    size_t size;
    size_t numberOfElements;
    HashTable *hashTable;
};

static bool isFull(Dictionary *dictionary) {
    return LOAD_FACTOR_DIVIDER * dictionary->numberOfElements >= LOAD_FACTOR_MULTIPLIER * dictionary->size;
}

Dictionary *initializeDictionary() {
    Dictionary *newDictionary = malloc(sizeof(Dictionary));

    if (newDictionary == NULL) {
        return NULL;
    }

    newDictionary->id = 0;
    newDictionary->numberOfElements = 0;
    newDictionary->size = INITIAL_HASH_TABLE_SIZE;
    newDictionary->hashTable = initializeHashTable(newDictionary->size);

    if (newDictionary->hashTable == NULL) {
        free(newDictionary);
        return NULL;
    }

    return newDictionary;
}

void *searchDictionary(Dictionary *dictionary, const char *name) {
    if (dictionary == NULL) {
        return NULL;
    }

    return searchHashTable(dictionary->hashTable, name);
}

size_t getId(Dictionary *dictionary) {
    return dictionary->id;
}

bool insertDictionary(Dictionary *dictionary, const char *name, void *value) {
    FAIL_IF(dictionary == NULL);
    FAIL_IF(name == NULL);
    FAIL_IF(value == NULL);

    if (isFull(dictionary)) {
        HashTable *newHashTable = resizeHashTable(dictionary->hashTable, 2 * dictionary->size);
        FAIL_IF(newHashTable == NULL);

        dictionary->hashTable = newHashTable;
    }

    FAIL_IF(!insertHashTable(dictionary->hashTable, name, value));

    dictionary->id++;

    return true;

    failure:;
    return false;
}

void deleteDictionary(Dictionary *dictionary, void deleteValue(void *)) {
    if (dictionary == NULL) {
        return;
    }

    deleteHashTable(dictionary->hashTable, deleteValue);
    free(dictionary);
}
