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

void *searchSet(Set *set, bool isOk(void *, void *), void *value) {
    return searchVector(set->vector, isOk, value);
}

bool insertSet(Set *set, void *value) {
    return pushVector(set->vector, value);
}

void deleteSet(Set *set, void deleteValue(void *)) {
    deleteVector(set->vector, deleteValue);
    free(set);
}