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
    ListNode *beginning; // wskaźnik na początek listy
    ListNode *end; // wskaźnik na koniec listy
};

struct ListIterator {
    ListNode *listNode;
};

/**
 * @brief Alokuje pamięć pod i zwraca wskaźnik na
 * nową listę.
 * @return Wskaźnik na zaalokowaną strukturę, lub NULL,
 * jeżeli nie udało się zaalokować pamięci lub parametr ma
 * wartość NULL.
 */
List *initializeList();

/**
 * @brief Usuwa zadaną listę. Jeżeli deleteValue to NULL,
 * to nie usuwa elementów listy.
 * @param list — lista, która ma zostać usunięta
 * @param deleteValue — funkcja usuwająca elementy listy
 */
void deleteList(List *list, void deleteValue(void *));

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
void *searchList(List *list, void *value, bool areEqual(void *, void *));

/**
 * @brief Funkcja łącząca listę do wstawienia z zadaną listą tak, że
 * jeżeli ostatni element list do wstawienia jest taki jak pierwszy
 * element listy, to wstawia ten pierwszy element zamiast ostatniego i robi
 * z nich jedną listę.
 * @param list — lista do której będzie dołączane
 * @param toInsert — lista, która będzie dołączana
 * @param deleteValue — funkcja usuwająca ostatni element listy do wstawienia
 * @return Wartość @p true, jeżeli udało się połączyć listy.
 * Wartość @p false, jeżeli zawartości końca drugiej listu i początku pierwszej
 * nie są równe lub któraś z list ma wartość NULL.
 */
bool insertAtTheBeginningList(List *list, List *toInsert, void (*deleteValue)(void *));

/**
 * @brief Funkcja łącząca listę do wstawienia z zadaną listą tak, że
 * jeżeli pierwszy element list do wstawienia jest taki jak ostatni
 * element listy, to wstawia ten pierwszy element zamiast ostatniego i robi
 * z nich jedną listę.
 * @param list — lista do której będzie dołączane
 * @param toInsert — lista, która będzie dołączana
 * @param deleteValue — funkcja usuwająca ostatni element listy
 * @return Wartość @p true, jeżeli udało się połączyć listy.
 * Wartość @p false, jeżeli zawartości początku drugiej listu i końca pierwszej
 * nie są równe lub któraś z list ma wartość NULL.
 */
bool insertAtTheEndList(List *list, List *toInsert, void (*deleteValue)(void *));

/**
 * @brief Mając dane dwie listy o długości co najmniej 2, sprawdza koniec i początek tej do wstawienia,
 * a następnie sprawdza, czy w liście do której ma wstawiać istnieją 2 kolejne elementy, które
 * jako ciąg są równe ciągowi początek, koniec z listy do wstawienia. Jeżeli znajdzie takie
 * elementy to wstawia pomiędzy nie listę do wstawienia, przy czym usuwa ostatni element z listy
 * do wstawienia i pierwszy przed nią.
 * @param list — lista do której ma być wstawiane
 * @param toInsert — lista, która ma być wstawiana
 * @param deleteValue — funkcja usuwająca zawartość z list
 * @param areEqual — funkcja porównująca zawartości list
 * @return Wartość @p true, jeżeli wstawianie się powiodło, wartość @p false, jeżeli parametry miały
 * nieodpowiednią wartość lub nie udało się znaleźć szukanej pary w liście do której wykonywana była
 * próba wstawienia.
 */
bool insertToList(List *list, List *toInsert, void deleteValue(void *), bool areEqual(void *, void *));

/**
 * @brief Alokuje pamięć na i zwraca nowy iteratory do zadanej listy.
 * @param list — lista do której iterator będzie tworzony
 * @return Wskaźnika na utworzoną strukturę, jeżeli lista była NULLem
 * lub nie udało się zaalokować pamięci.
 */
ListIterator *getNewListIterator(List *list);

/**
 * @brief Zwraca kolejną wartość zawartą w liście.
 * @param listIterator — iterator na listę z której ma być
 * zwrócona wartość
 * @return Kolejna wartość z listy lub NULL, jeżeli lista
 * została już przejrzana.
 */
void *getNextListIterator(ListIterator *listIterator);

/**
 * @brief Usuwa iterator na listę.
 * @param listIterator — iterator do usunięcia
 */
void deleteListIterator(ListIterator *listIterator);


#endif //DROGI_LIST_H
