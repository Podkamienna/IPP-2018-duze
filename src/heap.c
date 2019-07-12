//
// Created by alicja on 10.07.19.
//

#include "heap.h"
#include "vector.h"

typedef struct Heap Heap;

struct Heap {
    Vector *vector;

    int (*compare)(void *, void *);
};

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


#include <stdio.h> // TODO usunąć

void seeHeap(Heap *heap) {
    // void *pom = NULL;
    for (size_t i = 0; i < heap->vector->size; i++) {
        //pom = (heap->vector->data)[i];
        printf("%lu\n", i);
    }
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
    if (heap == NULL || !pushVector(heap->vector, value)) {
        return false;
    }

    size_t position = heap->vector->size - 1;
    void **data = heap->vector->data;

    while (position != 0) { //TODO???, warunek do ciała wrzucićć, zmiennna opomcnicza parent
        size_t parent = getParent(position);
        if (heap->compare(data[parent], data[position]) > 0) {
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

        if (heap->compare(data[position], data[minChild]) > 0) { //???
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