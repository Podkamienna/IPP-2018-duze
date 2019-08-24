/**
 * @file Implementacja klasy przechowującej miasta i drogi.
 */

#define _GNU_SOURCE

#include "citiesAndRoads.h"
#include "definitions.h"
#include "dictionary.h"
#include "set.h"

#include <stdlib.h>
#include <string.h>

/**
 * @brief Dodaje zadane miasto do zadanej mapy.
 * @param map — mapa do której miasto ma być dodane
 * @param city — miasto do dodania
 * @return Wartość @p true, jeżeli wszystko się udało.
 * Wartość @p false, jeżeli miasto już jest w mapie,
 * któryś z parametrów ma niepoprawną wartość lub nie udało
 * się zaalokować pamięci.
 */
static bool addCityToMap(Map *map, City *city);

static bool addCityToMap(Map *map, City *city) {
    if (city == NULL || map == NULL) {
        return false;
    }

    return insertDictionary(map->cities, city->name, city);
}

City *getNewCity(Map *map, const char *name) {
    City *newCity = NULL;

    FAIL_IF(name == NULL);
    FAIL_IF(!isCityName(name));

    newCity = malloc(sizeof(City));
    FAIL_IF(newCity == NULL);

    newCity->roads = initializeSet((bool (*)(void *, void *)) areEqualRoads);
    FAIL_IF(newCity->roads == NULL);

    newCity->name = strdup(name);
    FAIL_IF(newCity->name == NULL);

    newCity->id = getId(map->cities);

    FAIL_IF(!addCityToMap(map, newCity));

    return newCity;

    failure:;
    deleteCity(newCity);

    return NULL;
}

void deleteCity(City *city) {
    if (city == NULL) {
        return;
    }

    deleteSet(city->roads, NULL);

    free(city->name);
    free(city);
}

bool isCityName(const char *city) {
    if (city == NULL) {
        return false;
    }

    if (strlen(city) == 0) {
        return false;
    }

    for (size_t i = 0; city[i] != '\0'; i++) {
        if ((city[i] >= 0 && city[i] <= 31) || city[i] == ';') {
            return false;
        }
    }
    return true;
}

bool areEqualCities(City *city1, City *city2) {
    if (city1 == NULL && city2 == NULL) {
        return true;
    }

    if (city1 == NULL || city2 == NULL) {
        return false;
    }

    return city1->id == city2->id;
}

City *searchCity(Map *map, const char *city) {
    if (map == NULL) {
        return NULL;
    }

    if (city == NULL) {
        return NULL;
    }

    return searchDictionary(map->cities, city);
}

City *getNeighbour(Road *road, City *city) {
    FAIL_IF(road == NULL || city == NULL);
    FAIL_IF(road->isBlocked);

    if (areEqualCities(road->city1, city)) {
        return road->city2;
    }

    if (areEqualCities(road->city2, city)) {
        return road->city1;
    }

    failure:;
    return NULL;
}

Road *getNewRoad(int year, unsigned length, City *city1, City *city2) {
    Road *newRoad = malloc(sizeof(Road));

    if (newRoad == NULL) {
        return NULL;
    }

    newRoad->isBlocked = false;
    newRoad->length = length;
    newRoad->year = year;
    newRoad->city1 = city1;
    newRoad->city2 = city2;

    return newRoad;
}

void deleteRoad(Road *road) {
    free(road);
}

bool areEqualRoads(Road *road1, Road *road2) {
    if (road1 == NULL && road2 == NULL) {
        return true;
    }

    if (road1 == NULL || road2 == NULL) {
        return false;
    }

    if (areEqualCities(road1->city1, road2->city1) && areEqualCities(road1->city2, road2->city2)) {
        return true;
    }

    if (areEqualCities(road1->city1, road2->city2) && areEqualCities(road1->city2, road2->city1)) {
        return true;
    }

    return false;
}

void blockRoad(Road *road) {
    if (road == NULL) {
        return;
    }

    road->isBlocked = true;
}

void unblockRoad(Road *road) {
    if (road == NULL) {
        return;
    }

    road->isBlocked = false;
}

Road *searchRoad(Map *map, City *city1, City *city2) {
    if (map == NULL) {
        return NULL;
    }

    if (city1 == NULL || city2 == NULL) {
        return NULL;
    }

    Road tmpRoad;

    tmpRoad.length = 0;
    tmpRoad.year = 0;
    tmpRoad.city1 = city1;
    tmpRoad.city2 = city2;

    return searchSet(city1->roads, &tmpRoad);
}

bool deleteRoadFromMap(Map *map, City *city1, City *city2) {
    if (city1 == NULL || city2 == NULL) {
        return false;
    }

    Road *tmpRoad = searchRoad(map, city1, city2);

    if (tmpRoad == NULL) {
        return false;
    }

    deleteFromSet(city1->roads, NULL, tmpRoad);
    deleteFromSet(city2->roads, NULL, tmpRoad);

    return true;
}