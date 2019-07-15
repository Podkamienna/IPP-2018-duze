/** @file
 */

#include "vector.h"

#include <stdlib.h>
#include <stdbool.h>

bool resizeVector(Vector *vector) {
    void **newData;
    if (vector == NULL) {
        return false;
    }

    size_t newSize = 2 * vector->maxSize + 4;

    newData = realloc(vector->data, newSize * sizeof(void *));

    if (newData != NULL) {
        vector->data = newData;
        vector->maxSize = newSize;
        return true;
    }

    return false;
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

void *searchVector(Vector *vector, bool isOk(void *, void *), void *value) {
    for (size_t i = 0; i < vector->size; i++) {
        if (isOk(vector->data[i], value)) {
            return vector->data[i];
        }
    }

    return NULL;
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

void popFromVector(Vector *vector, void deleteValue(void *)) {
    if (vector == NULL) {
        return;
    }

    vector->size--;

    if (deleteValue != NULL) {
        deleteValue(vector->data[vector->size]);
    }
}

void deleteVector(Vector *vector, void deleteValue(void *)) {
    if (vector == NULL) {
        return;
    }

    if (vector->data != NULL && deleteValue != NULL) {
        for (size_t i = 0; i < vector->size; i++) {
            deleteValue(vector->data[i]);
        }
    }

    free(vector->data);
    free(vector);
}
