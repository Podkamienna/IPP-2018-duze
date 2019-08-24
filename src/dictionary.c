/**
 * @file Implementacja klasy będącej słownikiem.
 */

#include "dictionary.h"
#include "hashTable.h"
#include "definitions.h"
// TODO usunąć tego include i fail_ify

#include <stdbool.h>
#include <stdlib.h>

/*Stała oznaczająca początkowy rozmiar hash tablicy*/
static const size_t INITIAL_HASH_TABLE_SIZE = 128;

/*Stałe oznaczające, że jeżeli liczba elementów w hash tablicy
 * przekroczy LOAD_FACTOR_MULTIPLIER/LOAD_FACTOR_DIVIDER jej
 * rozmiaru, to należy ją zwiększyć (aby uniknąć zbyt częstych kolizji)*/
static const size_t LOAD_FACTOR_MULTIPLIER = 3;
static const size_t LOAD_FACTOR_DIVIDER = 4;


typedef struct Dictionary Dictionary;
typedef struct HashTable HashTable;
typedef struct Entry Entry;

struct Dictionary {
    size_t size;
    size_t numberOfElements;
    HashTable *hashTable;
};

/**
 * @brief Sprawdza, czy już należy powiększyć słownik.
 * @param dictionary — słownik do sprawdzenia
 * @return Wartość @p true, jeżeli należy zwiększyć słownik,
 * wartość @p false, jeżeli jeszcze nie.
 */
static bool isFull(Dictionary *dictionary);

static bool isFull(Dictionary *dictionary) {
    return LOAD_FACTOR_DIVIDER * dictionary->numberOfElements >= LOAD_FACTOR_MULTIPLIER * dictionary->size;
}

Dictionary *initializeDictionary() {
    Dictionary *newDictionary = malloc(sizeof(Dictionary));

    if (newDictionary == NULL) {
        return NULL;
    }

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
    if (dictionary == NULL) {
        return 0;
    }

    return dictionary->numberOfElements;
}

bool insertDictionary(Dictionary *dictionary, const char *name, void *value) {
    FAIL_IF(dictionary == NULL);
    FAIL_IF(name == NULL);
    FAIL_IF(value == NULL);

    if (isFull(dictionary)) {
        HashTable *newHashTable = resizeHashTable(dictionary->hashTable, 2 * dictionary->size);
        FAIL_IF(newHashTable == NULL);

        dictionary->size = 2 * dictionary->size;

        dictionary->hashTable = newHashTable;
    }

    FAIL_IF(!insertHashTable(dictionary->hashTable, name, value));

    dictionary->numberOfElements++;

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
