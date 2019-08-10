//
// Created by alicja on 01.08.2019.
//

#ifndef DROGI_LIST_H
#define DROGI_LIST_H

#include <stdbool.h>

typedef struct List List;

/**
 * @brief Alokuje pamięc pod i zwraca wskaźnik na
 * nową listę.
 * @param compare — funkcja pozwalająca na porównywanie
 * elementów zawartych w liście
 * @return Wskaźnik na zaalokowaną strukturę, lub NULL,
 * jeżeli nie udało się zaalokować pamięci lub parametr ma
 * wartość NULL.
 */
List *initializeList(int compare(void *, void *));

/**
 * @brief Alokuje pamięć pod i wstawia na początek listy nowy
 * element.
 * @param list — lista, do której ma być wstawiane
 * @param value — wartość do wstawienia
 * @return Wartość @p true jeżeli wszystko się udało.
 * Wartość @p false, jeżeli coś się nie powiodło - któryś
 * z parametrów ma wartość NULL lub nie udało się zaalokować
 * pamięci.
 */
bool addToList(List *list, void *value);

/**
 * @brief Funkcja sprawdzająca, czy zadana
 * wartość jest obecna w liście.
 * @param list — lista, w której wartość
 * będzie szukana
 * @param value — wartość, która będzie szukana
 * @return Wartość @p true, jeżeli element został znaleziony.
 * Wartość @p false, jeżeli któryś z elementów wejściowych
 * ma wartość NULL lub wyszukiwany element nie istnieje.
 */
bool exists(List *list, void *value);

/**
 * @brief Znajduje w zadanej liście elementy, pomiędzy którymi
 * ma wstawić listę do wstawienia i ją wstawia. Jeżeli zadana
 * para elementów występuje obok siebie więcej niż raz, to
 * wstawia tylko przy pierwszym jej wystąpieniu. Jeżeli początek ma
 * wartość NULL to wstawiwa na początek listy, a jeżeli koniec to
 * na koniec listy. Jeżeli oba mają wartość NULL i lista jest pusta
 * to wstawia zamiast tej listy. Jeżeli list jest pusta i koniec lub
 * początek nie jest NULLem, to nic nie robi
 * @param list — lista do której ma być wstawiane
 * @param toInsert — lista która ma być wstawiana
 * @param beginning — element po którym ma być wstawiona
 * @return Wartość @p true, jeżeli udało się wstawić.
 * Wartość @p false, jeżeli któraś z list ma wartość NULL,
 * początek lub koniec listy do wstawienia nie są zawarte
 * w liście, początek i koniec listy nie są 2 kolejnymi elementami
 * listy, z których początek jest pierwszy. Jeżeli początek i koniec
 * mają wartość NULL, a zadana lista jest niepusta, to zwraca false.
 */
bool insertToList(List *list, List *toInsert);

/**
 * @brief Usuwa zadaną listę. Jeżeli deleteValue to NULL,
 * to nie usuwa elementów listy.
 * @param list — lista, która ma zostać usunięta
 * @param deleteValue — funkcja usuwająca elementy listy
 */
void deleteList(List *list, void deleteValue(void *));

#endif //DROGI_LIST_H
