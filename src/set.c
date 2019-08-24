/** @file
 * Implementacja struktury zbiór.
 */

#include "set.h"
#include "vector.h"

#include <stdbool.h>
#include <stdlib.h>

struct Set {
    Vector *vector;

    bool (*areEqual)(void *, void *);
};

struct SetIterator {
    VectorIterator *vectorIterator;
};

Set *initializeSet(bool areEqual(void *, void *)) {
    Set *newSet = malloc(sizeof(Set));

    if (newSet == NULL) {
        return NULL;
    }

    newSet->vector = initializeVector();
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
    deleteVectorIterator(setIterator->vectorIterator);

    free(setIterator);
}