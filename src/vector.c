/** @file
 */
 //TODO poogarniać te nagłówki

#include "vector.h"

#include <stdlib.h>
#include <stdbool.h>

typedef struct Vector Vector;
typedef struct VectorIterator VectorIterator;

static bool resizeVector(Vector *vector) {
    // TODO może static? nigdzie nie jest uzywane na zewnatrz modulu? czy kiedys bedzie?
    if (vector == NULL) {
        return false;
    }

    size_t newSize = 2 * vector->maxSize + 4;

    void **newData = realloc(vector->data, newSize * sizeof(void *));

    if (newData == NULL) {
        return false;
    }

    vector->data = newData;
    vector->maxSize = newSize;

    return true;
}

Vector *initializeVector() {
    Vector *newVector = malloc(sizeof(Vector));

    if (newVector == NULL) {
        return NULL;
    }

    newVector->data = NULL;


    newVector->size = 0;
    newVector->maxSize = 0;

    return newVector;
}

void *searchVector(Vector *vector, int cmp(void *, void *), void *value) {
    if (vector == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < vector->size; i++) {
        if (cmp(vector->data[i], value) == 0) {
            return vector->data[i];
        }
    }

    return NULL;
}

bool pushVector(Vector *vector, void *value) {
    if (vector == NULL) {
        return false;
    }

    if (value == NULL) {
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

    if (deleteValue != NULL && vector->data[vector->size] != NULL) {
        deleteValue(vector->data[vector->size]);
    }

    vector->data[vector->size] = NULL;
}

static void swap(void **arr, size_t i, size_t j) {
    void *tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
}

bool deleteFromVector(Vector *vector, void deleteValue(void *), int compare(void *, void *), void *value) {
    if (vector == NULL) {
        return false;
    }

    size_t size = vector->size;
    size_t position = 0;

    while (position <= size) {
        if (compare(vector->data[size - position], value) == 0) {
            swap(vector->data, position, vector->size);
            popFromVector(vector, deleteValue);

            return true;
        }

        position++;
    }

    return false;
}

bool isEmptyVector(Vector *vector) {
    if (vector == NULL) {
        return true;
    }

    return vector->size == 0;
}

void deleteVector(Vector *vector, void deleteValue(void *)) {
    if (vector == NULL) {
        return;
    }

    if (vector->data != NULL && deleteValue != NULL) {
        for (size_t i = 0; i < vector->size; i++) {
            deleteValue(vector->data[i]);
            vector->data[i] = NULL;
        }
    }

    free(vector->data);
    free(vector);
}

VectorIterator *getNewVectorIterator(Vector *vector) {
    if (vector == NULL) {
        return NULL;
    }

    VectorIterator *newVectorIterator = malloc(sizeof(VectorIterator));
    if (newVectorIterator == NULL) {
        return NULL;
    }

    newVectorIterator->vector = vector;
    newVectorIterator->position = -1;

    return newVectorIterator;
}

bool incrementVectorIterator(VectorIterator *vectorIterator) {
    if (vectorIterator == NULL) {
        return false;
    }

    if (vectorIterator->position + 1 >= vectorIterator->vector->size) {
        return false;
    }

    vectorIterator->position++;

    return true;
}

void *getNextVectorIterator(VectorIterator *vectorIterator) {
    if (vectorIterator == NULL || vectorIterator->vector == NULL) {
        return NULL;
    }

    if (!incrementVectorIterator(vectorIterator)) {
        return NULL;
    }

    return vectorIterator->vector->data[vectorIterator->position];
}

void deleteVectorIterator(VectorIterator *vectorIterator) {
    free(vectorIterator);
}