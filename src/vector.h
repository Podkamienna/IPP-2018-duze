/** @file
 */

#ifndef DROGI_VECTOR_H
#define DROGI_VECTOR_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct Vector Vector;

struct Vector {
    size_t size;
    size_t maxSize;
    void **data;
};

/**
 * @brief Zwiększa rozmiar wektora.
 * @param vector — wektor do powiększenia
 * @return Wartość @p true jeżeli udało się powiększyć
 * wektor, false gdy któryś z parametrów
 * ma niepoprawną wartość lub nie udało
 * się zaalokować pamięci.
 */
bool resizeVector(Vector *vector);

/**
 * @brief Tworzy nowy wektor.
 * @return Nowy wektor lub NULL, gdy nie udało
 * się zaalokować pamięci.
 */
Vector *initializeVector();

/**
 * @brief Wyszukuje i zwraca pola w wektorze związanego z
 * z zadaną wartością.
 * @param vector — wektor w którym jest szukane
 * @param cmp — funkcja sprawdzająca, czy 2 wartości są równe
 * @param value — wartość która jest wyszukiwana
 * @return Zwraca znalezioną wartość lub NULL, gdy nie jest ona
 * obecna w wektorze, lub któryś z parametrów
 * ma niepoprawną wartość
 */
void *searchVector(Vector *vector, int cmp(void *, void *), void *value);

/**
 * @brief Dodaje zadaną wartość na koniec wektora.
 * @param vector — wektor do którego coś ma być dodane
 * @param value — wartość do dodanie
 * @return Wartość @p true jeżeli uda się dodać wartość,
 * wartość @p false, gdy coś nie wyjdzie: któryś z parametrów
 * ma niepoprawną wartość lub nie udało się zaalokować pamięci
 */
bool pushVector(Vector *vector, void *value);

/**
 * @brief Usuwa element z końca wektora.
 * @param vector — wektor z którego ma być usuwane
 * @param deleteValue — funkcja, która usuwa zawartość pola w wektorze
 */
void popFromVector(Vector *vector, void deleteValue(void *));

/**
 * @brief Usuwa wektor.
 * @param vector — wektor do usunięcia
 * @param deleteValue — funkcja, która usuwa zawartość pola w wektorze
 */
void deleteVector(Vector *vector, void deleteValue(void *));


#endif /* DROGI_VECTOR_H */
