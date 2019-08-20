//
// Created by alicja on 01.08.2019.
//

#ifndef DROGI_LIST_H
#define DROGI_LIST_H

#include <stdbool.h>

typedef struct ListNode ListNode;
typedef struct List List;
typedef struct ListIterator ListIterator;

struct ListNode {
    void *data;
    ListNode *next;
    ListNode *prev;
};

struct List {
    ListNode *beginning; //wskaźnik na początek listy
    ListNode *end; //wskaźnik na koniec listy
    int (*compare)(void *, void *);
};

struct ListIterator {
    ListNode *listNode;
};

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
 * @brief Odwraca zadaną listę
 * @param list — lista do odwrócenia
 */
void reverseList(List *list);
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

bool insertAtTheBeginning(List *list, List *toInsert, void deleteValue(void *));

bool insertAtTheEnd(List *list, List *toInsert, void deleteValue(void *));

/**
 * @brief Ws
 * @param list
 * @param toInsert
 * @return
 */
bool insertToList(List *list, List *toInsert, void deleteValue(void *));

/**
 * @brief Usuwa zadaną listę. Jeżeli deleteValue to NULL,
 * to nie usuwa elementów listy.
 * @param list — lista, która ma zostać usunięta
 * @param deleteValue — funkcja usuwająca elementy listy
 */
void deleteList(List *list, void deleteValue(void *));

void *getLast(List *list);

ListIterator *getNewListIterator(List *list);

bool incrementListIterator(ListIterator *listIterator);

void *getNextListIterator(ListIterator *listIterator);

void deleteListIterator(ListIterator *listIterator);


#endif //DROGI_LIST_H
