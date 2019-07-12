//
// Created by alicja on 12.07.19.
//

#include <stdio.h>
#include <stdlib.h>

#include "heap.h"

int comp(int *a, int *b) {
    return *a - *b;
}
//zwraca > 0 gdy a > b, < 0 b > a

int main() {
    Heap *heap = initializeHeap(comp);
    int *a = malloc(sizeof(int)), *b = malloc(sizeof(int)), *c = malloc(sizeof(int)), *d = malloc(sizeof(int)),*e, *f, *g,*h, *i, *j;

    *a = 1;

    *b = 2;
    *c = 3;
    *d = 4;

    seeHeap(heap);
    pushHeap(heap, a);
    seeHeap(heap);
    pushHeap(heap, c);
    seeHeap(heap);
    pushHeap(heap, b);
    seeHeap(heap);
    pushHeap(heap, d);
    seeHeap(heap);
    popHeap(heap);
    seeHeap(heap);
    popHeap(heap);
    seeHeap(heap);
    popHeap(heap);
    seeHeap(heap);
    popHeap(heap);
    seeHeap(heap);


}