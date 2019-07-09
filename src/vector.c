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

Vector *initializeVector(size_t initialSize) {
    Vector *newVector = NULL;
    newVector = malloc(sizeof(Vector));

    if (newVector == NULL) {
        return NULL;
    }

    newVector->data = malloc(initialSize*sizeof(void *));

    if (newVector->data == NULL) {
        return NULL;
    }

    newVector->size = 0;
    newVector->maxSize = initialSize;

    return newVector;
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

/**
 * inserts a value in a first place after a given location
 * that is NULL
 * @param vector
 * @param value
 * @param location
 * @return
 */
bool insertAfterVector(Vector *vector, void *value, size_t location) { //jaki typ tutaj??????????
    if (vector == NULL) { //czy jak dojdę na koniec wektora to powiększać, czy nie???????
        return false;
    }

    size_t i = location;

    if (vector->data[i] == NULL) {
        vector->data[i] = malloc(sizeof(void *));
        vector->data[i] = value;

        return true;
    }

    i++;

    while (vector->data[i] != NULL && i != location) {
        i = (i + 1) % (location + 1);
    }

    if (i == location) { //ale to nigdy sie nie stanie
        return false;
    }
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
