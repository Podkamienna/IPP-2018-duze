//
// Created by alicja on 03.07.19.
//

#include "dictionary.h"
#include "hashTable.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

static const size_t INITIAL_HASH_TABLE_SIZE = 100;
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
    return LOAD_FACTOR_DIVIDER*dictionary->numberOfElements >= LOAD_FACTOR_MULTIPLIER*dictionary->size;
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

/**
 * @param dictionary
 * @param name
 * @param hash
 * @return NULL if name is NULL or dictionary is NULL or the entry that is being searched
 * for doesn't exist
 */

void *searchDictionary(Dictionary *dictionary, const char *name) {
    if (dictionary == NULL) {
        return NULL;
    }

    return searchHashTable(dictionary->hashTable, name);
}
/**
 * the dictionary is not NULL
 * @param dictionary
 * @param name
 * @param getNewContent
 * @param hash
 * @return
 */
bool insertDictionary(Dictionary *dictionary, const char *name, void *value, void deleteValue(void *)) {
    if (dictionary == NULL || name == NULL || value == NULL || dictionary->hashTable == NULL) {
        return false;
    }

    if (isFull(dictionary)) {
        HashTable *newHashTable = resizeHashTable(dictionary->hashTable, 2*dictionary->size);

        if (newHashTable == NULL) {
            return false;
        }
// TODO
        if (!insertHashTable(newHashTable, name, value)) {
            deleteHashTable(newHashTable, deleteValue);
            return false;
        }

        dictionary->hashTable = newHashTable;
        return true;
    }

    if (!insertHashTable(dictionary->hashTable, name, value)) {
        return false;
    }

    return true;
}

void deleteDictionary(Dictionary *dictionary, void deleteValue(void *)) {
    if (dictionary == NULL) {
        return;
    }

    deleteHashTable(dictionary->hashTable, deleteValue);
    free(dictionary);
}
