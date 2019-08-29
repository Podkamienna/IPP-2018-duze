/** @file
 * Interfejs struktury string, reprezentującej napis o zmiennej długości.
 *
 * @author Alicja Ziarko <ala.ziarko@gmail.com>
 * @date 11.08.2019
 */

#ifndef DROGI_STRING_H
#define DROGI_STRING_H


#include <stdlib.h>
#include <stdbool.h>

/**
 * Struktura przechowująca napis.
 */
typedef struct String String;

/**
 * @brief Tworzy nową strukturę przechowującą napis.
 * @return Wskaźnik na nową strukturę lub NULL, gdy nie udało
 * się zaalokować pamięci.
 */
String *initializeString();

/**
 * @brief Usuwa strukturę przechowującą napis,
 * @param string — struktura z napisem do usunięcia
 * @param deleteData — parametr wskazujący na to, czy usunąć napis
 */
void deleteString(String *string, bool deleteData);

/**
 * @brief Zwraca napis trzymany przez
 * zadaną strukturę.
 * @param string — struktura trzymająca napis
 * @return Napis trzymany przez strukturę.
 */
char *getData(String *string);

/**
 * @brief Zamienia liczbę całkowitą na napis.
 * Alokuje pamięć na niego i zwraca wskaźnik
 * na tak uzyskaną strukturę.
 * @param number — liczba do zamienienia
 * @return wartość @p NULL jeżeli nie udało się zaalokować pamięci.
 * Utworzony napis, jeżeli wszystko poszło dobrze.
 */
char *intToString(int number);

/**
 * @brief Zamienia liczbę całkowitą bez znaku
 * na napis. Alokuje pamięć na niego i zwraca wskaźnik
 * na tak uzyskaną strukturę.
 * @param number — liczba do zamienienia
 * @return wartość @p NULL jeżeli nie udało się zaalokować pamięci.
 * Utworzony napis, jeżeli wszystko poszło dobrze.
 */
char *unsignedToString(unsigned number);

/**
 * @brief Dodaje zadaną wartość na koniec napisu.
 * @param string1 — struktura z napisem do którego coś ma być dodane
 * @param string2 — wartość do dodania
 * @return Wartość @p true jeżeli uda się dodać wartość,
 * wartość @p false, gdy coś nie wyjdzie: któryś z parametrów
 * ma niepoprawną wartość lub nie udało się zaalokować pamięci
 */
bool concatenateString(String *string1, const char *string2);


#endif //DROGI_STRING_H
