//
// Created by alicja on 19.07.19.
//

#include "route.h"
#include "definitions.h"
#include "dijkstra.h"
#include "citiesAndRoads.h"

#include <stdlib.h>
#include <string.h>

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

//route  - wskaźnik na początek drogi krajowej
//city - miasto zawierający które NODE chcemy
Route *findFirstCity(Route *route, const char *city1, const char *city2) { //TODO static
    Route *position = route;
    while (position != NULL) {
        if (strcmp(city1, position->city->name) == 0) { //czy tak jest ok?
            return position;
        }

        if (strcmp(city2, position->city->name) == 0) {
            return position;
        }
    }

    return NULL;
}

bool overwriteRoute(Map *map, unsigned routeId, const char *city1, const char *city2, Route *route) {
    if (map == NULL) {
        return false;
    }

    if (city1 == NULL || city2 == NULL) {
        return false;
    }

    Route *firstCity = findFirstCity(map->routes[routeId], city1, city2), *secondCity;
    Route *position = firstCity;

    //sprawdzenie, czy to 2. miasto wgl jest w drodze krajowej
    while (position->prev != NULL) {
        position = position->prev;

        if (strcmp(position->city->name, city1) == 0) {
            break;
        }

        if (strcmp(position->city->name, city2) == 0) {
            break;
        }
    }

    if (position->prev == NULL) {
        return false;
    }

    //usunięcie rzeczy
    position = firstCity;

    while (true) {
        position = position->prev;

        if (strcmp(position->city->name, city1) == 0) {
            break;
        }

        if (strcmp(position->city->name, city2) == 0) {
            break;
        }

        deleteRoute(position);
    }

    secondCity = position;

    //włożenie nowych rzeczy
    position = route;

    firstCity->prev = route;

    while (position->prev != NULL) {
        position = position->prev;
    }

    position->prev = secondCity;

    return true;
}

bool insertToRoute(Map *map, unsigned routeId, const char *city1, const char *city2){

}

bool addToRoute(Map *map, unsigned routeId, const char *city);

