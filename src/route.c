//
// Created by alicja on 19.07.19.
//

#include "route.h"
#include "definitions.h"
#include "dijkstra.h"
#include "citiesAndRoads.h"

#include <stdlib.h>
#include <string.h>

Route *getNewRoute() {
    Route *newRoute = malloc(sizeof(Route));

    return newRoute;
}

bool addNewRoute(Map *map, unsigned routeId, const char *city1, const char *city2) {
    if (map == NULL) {
        return false;
    }

    if (city1 == NULL || city2 == NULL) {
        return false;
    }

    Route *newRoute = dijkstra(searchDictionary(map->cities, city1), searchDictionary(map->cities, city2));

    if (newRoute == NULL) {
        return false;
    }

    map->routes[routeId] = newRoute;

    return true;
}

//extendRoute
//wszystkie miasta, które już są w drodze krajowej, nie mogą być w rozszerzeniu
//puszczam dijkstrę z obu końców i wybieram lepszy, ale nie NULL
bool addToRoute(Map *map, unsigned routeId, const char *city) {
    if (map == NULL) {
        return false;
    }

    if (city == NULL) {
        return false;
    }

    List *restrictedCities = initializeList(compareCities);
}

//Nie usuwa miast!!!
void deleteRoute(Route *route) {
    deleteList(route->path, deleteCity);

    free(route);
}

