//
// Created by alicja on 04.07.19.
//

#ifndef DROGI_CITIESANDROADS_H
#define DROGI_CITIESANDROADS_H

#include "definitions.h"

bool isCityName(const char *city);

Road *getNewRoad(int year, int length, City *city1, City *city2);

City *getNewCity(const char *name);

bool fixRoad(Map *map, const char *city1, const char *city2, int repairYear);

bool addNewRoad(Map *map, const char *city1, const char *city2, int year, int length);

void zeroVisited(City *city);

City *searchCity(Map *map, const char *city);

Road *searchRoad(Map *map, City *city1, City *city2);

void deleteCity(City *city);

void deleteRoad(Road *road);

#endif //DROGI_CITIESANDROADS_H
