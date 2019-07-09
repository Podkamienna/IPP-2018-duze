//
// Created by alicja on 03.07.19.
//

#include "dictionary.h"
#include "vector.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct Dictionary Dictionary;
typedef struct HashTable HashTable;
typedef struct EntryList EntryList;

static const uint64_t PRIME = 179425373; //big prime number
static const uint64_t GENERATOR = 259; //number that generates the multiplicative group Z_MOD(PRIME)
static const uint64_t INITIAL_HASH_TABLE_SIZE = 100;
/**
 * getHash is not NULL
 * @param text
 * @return
 */
static size_t getHash(const char *text) {
    size_t n = strlen(text);
    uint64_t coefficient = 1, tmpHash = 0;

    for (size_t i = 0; i < n; i++) {
        tmpHash += coefficient * text[i];
        coefficient *= GENERATOR;
        tmpHash %= PRIME;
        coefficient %= PRIME;
    }

    return tmpHash;
}


struct Dictionary {
    uint64_t id;
    HashTable *hashTable;
};

struct HashTable {
    Vector *table; //entries with certain hash
    uint64_t size;
    uint64_t numberOfUsed;
};

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

    if(newDictionary->hashTable->table == NULL) {
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
    if (dictionary == NULL || name == NULL) {
        return NULL;
    }

    size_t i = getHash(name) % dictionary->hashTable->size;

    while (dictionary->hashTable->table->data[i] != NULL && !isRight(dictionary->hashTable->table->data[i], name)) {
        i = (i + 1) % (dictionary->hashTable->size + 1);
    }

    if (dictionary->hashTable->table->data[i] == NULL) {
        return NULL;
    }

    return dictionary->hashTable->table->data[i];
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
