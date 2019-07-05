/** @file
 */

#include "vector.h"

#include <stdlib.h>
#include <stdbool.h>

static bool resizeVector(Vector *vector) {
    void **newData;
    size_t newSize = 2 * vector->maxSize + 4;

    newData = realloc(vector->data, newSize * sizeof(void *));

    if (newData != NULL) {
        vector->data = newData;
        vector->maxSize = newSize;
        return true;
    }

    return false;
}

bool pushVector(Vector *vector, void *value) {
    if (vector == NULL) {
        return false;
    }

    if (vector->size + 1 > vector->maxSize) {
        if (!resizeVector(vector)) {
            return false;
        }
    }
    vector->data[vector->size++] = value;
    return true;
}

Vector *initializeVector() {
    Vector *newVector = NULL;
    newVector = malloc(sizeof(Vector));

    if (newVector == NULL) {
        return NULL;
    }

    newVector->data = NULL;
    newVector->size = 0;
    newVector->maxSize = 0;

    return newVector;
}

void freeVector(Vector *vector, void freeValue(void *)) {
    if (vector == NULL) {
        return;
    }

    if (vector->data != NULL && freeValue != NULL) {
        for (size_t i = 0; i < vector->size; i++) {
            freeValue(vector->data[i]);
        }
    }

    free(vector->data);
    free(vector);
}
