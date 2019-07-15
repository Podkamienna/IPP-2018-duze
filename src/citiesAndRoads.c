//
// Created by alicja on 04.07.19.
//

#include "citiesAndRoads.h"
#include "definitions.h"
#include "set.h"

#include <stdlib.h>

Road *getNewRoad(int year, int length, City *city1, City *city2) {
    Road *newRoad = malloc(sizeof(Road));

    if (newRoad == NULL) {
        return NULL;
    }

    newRoad->year = year;
    newRoad->length = length;
    newRoad->city1 = city1;
    newRoad->city2 = city2;

    return newRoad;
}

City *getNewCity(const char *name) {
    City *newCity = malloc(sizeof(City));

    if(newCity == NULL) {
        return NULL;
    }

    newCity->roads = initializeSet();

    if (newCity->roads == NULL) {
        free(newCity);
        return NULL;
    }

    newCity->name = name;
    newCity->visited = 0;

    return newCity;
}

City *searchCity(Map *map, const char *city) {
    return searchDictionary(map->cities, city);
}
/*
 * No tu jest taki problem, ze jak cos sie psuje, to chce zwracac NULLa... Ale tez jak nie znajde... :(
 */

Road *searchRoad(Map *map, const char *city1, const char *city2) {
    if (city1 == NULL || city2 == NULL) {
        return NULL;
    }
    City *tmpCity = searchCity(map, city1);

    if (tmpCity == NULL) {
        return NULL;
    }

    City *src = getNewCity(city1), *dest = getNewCity(city2);

    if (src == NULL || dest == NULL) {
        deleteCity(src);
        deleteCity(dest);
        return NULL;

    }
    Road *tmpRoad = getNewRoad(0, 0, getNewCity(city1), getNewCity(city2));


}

void deleteCity(City *city);

void deleteRoad(Road *road);