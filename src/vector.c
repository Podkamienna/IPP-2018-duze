/** @file
 * Implementacja struktury wektor reprezentującej dynamicznie alokowane tablice
 * o zmiennym rozmiarze.
 *
 * @author Alicja Ziarko <ala.ziarko@gmail.com>
 * @date 02.07.2019
 */


#include "vector.h"

#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Funkcja zwiększająca zadany wektor.
 * @param vector — wektor do zwiększenia
 * @return Wartość @p true jeżeli wszystko się powiodło.
 * Wartość @p false jeżeli nie udało się zaalokować pamięci,
 * lub zadany wektor był NULLem.
 */
static bool resizeVector(Vector *vector);

/**
 * @brief Funkcja zamieniająca 2 miejsca w tablicy,
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

void resetVector(Vector *vector) {
    if (vector == NULL) {
        return;
    }

    vector->size = 0;
}

void *searchVector(Vector *vector, bool areEqual(void *, void *), void *value) {
    if (vector == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < vector->size; i++) {
        if (areEqual(vector->data[i], value)) {
            return vector->data[i];
        }
    }

    return NULL;
}

bool pushToVector(Vector *vector, void *value) {
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

    if (vector->size == 0) {
        return;
    }

    vector->size--;

    if (deleteValue != NULL) {
        deleteValue(vector->data[vector->size]);
    }
}

bool deleteFromVector(Vector *vector, void deleteValue(void *), bool areEqual(void *, void *), void *value) {
    if (vector == NULL) {
        return false;
    }

    if (value == NULL) {
        return false;
    }

    size_t size = vector->size;

    for (size_t position = 1; position <= size; position++) {
        if (areEqual(vector->data[size - position], value)) {
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