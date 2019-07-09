//
// Created by alicja on 03.07.19.
//

#include "dictionary.h"
#include "vector.h"

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
    HashTable *hashTable;
};


static bool isFull(HashTable *hashTable) {
    return true;
}

Dictionary *initializeDictionary() {
    Dictionary *newDictionary = malloc(sizeof(Dictionary));

    if (newDictionary == NULL) {
        return NULL;
    }

    newDictionary->id = 0;
    newDictionary->hashTable = malloc(sizeof(HashTable));

    if (newDictionary->hashTable == NULL) {
        free(newDictionary);

        return NULL;
    }

    newDictionary->hashTable->numberOfUsed = 0;
    newDictionary->hashTable->table = initializeVector(INITIAL_HASH_TABLE_SIZE);
    newDictionary->hashTable->size = INITIAL_HASH_TABLE_SIZE;

    if (newDictionary->hashTable->table == NULL) {
        free(newDictionary->hashTable);
        free(newDictionary);

        return NULL;
    }

    return newDictionary;
}
//zakladam, ze dictionary nie jest NULLem i dictionary->hashTable też
bool resizeDictionary(Dictionary *dictionary) {
    if (!resizeVector(dictionary->hashTable->table)) {
        return false;
    }

    dictionary->hashTable->size = dictionary->hashTable->table->maxSize;

}

/**
 * @param dictionary
 * @param name
 * @param hash
 * @return NULL if name is NULL or dictionary is NULL or the entry that is being searched
 * for doesn't exist
 */

void *searchDictionary(Dictionary *dictionary, const char *name, bool isRight(void *, string)) {

}
/**
 * the dictionary is not NULL
 * @param dictionary
 * @param name
 * @param getNewContent
 * @param hash
 * @return
 */
bool insertDictionary(Dictionary *dictionary, const char *name, void *content) {
    if (dictionary == NULL || name == NULL || content == NULL || dictionary->hashTable == NULL) {
        return false;
    }

    if (2 * dictionary->hashTable->numberOfUsed > dictionary->hashTable->size) { //Czy taka licza jest ok???
        resizeDictionary(dictionary);
    }


}

void freeDictionary(Dictionary *dictionary, void *freeContent(void *)) {
    while (tmp1 != NULL) {
        freeContent(tmp1->content);
        tmp2 = tmp1;
        free(tmp1->name);
        tmp1 = tmp1->next;
        free(tmp2);
    }
}
