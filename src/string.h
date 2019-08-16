//
// Created by alicja on 11.08.2019.
//

#ifndef DROGI_STRING_H
#define DROGI_STRING_H

/** @file
 */

#include <stdlib.h>
#include <stdbool.h>

typedef struct String String;

struct String {
    size_t size;
    size_t maxSize;
    char *data;
};

/**
 * @brief Tworzy nową strukturę przechowującą napis.
 * @return Wskaźnikna nową strukturę lub NULL, gdy nie udało
 * się zaalokować pamięci.
 */
String *initializeString();

/**
 * Zwraca napis trzymany przez
 * zadaną strukturę.
 * @param string — struktura trzumająca napis
 * @return Napis trzymany przez strukturę.
 */
char *getData(String *string);

char *numberToString(int number);

/**
 * @brief Dodaje zadaną wartość na koniec napisu.
 * @param string — struktura z napisem do którego coś ma być dodane
 * @param value — wartość do dodania
 * @return Wartość @p true jeżeli uda się dodać wartość,
 * wartość @p false, gdy coś nie wyjdzie: któryś z parametrów
 * ma niepoprawną wartość lub nie udało się zaalokować pamięci
 */
bool concatenateString(String *string1, const char *string2);

/**
 * @brief Usuwa strukturę przechowującą napis,
 * @param string — struktura z napisem do usunięcia
 * @param deleteData — parametr wskazujący na to, czy usunąć napis
 * @param deleteValue — funkcja, która usuwa zawartość pola w wektorze
 */
void deleteString(String *string, bool deleteData);

#endif //DROGI_STRING_H
