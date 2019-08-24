/**
 * @file Interfejs klasy przechowującej miasta i drogi.
 */

#ifndef DROGI_CITIES_AND_ROADS_H
#define DROGI_CITIES_AND_ROADS_H

#include "definitions.h"

#include <stdbool.h>

/**
 * @brief Alokuje pamięc pod i zwraca wskaźnik na
 * nowe miasto.
 * @param name — nazwa nowego miasta
 * @return Wartość @p true jeżeli udało się dodać miasto.
 * Wartość @p false jeżeli nie udało się zaalokować pamięci,
 * lub jeżeli zadany napis nie jest poprawną nazwą miasta.
 */
City *getNewCity(Map *map, const char *name);

/**
 * @brief Usuwa zadane miasto.
 * @param city — miasto do usunięcia
 */
void deleteCity(City *city);

/**
 * @brief Sprawdza, czy zadany napis jest poprawną nazwą miasta.
 * @param city — wskaźnik na napis do sprawdzenia
 * @return Wartość @p true jeżeli napis jest poprawną nazwą miasta.
 * Wartość @p false jeżeli city ma wartość NULL lub nie jest
 * poprawną nazwą miastą.
 */
bool isCityName(const char *city);

/**
 * @brief Porównuje miasta, sortując je po momencie
 * dodania ich do mapy.
 * @param city1 — pierwsze z miast do porównania
 * @param city2 — drugie z miast do porówania
 * @return Wartość @p 1 jeżeli pierwsze miasto jest
 * większe, wartość @p 0 jeżeli miasta są równe,
 * wartość @p -1 jeżeli drugie miasto jest większe.
 */
bool areEqualCities(City *city1, City *city2);

/**
 * @brief Wyszukuje w zadanej mapie miasta powiązanego z
 * zadaną nazwą.
 * @param map — mapa w której ma być wyszukiwane
 * @param city — nazwa miasta do odnalezienia
 * @return Znalezione miasto lub wartość @p NULL, jeżeli
 * nie udało się go odnaleźć lub któryś z parametrów miał
 * wartość NULL.
 */
City *searchCity(Map *map, const char *city);

/**
 * @brief Mając drogę z zadanego miasta do innego,
 * zwraca ten jej koniec, który nie jest zadanym miastem.
 * @param road — droga z zadanego miasta
 * @param city — miasto, którego sąsiada szukamy
 * @return Wartość @p NULL jeżeli któryś z parametrów jest
 * NULLem, zadana droga jest zablokowana, zadane miasto nie
 * jest równe żadnemu z końców zadanej drogi.
 */
City *getNeighbour(Road *road, City *city);

/**
 * @brief Alokuje pamieć pod i zwraca wskaźnik na nową drogę.
 * @param year — rok budowy nowej drogi
 * @param length — długość nowej drogi
 * @param city1 — jeden koniec nowej drogi
 * @param city2 — drugi koniec nowej drogi
 * @return Wskaźnik na nową drogę, jeżeli wszystko się uda.
 * Null jeżeli nie udało się zaalokować pamięci.
 */
Road *getNewRoad(int year, int length, City *city1, City *city2);

/**
 * @brief Usuwa zadaną drogę
 * @param road — droga do usunięcia
 */
void deleteRoad(Road *road);

/**
 * @brief Porównuje 2 drogi. 2 drogi są równe, jeżeli
 * obie mają wartość NULL lub łączą te same miasta.
 * W innych wypadkach pierwsza droga jest większa
 * od drugiej.
 * @param road1 — pierwsza z dróg do porównanie
 * @param road2 — druga z dróg do porównania
 * @return Wartość @p 0 jeżeli drogi są równe,
 * wartość @p 1, jeżeli nie są równe.
 */
bool areEqualRoads(Road *road1, Road *road2);

/**
 * @brief Ustawia drogę na niemożliwą do przejścia
 * @param road — droga do ustawienia na niemożliwą do przejścia
 */
void blockRoad(Road *road);

/**
 * @brief Ustawia drogę na możliwą do przejścia
 * @param road — droga do ustawienia na możliwą do przejścia
 */
void unblockRoad(Road *road);

/**
 * @brief Wyszukuje drogi między dwoma zadanymi miastami w zadanej mapie.
 * @param map — mapa w której jest szukane
 * @param city1 — jeden z końców drogi
 * @param city2 — drugi z końców drogi
 * @return Znaleziona droga lub NULL, gdy nie ma drogi lub któryś
 * z argumentów ma niepoprawną wartość
 */
Road *searchRoad(Map *map, City *city1, City *city2);

/**
 * @brief Wyszukuje miasta zadanego danym napisem w danej mapie.
 * @param map — mapa w której ma być wyszukiwane
 * @param city — napis zadający miasto, które ma być wyszukane
 * @return Wyszukiwane miasto jeżeli wszystko się powiodło,
 * NULL, jeżeli coś się nie powiodło - miasto nie istnieje
 * w mapie lub parametry wejściowe są nieprawidłowe.
 */

bool deleteRoadFromMap(Map *map, City *city1, City *city2);

#endif //DROGI_CITIES_AND_ROADS_H
