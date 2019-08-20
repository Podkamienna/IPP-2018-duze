//
// Created by alicja on 04.07.19.
//

#define _GNU_SOURCE

#include "citiesAndRoads.h"
#include "definitions.h"
#include "dictionary.h"
#include "set.h"

#include <stdlib.h>
#include <string.h>

static bool addCity(Map *map, City *city) {
    if (city == NULL || map == NULL) {
        return false;
    }

    return insertDictionary(map->cities, city->name, city);
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

City *getNewCity(Map *map, const char *name) {
    City *newCity = NULL;

    FAIL_IF(name == NULL);
    FAIL_IF(!isCityName(name));

    newCity = malloc(sizeof(City));
    FAIL_IF(newCity == NULL);

    newCity->roads = initializeSet((int(*)(void *, void *))compareRoads);
    FAIL_IF(newCity->roads == NULL);

    newCity->name = strdup(name);
    FAIL_IF(newCity->name == NULL);

    newCity->id = getId(map->cities);

    FAIL_IF(!addCity(map, newCity));

    return newCity;

    failure:;
    deleteCity(newCity);

    return NULL;
}

int compareCities(City *city1, City *city2) {
    if (city1 == NULL && city2 == NULL) {
        return 0;
    }

    if (city1 == NULL) {
        return 1;
    }

    if (city2 == NULL) {
        return -1;
    }

    if (city1->id > city2->id) {
        return 1;
    }

    if (city1->id < city2->id) {
        return -1;
    }

    return 0;
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
    if (road == NULL || city == NULL) {
        return NULL;
    }

    if (road->isBlocked) {
        return NULL;
    }

    if (compareCities(road->city1, city) != 0  && compareCities(road->city2, city) != 0) {
        return NULL;
    }

    if (compareCities(road->city1, city) != 0) {
        return road->city1;
    }

    if (compareCities(road->city2, city) != 0) {
        return road->city2;
    }

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

Road *getNewRoad(int year, int length, City *city1, City *city2) {
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

bool addNewRoad(Map *map, const char *city1, const char *city2, int year, int length) {
    FAIL_IF_NAMED(map == NULL, 4);
    FAIL_IF_NAMED(city1 == NULL || city2 == NULL, 4);

    City *newCity1 = searchCity(map, city1);
    City *newCity2 = searchCity(map, city2);

    if (newCity1 == NULL) {
        newCity1 = getNewCity(map, city1);
        FAIL_IF_NAMED(newCity1 == NULL, 4);
    }

    if (newCity2 == NULL) {
        newCity2 = getNewCity(map, city2);
        FAIL_IF_NAMED(newCity2 == NULL, 4);
    }

    FAIL_IF_NAMED(searchRoad(map, newCity1, newCity2), 4);

    Road *newRoad = getNewRoad(year, length, newCity1, newCity2);
    FAIL_IF_NAMED(newRoad == NULL, 4);

    FAIL_IF_NAMED(!pushToVector(map->roads, newRoad), 3);
    FAIL_IF_NAMED(!insertSet(newCity2->roads, newRoad), 2);
    FAIL_IF_NAMED(!insertSet(newCity1->roads, newRoad), 1);

    return true;

    failure1:;
    deleteFromSet(newCity2->roads, NULL, newRoad);

    failure2:;
    popFromVector(map->roads, NULL);

    failure3:;
    deleteRoad(newRoad);

    failure4:;
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

bool removeSomeRoad(Map *map, City *city1, City *city2) {
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

Road *searchRoad(Map *map, City *city1, City *city2) {
    if (map == NULL) {
        return NULL;
    }

    if (city1 == NULL || city2 == NULL) {
        return NULL;
    }

    Road tmpRoad1, tmpRoad2;

    tmpRoad1.length = 0;
    tmpRoad1.year = 0;
    tmpRoad1.city1 = city1;
    tmpRoad1.city2 = city2;

    tmpRoad2.length = 0;
    tmpRoad2.year = 0;
    tmpRoad2.city1 = city1;
    tmpRoad2.city2 = city2;
   // setRoad(tmpRoad1, 0, 0, city1, city2); TODO czemu nie działa? :'(
   // setRoad(tmpRoad2, 0, 0, city2, city1);

    Road *searchResult1, *searchResult2; //TODO wystarczy jednego z tych wyszukać, bo komparator jest miły

    searchResult1 = searchSet(city1->roads, &tmpRoad1);
    searchResult2 = searchSet(city1->roads, &tmpRoad2);

    if (searchResult1 == NULL) {
        return searchResult2;
    }

    return searchResult1;
}

//Najpierw trzeba zmienic wyszukiwanie dróg. Potem trzeba ogarnąć, czy dodawać drogi w obie strony
//(chyba lepiej nie) ale to w ogóle jest jakieś brzydkie strasznie i trzeba w każdej funkcji tutaj
//zobaczyć, czy to jest ok. Tylko tutaj raczej, więc nie aż tak tragicznie. Też trochę przypał z tym zerowaniem
//odwiedzonych - lepiej by było robić to w Dijkstrze - cykliczne odwołania? Nie brzmi dobrze. Przecież nie bardzo
//potem dodać repair road.
//TODO do map.c
bool updateYearRoad(Map *map, const char *city1, const char *city2, int year) {
    if (map == NULL) {
        return false;
    }

    if (city1 == NULL || city2 == NULL) {
        return false;
    }

    City *tmpCity1 = searchCity(map, city1), *tmpCity2 = searchCity(map, city2);

    //jeżeli nie ma któregoś z miast, to drogi też nie ma
    if (tmpCity1 == NULL || tmpCity2 == NULL) {
        return false;
    }

    Road *tmpRoad = searchRoad(map, tmpCity1, tmpCity2);

    if (tmpRoad == NULL) {
        return false;
    }

    if (tmpRoad->year > year) {
        return false;
    }

    tmpRoad->year = year;

    return true;
}

void deleteRoad(Road *road) {
    if (road == NULL) {
        return;
    }

    free(road);
}