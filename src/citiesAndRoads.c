//
// Created by alicja on 04.07.19.
//

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

static Road getRoad(int year, int length, City *city1, City *city2) {
    Road newRoad;

    newRoad.year = year;
    newRoad.length = length;
    newRoad.city1 = city1;
    newRoad.city2 = city2;

    return newRoad;
}

//zakladam, ze nie sa NULLami
static int compareRoads(Road *road1, Road *road2) {//TODO static????
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

    for (size_t i = 0; city[i] != '\0'; i++) {
        if (city[i] <= 31 || city[i] == 59) {
            return false;
        }
    }
    return true;
}

City *getNewCity(const char *name) {
    if (name == NULL) {
        return NULL;
    }

    if (!isCityName(name)) {
        return NULL;
    }

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

//zakladam, ze nie sa NULLami
int compareCities(City *city1, City *city2) {
    if (strcmp(city1->name, city2->name)) {
        return 0;
    }

    return 1;
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

void visit(City *city) {
    if (city == NULL) {
        return;
    }

    city->visited = 1;
}

void unvisit(City *city) {
    if (city == NULL) {
        return;
    }

    city->visited = 0;
}

bool isVisited(City *city) {
    if (city == NULL) {
        return false;
    }

    return city->visited != 0;
}

City *getNeighbour(Road *road, City *city) {
    if (road == NULL || city == NULL) {
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

    deleteSet(city->roads, deleteRoad);
    free(city->name);
    free(city);
    // TODO jak są usuwane odcinki drogowe? Hmm?
}

Road *getNewRoadPointer(int year, int length, City *city1, City *city2) {
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

    if (searchRoad(map, newCity1, newCity2)) {
        return false;
    }

    Road *newRoad = getNewRoadPointer(year, length, newCity1, newCity2);

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

Road *searchRoad(Map *map, City *city1, City *city2) {
    if (map == NULL) {
        return NULL;
    }

    if (city1 == NULL || city2 == NULL) {
        return NULL;
    }

    Road tmpRoad1, tmpRoad2;

    tmpRoad1 = getRoad(0, 0, city1, city2);
    tmpRoad2 = getRoad(0, 0, city2, city1);

    Road *searchResult1, *searchResult2;

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
//TODO
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
    free(road);
}