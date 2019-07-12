//
// Created by alicja on 10.07.19.
//

#ifndef DROGI_HEAP_H
#define DROGI_HEAP_H

#include <stdbool.h>

typedef struct Heap Heap;

Heap *initializeHeap(int compare (void *, void *));

bool pushHeap(Heap *heap, void *value);

void *popHeap(Heap *heap);

void seeHeap(Heap *heap);

void deleteHeap(Heap *heap, void deleteValue(void *));

#endif //DROGI_HEAP_H
