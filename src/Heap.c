//
// Created by alicja on 10.07.19.
//

#include "Heap.h"
#include "vector.h"

typedef struct Heap Heap;

struct Heap {
    Vector *vector;
    int (*compare) (void *, void *);
};

static int parent(int i) {
    return (i - 1) / 2;
}

static int leftChild(int i) {
    return 2 * i + 1;
}

static int rightChild(int i) {
    return 2 * i + 2;
}

static int minNotNull(Heap *heap, int i, int j) {
    if (heap->vector->data[i] == NULL && heap->vector->data[j] == NULL) {
        return -1;
    }


}

static void swap(void *a, void *b) { //czy ta funckcja działa?
    void *c = a;
    a = b;
    b = c;
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

    newHeap->compare = compare;

    return newHeap;

}

bool pushHeap(Heap *heap, void *value) {
    if (heap == NULL || !pushVector(heap->vector, value)) {
        return false;
    }

    size_t position = heap->vector->size;

    while (position != 0 && heap->compare(heap->vector->data[parent(position)], heap->vector->data[position]) > 0) { //???
        swap(heap->vector->data[position], heap->vector->data[parent(position)]);
        position = parent(position);
    }
}

void *popHeap(Heap *heap) {
    if (heap == NULL)
        return NULL;

    swap(heap->vector->data[0], heap->vector->data[heap->vector->size]);
    deleteFromVector(heap->vector, NULL);

    size_t position = 0;//////////////////////////////////////////////////

    while (leftChild(position) <= heap->vector->size && rightChild(position) <= heap->vector->size) {
        int minChild = minNotNull(heap, leftChild(position), rightChild(position));

        if (heap->compare(heap->vector->data[minChild], heap->vector->data[position]) < 0) { //???
            swap(heap->vector->data[position], heap->vector->data[minChild]);
            position = minChild;
        }

        else {
            break;
        }
    }

    return heap->vector->data[heap->vector->size + 1];
}

void deleteHeap(Heap *heap, void deleteValue(void *));