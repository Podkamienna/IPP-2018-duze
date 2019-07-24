//
// Created by alicja on 04.07.19.
//

#include "citiesAndRoads.h"
#include "definitions.h"
#include "set.h"

#include <stdlib.h>
#include <string.h>

bool isCityName(const char *city) {
    if (city == NULL) {
        return false;
    }

    size_t len = strlen(city);
    // TODO
}

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

int compareRoads(Road *road1, Road *road2) {
    if (road1->city1 == road2->city1 && road1->city2 == road2->city2) {
        return 0;
    }

    if (road1->city1 == road2->city2 && road1->city2 == road2->city1) {
        return 0;
    }

    return 1;
}

//returns true if the road doesn't exist and false otherwise and if sth goes wrong
//called from addRoad
static bool notExistsRoad(Map *map, City *city1, City *city2) {
    if (city1 == NULL || city2 == NULL) {
        return false;
    }
//TODO Czy da się bez tego alokowania? W sensie można np porownywac tylko z 2. miastem
    Road *tmpRoad = getNewRoad(0, 0, city1, city2);

    if (tmpRoad == NULL) {
        return false;
    }

    return !searchSet(city1->roads, tmpRoad);
}

City *getNewCity(const char *name) {
    City *newCity = malloc(sizeof(City));

    if (newCity == NULL) {
        return NULL;
    }

    newCity->roads = initializeSet(compareRoads);

    if (newCity->roads == NULL) {
        free(newCity);
        return NULL;
    }

    newCity->name = name;
    newCity->visited = 0;

    return newCity;
}

static bool addCity(Map *map, City *city) {
    if (city == NULL || map == NULL) {
        return false;
    }

    return insertDictionary(map->cities, city, tmpCity);
}

bool addNewRoad(Map *map, const char *city1, const char *city2, int year, int length) {
    if (map == NULL) {
        return false;
    }

    if (city1 == NULL || city2 == NULL) {
        return false;
    }

    City *newCity1 = searchCity(map, city1);
    City *newCity2 = searchCity(map, city2);

    if (newCity1 == NULL) {
        newCity1 = getNewCity(city1);

        if (newCity1 == NULL || !addCity(map, newCity1)) {
            return false;
        }
    }

    if (newCity2 == NULL) {
        newCity2 = getNewCity(city2);

        if (newCity2 == NULL || !addCity(map, newCity2)) {
            return false;
        }
    }

    //jeżeli tak jest, to miasta istniały wcześniej, więc nie trzeba nic usuwać
    if (!notExistsRoad(map, newCity1, newCity2)) { // TODO wymyślić lepszą nazwę
        return false;
    }

    Road *newRoad = getNewRoad(year, length, newCity1, newCity2);

    if (newRoad == NULL) {
        return false;
    }

    if (!insertSet(newCity2->roads, newRoad)) {
        deleteRoad(newRoad);

        return false;
    }

    if (!insertSet(newCity1->roads, newRoad)) {
        deleteLastAdded(newCity2->roads, deleteRoad);
        deleteRoad(newRoad);

        return false;
    }

    return true;
}

void zeroVisited(City *city) {
    if (city == NULL) {
        return;
    }

    city->visited = 0;
}


City *searchCity(Map *map, const char *city) {
    if (map == NULL || city == NULL) {
        return NULL;
    }

    return searchDictionary(map->cities, city);
}

//called from repairRoad
Road *searchRoad(Map *map, City *city1, City *city2) {
    if (city1 == NULL || city2 == NULL) {
        return NULL;
    }

    Road *tmpRoad = getNewRoad(0, 0, city1, city2);

    if (tmpRoad == NULL) {
        return NULL;
    }

    return searchSet(city1->roads, tmpRoad);
}

void deleteRoad(Road *road) {
    free(road);
}

bool fixRoad(Map *map, const char *city1, const char *city2, int repairYear) {
    if (city1 == NULL || city2 == NULL) {
        return false;
    }

    City *tmpCity1 = searchCity(map, city1), *tmpCity2 = searchCity(map, city2);

    if (tmpCity1 == NULL || tmpCity2 == NULL) {
        return false;
    }

    // TODO
}

void deleteCity(City *city) {
    deleteSet(city->roads, deleteRoad);

    free(city->name);
    free(city);
    // TODO jak są usuwane odcinki drogowe? Hmm?
}