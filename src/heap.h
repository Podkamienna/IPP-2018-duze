//
// Created by alicja on 10.07.19.
//

#ifndef DROGI_HEAP_H
#define DROGI_HEAP_H

#include <stdbool.h>

typedef struct Heap Heap;

/**
 * @brief Alokuje pamięć pod i tworzy nowy kopiec.
 * @param compare — funkcja do porównywania elementów kopca
 * @return Wskaźnik na stworzoną strukturę lub NULL, gdy nie udało
 * się zaalokować pamięci.
 */
Heap *initializeHeap(int compare(void *, void *));

/**
 * @brief Wkłada nową wartość na kopiec.
 * @param heap — kopiec na który ma być wkładane
 * @param value — wartość do włożenia
 * @return Wartość @p true jeżeli wkładanie się powiodło
 * wartość false, jeżeli nie udało się zaalokować pamięci
 * lub któryś z parametrów ma niepoprawną wartość
 */
bool pushHeap(Heap *heap, void *value);

/**
 * @brief Usuwa z kopca element o najmniejszym priorytecie.
 * @param heap — kopiec z którego element ma być usunięty
 * @return Usuwany element lub NULL jeżeli któryś z parametrów
 * ma niepoprawną wartość
 */
void *popHeap(Heap *heap);

void seeHeap(Heap *heap);

/**
 * @brief Usuwa kopiec.
 * @param heap — kopiec do usunięcia
 * @param deleteValue — funkcja usuwająca element kopca
 */
void deleteHeap(Heap *heap, void deleteValue(void *));

#endif //DROGI_HEAP_H
