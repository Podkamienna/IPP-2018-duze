/**
 * @file Implementacja struktury będącej kopcem, zaimplementowanym w wersji tablicowej.
 */

#include "heap.h"
#include "vector.h"

#include <stdbool.h>
#include <stdlib.h>

/**
 * Struktura będąca kopcem.
 */
typedef struct Heap Heap;

struct Heap {
    Vector *vector; ///< wektor, na którym jest zaimplementowany kopiec
    int (*compare)(void *, void *); ///< funkcja do porównywania elementów kopca
};

/**
 * @brief Zwraca indeks rodzica elementu kopca z zadanym indeksem.
 * @param i — indeks, którego rodzic jest poszukiwany
 * @return Znaleziony indeks.
 */
static size_t getParent(size_t i);

/**
 * @brief Zwraca indeks lewego syna elementu kopca z zadanym indeksem.
 * @param i — indeks, którego lewy syn jest poszukiwany
 * @return Znaleziony indeks.
 */
static size_t getLeftChild(size_t i);

/**
 * @brief Zwraca indeks prawego syna elementu kopca z zadanym indeksem.
 * @param i — indeks, którego prawy syn jest poszukiwany
 * @return Znaleziony indeks.
 */
static size_t getRightChild(size_t i);

/**
 * @brief Mając zadane 2 indeksy i kopiec, zwraca ten z nich, któremu
 * przypisana jest wartość niebędąca NULLem, i jeżeli oba takie są, to
 * zwraca ten któremu przypisana jest wartość mniejsza. Jeżeli oba mają
 * wartość NULL, to zwraca pierwszy z nich.
 * @param heap — kopiec w którym sprawdzane są wartości
 * @param i — indeks, któremu przypisana jest pierwsza wartość
 * @param j — indeks, któremu przypisana jest druga wartość
 * @return Wybrany wedle opisu indeks.
 */
static size_t minNotNull(Heap *heap, size_t i, size_t j);

/**
 * @brief Zamienia wartości będące w zadanej tablicy pod zadanymi indeksami.
 * @param arr — tablica w której są wartości
 * @param i — pierwszy indeks
 * @param j — drugi indeks
 */
static void swap(void **arr, size_t i, size_t j);

static size_t getParent(size_t i) {
    return (i - 1) / 2;
}

static size_t getLeftChild(size_t i) {
    return 2 * i + 1;
}

static size_t getRightChild(size_t i) {
    return 2 * i + 2;
}

static size_t minNotNull(Heap *heap, size_t i, size_t j) {
    if (j >= heap->vector->size) {
        return i;
    }

    if (heap->compare(heap->vector->data[i], heap->vector->data[j]) <= 0) {
        return i;
    }

    return j;
}

static void swap(void **arr, size_t i, size_t j) {
    void *tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
}

bool isEmptyHeap(Heap *heap) {
    if (heap == NULL) {
        return true;
    }

    return isEmptyVector(heap->vector);
}

Heap *initializeHeap(int compare(void *, void *)) {
    Heap *newHeap = malloc(sizeof(Heap));

    if (newHeap == NULL) {
        return NULL;
    }

    newHeap->vector = initializeVector();

    if (newHeap->vector == NULL) {
        free(newHeap);
        return NULL;
    }

    newHeap->compare = compare;

    return newHeap;

}

bool pushHeap(Heap *heap, void *value) {
    if (heap == NULL || !pushToVector(heap->vector, value)) {
        return false;
    }

    size_t position = heap->vector->size - 1;
    void **data = heap->vector->data;

    while (true) {
        size_t parent = getParent(position);

        if (position != 0 && heap->compare(data[parent], data[position]) > 0) {
            swap(data, position, parent);
            position = parent;
        } else {
            break;
        }
    }

    return true;
}

void *popHeap(Heap *heap) {
    if (heap == NULL) {
        return NULL;
    }

    void **data = heap->vector->data;
    size_t position = 0;
    void *top = data[0];


    swap(data, 0, heap->vector->size - 1);
    popFromVector(heap->vector, NULL);

    while (getLeftChild(position) < heap->vector->size) {
        size_t minChild = minNotNull(heap, getLeftChild(position), getRightChild(position));

        if (heap->compare(data[position], data[minChild]) > 0) {
            swap(data, position, minChild);
            position = minChild;
        } else {
            break;
        }
    }

    return top;
}

void deleteHeap(Heap *heap, void deleteValue(void *)) {
    if (heap == NULL) {
        return;
    }

    deleteVector(heap->vector, deleteValue);
    free(heap);
}