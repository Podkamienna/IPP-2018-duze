//
// Created by alicja on 19.07.19.
//

#include "route.h"
#include "definitions.h"
#include "dijkstra.h"

#include <stdlib.h>

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

bool insertToRoute(Map *map, unsigned routeId, const char *city1, const char *city2) {
    
}

bool addToRoute(Map *map, unsigned routeId, const char *city);

