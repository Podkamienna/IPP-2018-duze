/**
 * @file Implementacja tablicy hashującej.
 */

#include "hashTable.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

//TODO komentarze przy typedefach czy przy strukturach????
//TODO pozmieniać wszystkie krótkie komentarze na małe litery bez kropek, a duże na wielkie i kropki.

/**
 * Duża liczba pierwsza.
 */
static const uint64_t PRIME = 179425373;

/**
 * Generator grupy multiplikatywnej Z_{PRIME}.
 */
static const uint64_t GENERATOR = 259;

/**
 * Struktura przechowująca tablicę hashującą.
 */
typedef struct HashTable HashTable;

/**
 * Struktura przechowująca jedno wejście tablicy hashującej,
 */
typedef struct Entry Entry;

struct HashTable {
    Entry **table; ///< tablica na której jest tablica hashująca
    size_t size; ///< rozmiar tablicy hashującej
};

struct Entry {
    uint64_t hash; ///< hash nazwy z danego wejścia hash tablicy
    const char *name; ///< nazwa danego wejścia hash tablicy
    void *value; ///< wartość przypisana danemu napisowi w hash tablicy
};

/**
 * @brief Oblicza i zwraca hash zadanego tekstu.
 * @param text — tekst którego hash ma być obliczony.
 * @return Obliczony hash.
 */
static uint64_t getHash(const char *text);

/**
 * @brief Usuwa hash tablicę, bez usuwania zawartości
 * @param hashTable — hash tablica do usunięcia
 */
static void deleteHashTableLeaveEntries(HashTable *hashTable);

/**
 * @brief Alokuje pamięć pod i zwraca wskaźnik na nowy
 * element hash tablicy.
 * @param name — nazwa do której ma być przypisana wartość w hash tablicy
 * @param value — wartość do wstawienia do hash tablicy
 * @return Wskaźnik na nową tablicę, lub NULL, gdy któryś z parametrów miał
 * złą wartość lub nie udało się zaalokować pamięci.
 */
static Entry *getNewEntry(const char *name, void *value);

/**
 * @brief Sprawdza, czy element hash tablicy jest przypisany danemu
 * napisowi
 * @param hash — hash napisu, który będzie sprawdzany
 * @param name  — napis, który będzie sprawdzany
 * @param entry  — element hash tablicy do sprawdzenia
 * @return Wartość @p true, jeżeli równość zachodzi.
 * Wartość @p false, jeżeli nie zachodzi.
 */
static bool isEqual(uint64_t hash, const char *name, Entry *entry);

static size_t getPosition(HashTable *hashTable, size_t hash);

static bool insertEntry(HashTable *hashTable, Entry *entry);

static void deleteEntry(Entry *entry, void deleteValue(void *));

static uint64_t getHash(const char *text) {
    uint64_t coefficient = 1, tmpHash = 0;

    for (size_t i = 0; text[i] != '\0'; i++) {
        tmpHash += coefficient * text[i];
        coefficient *= GENERATOR;
        tmpHash %= PRIME;
        coefficient %= PRIME;
    }

    return tmpHash;
}

static void deleteHashTableLeaveEntries(HashTable *hashTable) {
    if (hashTable == NULL) {
        return;
    }

    free(hashTable->table);
    free(hashTable);
}

static Entry *getNewEntry(const char *name, void *value) {
    Entry *newEntry = malloc(sizeof(Entry));

    if (newEntry == NULL) {
        return NULL;
    }

    newEntry->name = name;
    newEntry->hash = getHash(name);
    newEntry->value = value;

    return newEntry;
}

static bool isEqual(uint64_t hash, const char *name, Entry *entry) {
    return entry->hash == hash && strcmp(entry->name, name) == 0;
}

static size_t getPosition(HashTable *hashTable, size_t hash) {
    return hash % hashTable->size;
}

static bool insertEntry(HashTable *hashTable, Entry *entry) {
    size_t position = getPosition(hashTable, entry->hash);

    for (size_t i = 0; i < hashTable->size; i++, position = (position + 1) % hashTable->size) {
        if (hashTable->table[position] == NULL) {
            hashTable->table[position] = entry;

            return true;
        }

        if (isEqual(entry->hash, entry->name, hashTable->table[position])) {
            return false;
        }
    }

    return false;
}

static void deleteEntry(Entry *entry, void deleteValue(void *)) {
    if (entry == NULL) {
        return;
    }

    if (deleteValue != NULL) {
        deleteValue(entry->value);
    }

    free(entry);
}

HashTable *initializeHashTable(size_t initialSize) {
    if (initialSize <= 0) {
        return NULL;
    }

    HashTable *newHashTable = malloc(sizeof(HashTable));

    if (newHashTable == NULL) {
        return NULL;
    }

    newHashTable->size = initialSize;
    newHashTable->table = calloc(newHashTable->size, sizeof(Entry));

    if (newHashTable->table == NULL) {
        free(newHashTable);

        return NULL;
    }

    return newHashTable;
}


void *searchHashTable(HashTable *hashTable, const char *name) {
    if (hashTable == NULL) {
        return NULL;
    }

    if (name == NULL) {
        return NULL;
    }

    uint64_t hash = getHash(name);
    size_t position = hash % hashTable->size;

    for (size_t i = 0; i < hashTable->size; i++, position = (position + 1) % hashTable->size) {
        if (hashTable->table[position] == NULL) {
            return NULL;
        }

        if (isEqual(hash, name, hashTable->table[position])) {
            return hashTable->table[position]->value;
        }
    }

    return NULL;
}

bool insertHashTable(HashTable *hashTable, const char *name, void *value) {
    Entry *newEntry = NULL;

    if (hashTable == NULL || name == NULL || value == NULL) {
        return false;
    }

    newEntry = getNewEntry(name, value);

    if (newEntry == NULL) {
        return false;
    }

    if (!insertEntry(hashTable, newEntry)) {
        deleteEntry(newEntry, NULL);
        return false;
    }

    return true;
}

HashTable *resizeHashTable(HashTable *hashTable, size_t newSize) {
    if (hashTable == NULL || newSize <= 0) {
        return NULL;
    }

    HashTable *newHashTable = initializeHashTable(newSize);

    if (newHashTable == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < hashTable->size; i++) {
        if (hashTable->table[i] != NULL) {
            if (!insertEntry(newHashTable, hashTable->table[i])) {
                deleteHashTableLeaveEntries(newHashTable);

                return NULL;
            }
        }
    }
    deleteHashTableLeaveEntries(hashTable);

    return newHashTable;
}

void deleteHashTable(HashTable *hashTable, void deleteValue(void *)) {
    if (hashTable == NULL) {
        return;
    }

    if (hashTable->table != NULL) {
        for (size_t i = 0; i < hashTable->size; i++) {
            deleteEntry(hashTable->table[i], deleteValue);
        }
    }

    free(hashTable->table);
    free(hashTable);
}