/** @file
 * Moduł zawierający definicje używane w projekcie.
 *
 * @author Alicja Ziarko <ala.ziarko@gmail.com>
 * @date 05.07.2019
 */

#ifndef DROGI_DEFINITIONS_H
#define DROGI_DEFINITIONS_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * Struktura przechowująca miasto.
 */
typedef struct City City;

/**
 * Struktura przechowująca drogę.
 */
typedef struct Road Road;

/**
 * Struktura przechowując drogę krajową.
 */
typedef struct Route Route;

/// @cond ALREADY_DOCUMENTED
typedef struct List List;
typedef struct Map Map;
typedef struct Dictionary Dictionary;
typedef struct Set Set;
typedef struct Vector Vector;
/// @endcond

/**
 * Kończy wykonywanie funkcji, wykonując wcześniej polecenia pod etykietą failure.
 */
#define FAIL           \
    do {               \
        goto failure;  \
    } while(0)

/**
 * Jeżeli warunek jest spełniony, kończy wykonywanie funkcji,
 * wykonując wcześniej polecenia pod etykietą failure.
 */
#define FAIL_IF(condition)              \
    do {                                \
        if (condition) goto failure;    \
    } while(0)                          \

/**
* Jeżeli warunek jest spełniony, kończy wykonywanie funkcji,
* wykonując wcześniej polecenia pod etykietą failure##label.
*/
#define FAIL_IF_LABELED(condition, label)    \
    do {                                     \
        if (condition) goto failure##label;  \
    } while(0)                               \

/**
 * Liczba dróg krajowych
 */
#define ROUTE_COUNT 1000

/**
 * Znak oddzielający kolejne wyrazy w wejściu
 */
extern const char *DELIMITER;

struct Map {
    Route *routes[ROUTE_COUNT]; ///< tablica dróg krajowych
    Dictionary *cities; ///< słownik zawierający miasta dodane do mapy
    Vector *roads; ///< vector dróg dodanych do mapy
};

struct City {
    size_t id; ///< id miasta
    char *name; ///< nazwa miasta
    Set *roads; ///< zbiór dróg wychodzących z miasta
};

struct Road {
    bool isBlocked; ///< Parametr pozwalający stwierdzić, czy wolno przejechać daną drogą.
    unsigned length; ///< długość drogi
    int year; ///< Rok budowy/ostatniego remontu drogi.
    City *city1, *city2; ///< końce drogi
};

struct Route {
    List *path; ///< najkrótsza ścieżka między source a destination
    City *source, *destination; ///< miasto początkowe i końcowe ścieżki
};

struct PathNode {
    City *city; ///< miasto
    Road *road; ///< droga wychodząca z city
};

#endif //DROGI_DEFINITIONS_H
