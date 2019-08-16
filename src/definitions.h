//
// Created by alicja on 04.07.19.
//

#ifndef DROGI_DEFINITIONS_H
#define DROGI_DEFINITIONS_H

#include <stddef.h>
#include "vector.h"

typedef struct City City;
typedef struct Road Road;
typedef struct Route Route;
typedef struct List List;
typedef struct Map Map;
typedef struct Dictionary Dictionary;
typedef struct Set Set;



extern const char *SEMICOLON;
extern const char *MINUS;
extern const char ZERO;

struct Map {
    Route *routes[1000];
    Dictionary *cities;
    Vector *roads;
};

struct City {
    size_t id;
    const char *name;
    Set *roads;
};

struct Road {
    int length;
    int year;
    City *city1, *city2;
};

struct Route {
    List *path;
    City *source, *destination;
    unsigned length;
    int minimalYear;
};

#endif //DROGI_DEFINITIONS_H
