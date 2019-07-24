/** @file
 *
 */

#ifndef DROGI_HASH_TABLE_H
#define DROGI_HASH_TABLE_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct HashTable HashTable;

/**
 * @brief Alokuje pamięć pod i zwraca wskaźnik na
 * nową hash tablicę.
 * @param initialSize — początkowy rozmiar hash tablicy
 * @return Wskaźnik na nową hash tablicę, jeżeli się wszystko udało
 * i NULL jeżeli któryś z parametrów ma niepoprawną wartość
 * lub nie udało się zaalokować pamięci.
 */
HashTable *initializeHashTable(size_t initialSize);

/**
 * @brief Wyszukuje pola o danej nazwie w
 * zadanej hash tablicy.
 * @param hashTable — hash tablica, w której będzie wyszukiwane
 * @param name — nazwa wyszukiwanego pola
 * @return Pole hash tablicy lub NULL jeżeli któryś z parametrów jest
 * nieprawidłowy lub szukane pole nie istnieje
 */
void *searchHashTable(HashTable *hashTable, const char *name);

/**
 * @brief Dodaje element do hash tablicy, jeżeli go nie ma w tej tablicy
 * @param hashTable — hash tablica do której element jest dodawany
 * @param name — nazwa przyporządkowana do wartości, którama być dodana
 * @param value — wartość która ma być dodana to hash tablicy
 * @return Wartość @p true jeżeli udało się dodać element.
 * Wartość @p false, jeżeli coś się nie powiodło: nie udało się zaalokować
 * pamięci, któryś z parametrów miał niepoprawną wartość lub nazwa ma już
 * przypisaną wartość w hash tablicy.
 */
bool insertHashTable(HashTable *hashTable, const char *name, void *value);

void iterate(HashTable *hashTable, void fun(void *)); //TODO usunac

/**
 * @brief Zwiększa rozmiar hash tablicy do żądanego rozmiaru.
 * @param hashTable — hash tablica, której rozmiar ma być zmieniony
 * @param newSize — nowy rozmiar hash tablicy
 * @return Wartość @p true, jeżeli udało się zwiększyć rozmiar tablicy
 * wartość @p false, jeżeli nie udało się zaalokować pamięci lub parametry
 * mają niepoprawną wartość
 */
HashTable *resizeHashTable(HashTable *hashTable, size_t newSize);

/**
 * @brief Usuwa wartość przypisaną do danego napisu z hash tablicy.
 * @param hashTable — tablica z której jest usuwane
 * @param name — napis do którego jest przypisana usuwana wartość
 * @param deleteValue — funkcja usuwająca wartość
 */
void deleteFromHashTable(HashTable *hashTable, const char *name, void deleteValue(void *));

/**
 * @brief Usuwa hash tablicę.
 * @param hashTable — tablica, która ma być usunięta
 * @param deleteValue — funkcja usuwająca wartość
 */
void deleteHashTable(HashTable *hashTable, void deleteValue(void *));

#endif /* DROGI_HASH_TABLE_H */
