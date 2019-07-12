//
// Created by alicja on 10.07.19.
//
#include <stdio.h>

#include "heap.h"
#include "vector.h"

typedef struct Heap Heap;

struct Heap {
    Vector *vector;
    int (*compare) (void *, void *);
};

static size_t parent(size_t i) {
    return (i - 1) / 2;
}

static size_t leftChild(size_t i) {
    return 2 * i + 1;
}

static size_t rightChild(size_t i) {
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

static void swap(void **arr, int i, int j) {
    void *tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
}

void seeHeap(Heap *heap) {
    void *pom = NULL;
    for(int i = 1; i < heap->vector->size; i++) {
        pom = (heap->vector->data)[i];
        printf("%d", i);
    }
    int k;
}

Heap *initializeHeap(int compare (void *, void *)) {
    Heap *newHeap = malloc(sizeof(Heap));

    if (newHeap == NULL) {
        return NULL;
    }

    newHeap->vector = initializeVector();

    if (newHeap->vector == NULL) {
        free(newHeap);
        return NULL;
    }

    newHeap->compare =  compare;

    return newHeap;

}

bool pushHeap(Heap *heap, void *value) {
    if (heap == NULL || !pushVector(heap->vector, value)) {
        return false;
    }

    size_t position = heap->vector->size - 1;

    while (position != 0 && heap->compare(heap->vector->data[parent(position)], heap->vector->data[position]) > 0) { //TODO???, warunek do ciała wrzucićć, zmiennna opomcnicza parent
        swap(heap->vector->data, position, parent(position));
        position = parent(position);
    }

    return true;
}

void *popHeap(Heap *heap) {
    if (heap == NULL) {
        return NULL;
    }

    void *top = heap->vector->data[0];
    size_t position = 0;

    swap(heap->vector->data, 0, heap->vector->size);
    popFromVector(heap->vector, NULL);

    while (leftChild(position) < heap->vector->size ) {
        size_t minChild = minNotNull(heap, leftChild(position), rightChild(position));

        if (heap->compare(heap->vector->data[minChild], heap->vector->data[position]) < 0) { //???
            swap(heap->vector->data, position, minChild);
            position = minChild;
        }

        else {
            break;
        }
    }

    return top;
}

void deleteHeap(Heap *heap, void deleteValue(void *)) {

}