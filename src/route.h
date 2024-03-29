/** @file
 * Interfejs modułu zawierającego operacje na drogach krajowych
 * i pojedyńczych elementach tworzących listy będące ścieżkami w tych
 * drogach krajowych.
 *
 * @author Alicja Ziarko <ala.ziarko@gmail.com>
 * @date 20.07.2019
 */

#ifndef DROGI_ROUTE_H
#define DROGI_ROUTE_H

#include "definitions.h"

#include <stdbool.h>

/** Stałe z minimalnym i maksymalnym numerem drogi krajowej */
extern const unsigned MINIMAL_ROUTE_ID;
extern const unsigned MAXIMAL_ROUTE_ID;

/** Struktura przechowująca jedno pole z listy będącej ścieżką drogi krajowej*/
typedef struct PathNode PathNode;

/**
 * @brief Alokuje pamięć pod i zwraca nowe pole ścieżki
 * drogi krajowej
 * @param city — miasto, informacja o którym ma być zawarta w nowej strukturze
 * @param road — miasto, informacja o której ma być zawarta w nowej strukturze
 * @return Wskaźnik na nową strukturę lub wartość @p NULL jeżeli nie udało się
 * zaalokować na nią pamięci
 */
PathNode *getNewPathNode(City *city, Road *road);

/**
 * @brief Usuwa pole ścieżki drogi krajowej.
 * @param pathNode — pole ścieżki drogi krajowej do usunięcia
 */
void deletePathNode(PathNode *pathNode);

/**
 * @brief Pierwsza z funkcji do porównywania elementów typu
 * PathNode.
 * @param a — pierwszy z argumentów do porównania
 * @param b — drugi z argumentów do porównania
 * @return Wartość @p 1 jeżeli pierwszy z argumentów jest większy,
 * wartość @p 0, gdy argumenty są równe,
 * wartość @p -1 gdy drugi argument jest większy
 */
bool areEqualPathNodes(PathNode *a, PathNode *b);

/**
 * @brief Druga z funkcji do porównywania elementów typu
 * PathNode.
 * @param a — pierwszy z argumentów do porównania
 * @param b — drugi z argumentów do porównania
 * @return Wartość @p 1 jeżeli pierwszy z argumentów jest większy,
 * wartość @p 0, gdy argumenty są równe,
 * wartość @p -1 gdy drugi argument jest większy
 */
bool areEqualPathNodesByCities(PathNode *a, PathNode *b);

/**
 * @brief Alokuje pamięć pod i zwraca nową drogę
 * krajową
 * @return Wskaźnik na nową strukturę lub wartość @p NULL,
 * jeżeli nie udało się zaalokować pamięci
 */
Route *getNewRoute(List *path, City *source, City *destination);

/**
 * @brief Usuwa drogę krajową.
 * @param route — droga krajowa do usunięcia
 * ścieżkę z drogi krajowej.
 */
void deleteRoute(Route *route);


#endif //DROGI_ROUTE_H
