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
    int visited;
    // TODO to chyba nie pasuje (visited).
};

struct Road {
    int length;
    int year;
    City *city1, *city2;
};

struct Route {
    City *city;
    Road *edge; //droga wchodzaca do city
    Route *prev; //do odzyskiwania najkrotszej sciezki
    int priority; //priority = najmn ob znana odl wierzch city od wierzcholka startowego
    int minYear; //minimalny rok ostatniego remontu na najkrótszej sciezce od wierzch start do obecnego
};


#endif //DROGI_DEFINITIONS_H
