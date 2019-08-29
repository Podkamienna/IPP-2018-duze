/** @file
 * Interfejs struktury zbiór.
 *
 * @author Alicja Ziarko <ala.ziarko@gmail.com>
 * @date 05.07.2019
 */

#ifndef DROGI_SET_H
#define DROGI_SET_H

#include <stdbool.h>
#include <stdlib.h>

/**
 * Struktura przechowująca zbiór
 */
typedef struct Set Set;

/**
 * Struktura przechowująca iterator na zbiór
 */
typedef struct SetIterator SetIterator;

/**
 * @brief Funkcja alokująca pamięć pod i zwracająca nowy Set.
 * @return Nowy Set lub NULL, gdy nie udało się zaalokować pamięci.
 */
Set *initializeSet(bool areEqual(void *, void *));

/**
 * @brief Usuwa zbiór.
 * @param set — zbiór do usunięcia
 * @param deleteValue — funkcja usuwająca elementy zbioru
 */
void deleteSet(Set *set, void deleteValue(void *));

/**
 * @brief Zwraca rozmiar zbioru.
 * @param set — zbiór, którego rozmiar ma być zwrócony
 * @return Wartość @p 0, gdy przekazany paramentr to NULL,
 * ilość elementów w zbiorze w innym razie.
 */
size_t getSetSize(Set *set);

/**
 * @brief Funkcja wyszukująca w Set zadanej wartości.
 * @param set — zbiór w którym będzie szukane
 * @param cmp — funkcja do porównywanie wartości
 * @param value — szukana wartość
 * @return Zwraca znalezioną wartość lub NULL, gdy się nie udało jej znaleźć,
 * lub set jest NULLem
 */
void *searchSet(Set *set, void *value);

/**
 * @brief Dodaje zadaną wartość do zbioru.
 * @param set — zbiór do którego będzie dodawane
 * @param value — wartość do dodania
 * @return Wartość @p true jeżeli dodawanie się powiodło, false,
 * jeżeli coś poszło źle: zadany zbiór lub wartość były NULL,
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
bool deleteFromSet(Set *set, void deleteValue(void *), void *value);

/**
 * @brief Alokuje pamięć pod i zwraca wskaźnik na
 * nowy iterator na zadanym wektorze.
 * @param vector — wektor dla którego tworzony będzie
 * iterator
 * @return Wskaźnik na zaalokowaną strukturę, jeżeli wszystko się udało,
 * @p NULL, jeżeli nie udało się zaalokować pamięci, lub zadany wektor jest
 * NULLem.
 */
SetIterator *getNewSetIterator(Set *set);

/**
 * @brief Funkcja usuwająca zadany iterator wektora.
 * @param vectorIterator — iterator do usunięcia.
 */
void deleteSetIterator(SetIterator *setIterator);

/**
 * @brief Zwraca kolejny element wektora.
 * @param vectorIterator — iterator, który
 * wie, jaki element zwrócić
 * @return Kolejny element wektora, lub wartość
 * @p NULL, jeżeli koniec wektora został osiągnięty
 */
void *getNextSetIterator(SetIterator *setIterator);

#endif /* DROGI_SET_H */
