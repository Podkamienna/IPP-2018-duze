/** @file
 * Interfejs struktury wektor, reprezentującej dynamicznie alokowane tablice
 * o zmiennym rozmiarze.
 */

#ifndef DROGI_VECTOR_H
#define DROGI_VECTOR_H

#include <stdlib.h>
#include <stdbool.h>

/** Struktura przechowująca wektor. */
typedef struct Vector Vector;
/** Struktura przechowująca iterator po wektorze*/
typedef struct VectorIterator VectorIterator;

struct Vector {
    size_t size;
    size_t maxSize;
    void **data;
};

struct VectorIterator {
    size_t position;
    Vector *vector;
};

/**
 * @brief Tworzy nowy wektor.
 * @return Nowy wektor lub @p NULL, gdy nie udało
 * się zaalokować pamięci.
 */
Vector *initializeVector();

/**
 * @brief Usuwa wektor. Jeżeli parametr deleteValue
 * ma wartość NULL, to nie usuwa zawartości wektora.
 * @param vector — wektor do usunięcia
 * @param deleteValue — funkcja, która usuwa zawartość pola w wektorze
 */
void deleteVector(Vector *vector, void deleteValue(void *));

/**
 * @brief Ustawia ilość zajętych pól w wektorze na 0.
 * @param vector — wektor który będzie edytowany
 */
void resetVector(Vector *vector);

/**
 * @brief Wyszukuje i zwraca pole w wektorze związane z
 * z zadaną wartością.
 * @param vector — wektor w którym jest szukane
 * @param areEqual — funkcja zadająca porządek na elementach wektora
 * @param value — wartość która jest wyszukiwana
 * @return Zwraca znalezioną wartość lub @p NULL, gdy nie jest ona
 * obecna w wektorze, lub któryś z parametrów
 * ma niepoprawną wartość
 */
void *searchVector(Vector *vector, bool areEqual(void *, void *), void *value);

/**
 * @brief Dodaje zadaną wartość na koniec wektora.
 * @param vector — wektor do którego coś ma być dodane
 * @param value — wartość do dodanie
 * @return Wartość @p true jeżeli uda się dodać wartość,
 * wartość @p false, gdy coś nie wyjdzie: któryś z parametrów
 * ma niepoprawną wartość lub nie udało się zaalokować pamięci
 */
bool pushToVector(Vector *vector, void *value);

/**
 * @brief Usuwa element z końca wektora.
 * @param vector — wektor z którego ma być usuwane
 * @param deleteValue — funkcja, która usuwa zawartość pola w wektorze,
 * jeżeli ma wartość NULL, to zawartość nie jest usuwana
 */
void popFromVector(Vector *vector, void deleteValue(void *));

/**
 * @brief Usuwa zadany element z wektora.
 * @param vector — wektor z którego będzie usuwane
 * @param deleteValue — funkcja usuwająca element wektora
 * @param value — element do usunięcia
 * @return Wartość @p true jeżeli udało się usunąć.
 * Wartość @p false, jeżeli element do usunięcia nie jest zawarty
 * w wektorze lub któryś z parametrów jest niepoprawny.
 */
bool deleteFromVector(Vector *vector, void deleteValue(void *), bool areEqual(void *, void *), void *value);

/**
 * @brief Funkcja sprawdzająca, czy wektor jest pusty.
 * @param vector — wektor do sprawdzenia jego pustości
 * @return Wartość @p true, jeżeli wektor ma rozmiar 0, lub
 * jest NULLem. Wartość @p false w innym wypadku.
 */
bool isEmptyVector(Vector *vector);

/**
 * @brief Alokuje pamięć pod i zwraca wskaźnik na
 * nowy iterator na zadanym wektorze.
 * @param vector — wektor dla którego tworzony będzie
 * iterator
 * @return Wskaźnik na zaalokowaną strukturę, jeżeli wszystko się udało,
 * @p NULL, jeżeli nie udało się zaalokować pamięci, lub zadany wektor jest
 * NULLem.
 */
VectorIterator *getNewVectorIterator(Vector *vector);

/**
 * @brief Funkcja usuwająca zadany iterator wektora.
 * @param vectorIterator — iterator do usunięcia.
 */
void deleteVectorIterator(VectorIterator *vectorIterator);

/**
 * @brief Zwraca kolejny element wektora.
 * @param vectorIterator — iterator, który
 * wie, jaki element zwrócić
 * @return Kolejny element wektora, lub wartość
 * @p NULL, jeżeli koniec wektora został osiągnięty
 */
void *getNextVectorIterator(VectorIterator *vectorIterator);

#endif /* DROGI_VECTOR_H */
