/** @file
 * Implementacja klasy będącej słownikiem.
 *
 * @author Alicja Ziarko <ala.ziarko@gmail.com>
 * @date 05.07.2019
 */

#include "dictionary.h"
#include "hashTable.h"

#include <stdbool.h>
#include <stdlib.h>

/**
 * Stała oznaczająca początkowy rozmiar hash tablicy
 */
static const size_t INITIAL_HASH_TABLE_SIZE = 128;

/**
 * Licznik ułamka, określającego, kiedy słownik jest pełny.
 */
static const size_t LOAD_FACTOR_MULTIPLIER = 3;

/**
 * Mianowik ułamka, określającego, kiedy słownik jest pełny.
 */
static const size_t LOAD_FACTOR_DIVIDER = 4;

/**
 * Struktura będąca słownikiem, używająca tablicy hashująąej.
 */
struct Dictionary {
    size_t size; ///< ile elementów może być włożonych do słownika
    size_t numberOfElements; ///< ile elementów jest włożonych do słownika
    HashTable *hashTable; ///< hash tablica na której przechowywane są elementy
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
    if (dictionary == NULL) {
        return false;
    }

    if (name == NULL) {
        return false;
    }

    if (value == NULL) {
        return false;
    }

    if (isFull(dictionary)) {
        HashTable *newHashTable = resizeHashTable(dictionary->hashTable, 2 * dictionary->size);
        if (newHashTable == NULL) {
            return false;
        }

        dictionary->size = 2 * dictionary->size;

        dictionary->hashTable = newHashTable;
    }

    if (!insertHashTable(dictionary->hashTable, name, value)) {
        return false;
    }

    dictionary->numberOfElements++;

    return true;
}

void deleteDictionary(Dictionary *dictionary, void deleteValue(void *)) {
    if (dictionary == NULL) {
        return;
    }

    deleteHashTable(dictionary->hashTable, deleteValue);
    free(dictionary);
}
