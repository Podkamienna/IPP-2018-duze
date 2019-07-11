//
// Created by alicja on 10.07.19.
//

#ifndef DROGI_HEAP_H
#define DROGI_HEAP_H

#include <stdbool.h>

typedef struct Heap Heap;

Heap *initializeHeap();

bool pushHeap(Heap *heap, void *value); //jaki typ?

void *popHeap(Heap *heap);

void deleteHeap(Heap *heap, void deleteValue(void *));

#endif //DROGI_HEAP_H
