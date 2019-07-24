/** @file
 */

#include "set.h"
#include "vector.h"

#include <stdlib.h>

struct Set {
    Vector *vector;
    int (*compare)(void *, void *);
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

void deleteLastAdded(Set *set, void deleteValue(void *)) {
    if (set == NULL) {
        return;
    }

    popFromVector(set->vector, deleteValue);
}

void deleteSet(Set *set, void deleteValue(void *)) {
    if (set == NULL) {
        return;
    }

    deleteVector(set->vector, deleteValue);
    free(set);
}