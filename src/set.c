/** @file
 */

#include "set.h"
#include "vector.h"

#include <stdlib.h>

struct Set {
    Vector *vector;
};

Set *initializeSet() {
    Set *newSet = malloc(sizeof(Set));

    if (newSet == NULL) {
        return NULL;
    }

    newSet->vector = initializeVector();
    return newSet;
}

void *searchSet(Set *set, bool cmp(void *, void *), void *value) {
    if (set == NULL) {
        return NULL;
    }

    return searchVector(set->vector, cmp, value);
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