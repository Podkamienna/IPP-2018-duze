/** @file
 *
 */

#include "hashTable.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static const uint64_t PRIME = 179425373; //duża liczba pierwsza
static const uint64_t GENERATOR = 259; //generator grupy multyplikatywnej Z_{PRIME}

typedef struct HashTable HashTable;
typedef struct Entry Entry;

struct HashTable {
    Entry **table;
    size_t size;
};

struct Entry {
    uint64_t hash;
    const char *name;
    void *value;
};

//funkcja licząca hash danego napisu
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

//usuwa hash tablicę, niezwalniając pamięci zaalokowanej na jej elementy
static void deleteHashTableLeaveEntries(HashTable *hashTable) {
    if (hashTable == NULL) {
        return;
    }

    free(hashTable->table);
    free(hashTable);
}

//zwraca nowe entry, lub NULL, gdy się nie udało zaalokować pamięci
static Entry *initializeEntry(const char *name, void *value) {
    Entry *newEntry = malloc(sizeof(Entry));

    if (newEntry == NULL) {
        return NULL;
    }

    newEntry->name = name;
    newEntry->hash = getHash(name);
    newEntry->value = value;

    return newEntry;
}

//jeżeli dane wejście jest wyznaczone przez zadany napisa i hash
//zwraca true, wpp zwraca false
static bool isEqual(uint64_t hash, const char *name, Entry *entry) {
    return entry->hash == hash && strcmp(entry->name, name) == 0;
}

//zwraca pozycje elementu w danej hash tablicy
static size_t getPosition(HashTable *hashTable, size_t hash) {
    return hash % hashTable->size;
}

//wstawia wejście na pierwsze po wyliczonej pozycji
//puste miejsce w hash tablicy
//jeżeli to wejście jest już obecne lub nie ma wolnych pól w tablicy
//zwraca false, inaczej zwraca true
static bool insertEntryHashTable(HashTable *hashTable, Entry *entry) {
    size_t position = getPosition(hashTable, entry->hash);

    for (size_t i = 0; i < hashTable->size; i++, position = (position + 1) % hashTable->size) { // TODO getPosition
        // TODO może lepiej zamiast getPosition zrobić getNextPosition
        if (hashTable->table[i] == NULL) {
            hashTable->table[i] = entry;

            return true;
        }

        if (isEqual(entry->hash, entry->name, hashTable->table[i])) {
            return false;
        }
    }

    return false;
}

//TODO Hmm...
static Entry *searchEntryHashTable(HashTable *hashTable, const char *name) {
    size_t hash = getHash(name);
    size_t position = getPosition(hashTable, hash);
    size_t i = 0;

    while (hashTable->table[(position + i) % (hashTable->size + 1)] != NULL || i < hashTable->size) {
        if (hashTable->table[(position + i) % (hashTable->size + 1)]->hash !=
            hash) { // TODO skrócić (zrobić getNextPosition)
            return NULL;
        }

        if (isEqual(hash, name, hashTable->table[(position + i) % (hashTable->size + 1)])) {
            return hashTable->table[(position + i) % (hashTable->size + 1)];
        }
    }

    return NULL;
}

//usuwa wejście hash tablicy
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
    HashTable *newHashTable = malloc(sizeof(HashTable));

    if (newHashTable == NULL) {
        return NULL;
    }

    if (initialSize <= 0) {
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
    if (hashTable == NULL || name == NULL) {
        return NULL;
    }

    uint64_t hash = getHash(name);
    size_t position = hash % hashTable->size;

    for (size_t i = 0; i < hashTable->size; i++, position = (position + 1) % hashTable->size) {
        if (hashTable->table[i] == NULL) {
            return NULL;
        }

        if (isEqual(hash, name, hashTable->table[i])) {
            return hashTable->table[i]->value;
        }
    }

    return NULL;
}

bool insertHashTable(HashTable *hashTable, const char *name, void *value) {
    if (hashTable == NULL || name == NULL || value == NULL) {
        return false;
    }

    Entry *newEntry = initializeEntry(name, value);

    if (newEntry == NULL) {
        return false;
    }

    if (!insertEntryHashTable(hashTable, newEntry)) {
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
            if (!insertEntryHashTable(newHashTable, hashTable->table[i])) {
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