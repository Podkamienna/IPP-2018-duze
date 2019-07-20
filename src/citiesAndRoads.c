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

static bool addCity(Map *map, City *city) {
    if (tmpCity == NULL) {
        return false;
    }

    return insertDictionary(map->cities, city, tmpCity);
}

bool addNewRoad(Map *map, const char *city1, const char *city2, int year, int length) {
    City *tmpCity1, *tmpCity2;
    tmpCity1 = searchCity(map, city1);
    tmpCity2 = searchCity(map, city2);

    City *newCity1, *newCity2;

    if (tmpCity1 == NULL) {
        newCity1 = getNewCity(city1);

        if (newCity1 == NULL || !addCity(map, newCity1)) {
            return false;
        }
    }

    else {
        newCity1 = tmpCity1;
    }

    if (tmpCity2 == NULL) {
        newCity2 = getNewCity(city2);
        if (newCity2 == NULL || !addCity(map, newCity2)) {
            if (tmpCity1 == NULL) {
                deleteCity(map, newCity1);
            }
            return false;
        }
    }

    else {
        newCity2 = tmpCity2;
    }

    if (!notExistsRoad(map, newCity1, newCity2)) {
        deleteCity(newCity1);
        deleteCity(newCity2);
        //to jest zle...
    }

}

void zeroVisited(City *city) {
    if (city == NULL) {
        return;
    }

    city->visited = 0;
}

City *searchCity(Map *map, const char *city) {
    return searchDictionary(map->cities, city);
}

bool predicate(Road *road1, Road *road2) {
    if(road1->city1 == road2->city1 && road1->city2 == road2->city2) {
        return true;
    }

    if(road1->city1 == road2->city2 && road1->city2 == road2->city1) {
        return true;
    }

    return false;
}

//returns true if the road doesn't exist and false otherwise and if sth goes wrong
//called from addRoad
bool notExistsRoad(Map *map, City *city1, City *city2) {
    if (city1 == NULL || city2 == NULL) {
        return false;
    }
//TODO Czy da się bez tego alokowania? W sensie można np porownywac tylko z 2. miastem
    Road *tmpRoad = getNewRoad(0, 0, city1, city2);

    if (tmpRoad == NULL) {
        return false;
    }

    return !searchSet(city1->roads, predicate, tmpRoad);
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
    //TODO czy rozważać sytuację,gdy jest droga w jednym mieście, a w drugim nie?
    return searchSet(city1->roads, predicate, tmpRoad);
}

void deleteRoad(Road *road) {
    free(road);
}

void deleteCity(City *city) {
    deleteSet(city->roads, deleteRoad);

    free(city->name);
    free(city);
    city = NULL; // czy to wystarczy, żeby było ok przy usuwaniu jezeli cos sie zepsuje przy usuwaniu drogi?
}