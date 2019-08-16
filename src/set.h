/** @file
 */

#ifndef DROGI_SET_H
#define DROGI_SET_H

#include <stdbool.h>
#include "vector.h"

typedef struct Set Set;
typedef struct SetIterator SetIterator;

struct SetIterator {
    VectorIterator *vectorIterator;
};
//TODO usunac
/**
 * @brief Funkcja alokująca pamięć pod i zwracająca nowy Set.
 * @return Nowy Set lub NULL, gdy nie udało się zaalokować pamięci.
 */
Set *initializeSet(int compare(void *, void *));

/**
 * @brief Funkcja wyszukująca w Set zadanej wartości.
 * @param set — zbiór w ktorym będzie szukane
 * @param cmp — funkcja do porównywanie wartości
 * @param value — szukana wartość
 * @return Zwraca znalezioną wartość lub NULL, gdy się nie udało jej znaleźć,
 * lub set jest NULLem
 */
void *searchSet(Set *set, void *value);

/**
 * @brief Dodaje zadaną wartość do setu.
 * @param set — Set do którego będzie dodawane
 * @param value — wartość do dodania
 * @return Wartość @p true jeżeli dodawanie się powiodło, false,
 * jeżeli coś poszło źle: zadany zbiór lub wartość były NULLami,
 * albo nie udało się zaalokować pamięci
 */
bool insertSet(Set *set, void *value);

/**
 * @brief Usuwa zadany element ze zbioru.
 * @param set — zbiór z którego będzie usuwane
 * @param deleteValue — funkcja usuwająca element zbioru
 * @param value — element do usunięcia
 * @return Wartość @p true jeżeli udało się usunąć.
 * Wartość @p false, jeżeli element do usunięcia nie jest zawarty
 * w zbiorze.
 */
 //może jednak void? TODO
bool deleteFromSet(Set *set, void deleteValue(void *), void *value);

/**
 * @brief Usuwa zbiór.
 * @param set — zbiór do usunięcia
 * @param deleteValue — funkcja usuwająca elementy zbioru
 */
void deleteSet(Set *set, void deleteValue(void *));

SetIterator *getNewSetIterator(Set *set);

bool incrementSetIterator(SetIterator *setIterator);

void *getNextSetIterator(SetIterator *setIterator);

void deleteSetIterator(SetIterator *setIterator);

#endif /* DROGI_SET_H */
