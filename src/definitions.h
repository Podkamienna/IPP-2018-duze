//
// Created by alicja on 04.07.19.
//

#ifndef DROGI_DEFINITIONS_H
#define DROGI_DEFINITIONS_H

#include "dictionary.h"
#include "set.h"

typedef struct City City;
typedef struct Road Road;
typedef struct Route Route;
typedef struct Map Map;

struct Map {
    Route *routes[1000];
    Dictionary *cities;
};

struct City {
    int id;
    char *name;
    Set *roads;
    int visited; //TODO funkcja do czyszczenia tego w citiesAndRoads
};

struct Road {
    int length;
    int year;
    City *city1, *city2;
};

struct Route {

};

#endif //DROGI_DEFINITIONS_H
