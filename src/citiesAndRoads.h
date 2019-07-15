//
// Created by alicja on 04.07.19.
//

#ifndef DROGI_CITIESANDROADS_H
#define DROGI_CITIESANDROADS_H

#include "definitions.h"

Road *getNewRoad(int year, int length, City *city1, City *city2);

City *getNewCity(const char *name);

bool addToMap(const char *city1, const char *city2);

City *searchCity(Map *map, const char *city);

Road *searchRoad(Map *map, const char *city1, const char *city2);

void deleteCity(City *city);

void deleteRoad(Road *road);

#endif //DROGI_CITIESANDROADS_H
