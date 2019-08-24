//
// Created by alicja on 04.07.19.
//

#ifndef DROGI_DEFINITIONS_H
#define DROGI_DEFINITIONS_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct City City;
typedef struct Road Road;
typedef struct Route Route;
typedef struct List List;
typedef struct Map Map;
typedef struct Dictionary Dictionary;
typedef struct Set Set;
typedef struct Vector Vector;

/**
 * Kończy wykonywanie funkcji, wykonując wcześniej polecenia pod etykietą failure.
 */
#define FAIL           \
    do {               \
        goto failure;  \
    } while(0)

/**
 * Jeżeli warunek jest spełniony, kończy wykonywanie funkcji,
 * wykonyjąc wcześniej polecenia pod etykietą failure.
 */
#define FAIL_IF(condition)              \
    do {                                \
        if (condition) goto failure;    \
    } while(0)                          \

/**
* Jeżeli warunek jest spełniony, kończy wykonywanie funkcji,
* wykonyjąc wcześniej polecenia pod etykietą failure##label.
*/
#define FAIL_IF_LABELED(condition, label)    \
    do {                                     \
        if (condition) goto failure##label;  \
    } while(0)                               \

/** Liczba dróg krajowych */
#define ROUTE_COUNT 1000

extern const char *DELIMITER;

struct Map {
    Route *routes[ROUTE_COUNT];
    Dictionary *cities;
    Vector *roads;
};

struct City {
    size_t id;
    char *name;
    Set *roads;
};

struct Road {
    bool isBlocked;
    unsigned length;
    int year;
    City *city1, *city2;
};

struct Route {
    List *path;
    City *source, *destination;
};

struct PathNode {
    City *city;
    Road *road;
};

#endif //DROGI_DEFINITIONS_H
