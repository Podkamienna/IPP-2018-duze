/** @file
 * Implementacja struktury wektor reprezentującej dynamicznie alokowane tablice
 * o zmiennym rozmiarze.
 */


#include "vector.h"
#include "definitions.h"

#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Funckja zwiększająca zadany wektor.
 * @param vector — wektor do zwiększenia
 * @return Wartość @p true jeżeli wszystko się powiodło.
 * Wartość @p false jeżeli nie udało się zaalokować pamięci,
 * lub zadany wektor był NULLem.
 */
static bool resizeVector(Vector *vector);

/**
 * @brief Funckcja zamieniająca 2 miejsca w tablicy,
 * używana na tablicy będącej przechowywaną przez wektor.
 * @param arr — tablica w której elementy będą zamieniane
 * @param i — pozycja pierwszego elementu do zamienienia
 * @param j — pozycja drugiego elementu do zamienienia
 */
static void swap(void **arr, size_t i, size_t j);

static bool resizeVector(Vector *vector) {
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

static void swap(void **arr, size_t i, size_t j) {
    void *tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
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

bool pushToVector(Vector *vector, void *value) {
    FAIL_IF(vector == NULL);
    FAIL_IF(value == NULL);

    if (vector->size + 1 > vector->maxSize) {
        FAIL_IF(!resizeVector(vector));
    }

    vector->data[vector->size++] = value;

    return true;

    failure:;
    return false;
}

void popFromVector(Vector *vector, void deleteValue(void *)) {
    if (vector == NULL) {
        return;
    }

    if (vector->size == 0) {
        return;
    }

    vector->size--;

    if (deleteValue != NULL) {
        deleteValue(vector->data[vector->size]);
    }
}

bool deleteFromVector(Vector *vector, void deleteValue(void *), int compare(void *, void *), void *value) {
    if (vector == NULL) {
        return false;
    }

    if (value == NULL) {
        return false;
    }

    size_t size = vector->size;

    for(size_t position = 1; position <= size; position++) {
        if (compare(vector->data[size - position], value) == 0) {
            swap(vector->data, size - position, vector->size - 1);
            popFromVector(vector, deleteValue);

            return true;
        }
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

void *getNextVectorIterator(VectorIterator *vectorIterator) {
    if (vectorIterator == NULL || vectorIterator->vector == NULL) {
        return NULL;
    }

    if (vectorIterator->position + 1 >= vectorIterator->vector->size) {
        return NULL;
    }

    vectorIterator->position++;

    return vectorIterator->vector->data[vectorIterator->position];
}

void deleteVectorIterator(VectorIterator *vectorIterator) {
    free(vectorIterator);
}