//
// Created by alicja on 04.07.19.
//

#ifndef DROGI_CITIESANDROADS_H
#define DROGI_CITIESANDROADS_H

#include "definitions.h"

/**
 * @brief Sprawdza, czy zadany napis jest poprawną nazwą miasta.
 * @param city — wskaźnik na napis do sprawdzenia
 * @return Wartość @p true jeżeli napis jest poprawną nazwą miasta.
 * Wartość @p false jeżeli city ma wartość NULL lub nie jest
 * poprawną nazwą miastą.
 */
bool isCityName(const char *city);

/**
 * @brief Alokuje pamięc pod i zwraca nowe miasto.
 * @param name — nazwa nowego miasta
 * @return Wartość @p true jeżeli udało się dodać miasto.
 * Wartość @p false jeżeli nie udało się zaalokować pamięci,
 * lub jeżeli zadany napis nie jest poprawną nazwą miasta.
 */
City *getNewCity(const char *name);

/**
 * @brief Wyszukuje miasta zadanego danym napisem w danej mapie.
 * @param map — mapa w której ma być wyszukiwane
 * @param city — napis zadający miasto, które ma być wyszukane
 * @return Wyszukiwane miasto jeżeli wszystko się powiodło,
 * NULL, jeżeli coś się nie powiodło - miasto nie istnieje
 * w mapie lub parametry wejściowe są nieprawidłowe.
 */
City *searchCity(Map *map, const char *city);

/**
 * @brief Zmienia zadane miasto na oznaczone jako nie odwiedzone
 * @param city — miasto do ododwiedzenia
 */
void unvisit(City *city);

/**
 * @brief Sprawdza, czy miasto było odwiedzone
 * @param city — wskaźnik na miasto do sprawdzenia, czy był odwiedzone
 * @return Wartość @p true jeżeli miasto nie jest odwiedzone,
 * Wartość @p false, jeżeli miasto jest odwiedzone, lub zadany wskaźnik
 * ma wartość NULL.
 */
bool isVisited(City *city);

/**
 * @brief Usuwa zadane miasto.
 * @param city — miasto do usunięcia
 */
void deleteCity(City *city);

/**
 * @brief Alokuje pamieć pod i zwraca wskaźnik na nową drogę.
 * @param year — rok budowy nowej drogi
 * @param length — długość nowej drogi
 * @param city1 — jeden koniec nowej drogi
 * @param city2 — drugi koniec nowej drogi
 * @return Wskaźnik na nową drogę, jeżeli wszystko się uda.
 * Null jeżeli nie udało się zaalokować pamięci.
 */
Road *getNewRoadPointer(int year, int length, City *city1, City *city2);

/**
 * @brief Dodaje nową drogę do mapy.
 * @param map — mapa do której będzie dodawane
 * @param city1 — jeden koniec dodawanej drogi
 * @param city2 — drugi koniec dodawanej drogi
 * @param year — rok budowy dodawanej drogi
 * @param length — długość dodawanej drogi
 * @return Wartość @p true, jeżeli udało się dodać drogę.
 * Wartość @p false, jeżeli nie udało się zaalokować pamięci
 * lub któryś z parametrów ma nieprawidłową wartość.
 */
bool addNewRoad(Map *map, const char *city1, const char *city2, int year, int length);

/**
 * @brief Sprawdza, czy droga pomiędzy dwoma zadanymi miastami istnieje
 * @param map — mapa w której jest szukane
 * @param city1 — jeden z końców drogi
 * @param city2 — drugi z końców drogi
 * @return Znaleziona droga lub NULL, gdy nie ma drogi lub któryś
 * z argumentów ma niepoprawną wartość
 */
Road *searchRoad(Map *map, City *city1, City *city2);

/**
 * @brief Aktualizuje rok budowy drogi zadanej przez swoje dwa końce.
 * @param map — mapa w której jest droga
 * @param city1 — napis wyznaczający jeden koniec drogi
 * @param city2 — napis wyznaczający drugi koniec drogi
 * @param year — nowy rok który ma być przypisany do drogi
 * @return Wartość @p true, jeżeli udało się zmienić rok.
 * Wartość @p false, jeżeli coś się nie powiodło - któryś
 * z parametrów ma niepoprawną wartość, lub zadana droga nieistnieje.
 */
bool updateYearRoad(Map *map, const char *city1, const char *city2, int year);

/**
 * @brief usuwa zadaną drogę
 * @param road — droga do usunięcia
 */
void deleteRoad(Road *road);

#endif //DROGI_CITIESANDROADS_H
