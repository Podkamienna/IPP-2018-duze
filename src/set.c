/** @file
 */

#include "set.h"
#include "vector.h"

#include <stdlib.h>

struct Set {
    Vector *vector;
    int (*compare)(void *, void *);
};

struct SetIterator {
    VectorIterator *vectorIterator;
};
Set *initializeSet(int compare(void *, void *)) {
    Set *newSet = malloc(sizeof(Set));

    if (newSet == NULL) {
        return NULL;
    }

    newSet->vector = initializeVector();
    newSet->compare = compare;

    return newSet;
}

void *searchSet(Set *set, void *value) {
    if (set == NULL) {
        return NULL;
    }

    return searchVector(set->vector, set->compare, value);
}

bool insertSet(Set *set, void *value) {
    if (set == NULL) {
        return false;
    }

    return pushVector(set->vector, value);
}

bool deleteFromSet(Set *set, void deleteValue(void *), void *value) {
    if (set == NULL) {
        return false;
    }

    return deleteFromVector(set->vector, deleteValue, set->compare, value);
}


void deleteSet(Set *set, void deleteValue(void *)) {
    if (set == NULL) {
        return;
    }

    deleteVector(set->vector, deleteValue);
    free(set);
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
//TODO czy zrobić nowy moduł?
bool incrementSetIterator(SetIterator *setIterator) {
    return incrementVectorIterator(setIterator->vectorIterator);
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