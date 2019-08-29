/** @file
 * Implementacja struktury zbiór.
 */

#include "set.h"
#include "vector.h"

#include <stdbool.h>
#include <stdlib.h>

/**
 * Struktura przechowująca zbiór.
 */
struct Set {
    Vector *vector; ///< wektor na którym są przechowywane elementy zbioru
    bool (*areEqual)(void *, void *); ///< predykat pozwalający na sprawdzenie, czy 2 elementy zbioru są równe
};

/**
 * Struktura przechowująca iterator na zbiór.
 */
struct SetIterator {
    VectorIterator *vectorIterator; ///< iterator na wektor, na którym są elementy zbioru
};

Set *initializeSet(bool areEqual(void *, void *)) {
    Set *newSet = malloc(sizeof(Set));

    if (newSet == NULL) {
        return NULL;
    }

    newSet->vector = initializeVector();

    if (newSet->vector == NULL) {
        deleteSet(newSet, NULL);

        return NULL;
    }

    newSet->areEqual = areEqual;

    return newSet;
}

void deleteSet(Set *set, void deleteValue(void *)) {
    if (set == NULL) {
        return;
    }

    deleteVector(set->vector, deleteValue);
    free(set);
}

size_t getSetSize(Set *set) {
    if (set == NULL) {
        return 0;
    }

    return set->vector->size;
}

void *searchSet(Set *set, void *value) {
    if (set == NULL) {
        return NULL;
    }

    return searchVector(set->vector, set->areEqual, value);
}

bool insertSet(Set *set, void *value) {
    if (set == NULL) {
        return false;
    }

    return pushToVector(set->vector, value);
}

bool deleteFromSet(Set *set, void deleteValue(void *), void *value) {
    if (set == NULL) {
        return false;
    }

    return deleteFromVector(set->vector, deleteValue, set->areEqual, value);
}

SetIterator *getNewSetIterator(Set *set) {
    if (set == NULL) {
        return NULL;
    }

    SetIterator *setIterator = malloc(sizeof(struct SetIterator));

    if (setIterator == NULL) {
        return NULL;
    }

    setIterator->vectorIterator = getNewVectorIterator(set->vector);

    if (setIterator->vectorIterator == NULL) {
        free(setIterator);

        return NULL;
    }

    return setIterator;
}


void *getNextSetIterator(SetIterator *setIterator) {
    if (setIterator == NULL) {
        return NULL;
    }

    return getNextVectorIterator(setIterator->vectorIterator);
}

void deleteSetIterator(SetIterator *setIterator) {
    if (setIterator == NULL) {
        return;
    }

    deleteVectorIterator(setIterator->vectorIterator);

    free(setIterator);
}