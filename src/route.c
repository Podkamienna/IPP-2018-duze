//
// Created by alicja on 19.07.19.
//

#include "route.h"
#include "definitions.h"
#include "dijkstra.h"
#include "citiesAndRoads.h"

#include <stdlib.h>
#include <string.h>

typedef struct PathNode PathNode;

struct PathNode {
    City *city;
    Road *road;
};

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

    Route *newRoute = dijkstra(searchDictionary(map->cities, city1), searchDictionary(map->cities, city2), NULL);

    if (newRoute == NULL) {
        return false;
    }

    map->routes[routeId] = newRoute;

    return true;
}

bool isCorrectRoute(Route *route) {
    if (route == NULL) {
        return false;
    }

    if (route->minimalYear == 0) {
        return false;
    }

    return true;
}

int min(int a, int b) {
    if (a > b) {
        return b;
    }

    return a;
}

//jezeli pierwsze wieksze to 1
int *compareRoute(Route *route1, Route *route2) {
    if (!isCorrectRoute(route1) && !isCorrectRoute(route2)) {
        return 0;
    }

    if (!isCorrectRoute(route1)) {
        return -1;
    }

    if (!isCorrectRoute(route2)) {
        return 1;
    }

    if (route1->length > route2->length) {
        return -1;
    }

    if (route2->length > route1->length) {
        return 1;
    }

    if (route1->minimalYear > route2->minimalYear) {
        return 1;
    }

    if (route2->minimalYear > route1->minimalYear) {
        return -1;
    }

    return 0;
}

PathNode *getNewPathNode(City *city, Road *road) {
    PathNode *pathNode = malloc(sizeof(PathNode));

    if (pathNode == NULL) {
        return NULL;
    }

    pathNode->city = city;
    pathNode->road = road;

    return pathNode;
}

//extendRoute
//wszystkie miasta, które już są w drodze krajowej, nie mogą być w rozszerzeniu
//puszczam dijkstrę z obu końców i wybieram lepszy, ale nie NULL
//jest problem, jeżeli alokacja się nie udała? Bo wtedy
//zakłada, że ścieżka już istnieje
bool addToRoute(Map *map, unsigned routeId, const char *city) {
    if (map == NULL) {
        return false;
    }

    if (city == NULL) {
        return false;
    }

    if (map->routes[routeId] == NULL) {
        return false;
    }

    List *restrictedCities = map->routes[routeId];

    Route *tempRoute1 = dijkstra(searchDictionary(map->cities, city), map->routes[routeId]->source, restrictedCities);

    if (tempRoute1 == NULL) {
        return false;
    }

    Route *tempRoute2 = dijkstra(map->routes[routeId]->destination, searchDictionary(map->cities, city), restrictedCities);

    if (tempRoute2 == NULL) {
        deleteRoute(tempRoute1);

        return false;
    }

    int compareResult = compareRoute(tempRoute1, tempRoute2);

    if (compareResult == 0) {
        deleteRoute(tempRoute1);
        deleteRoute(tempRoute2);

        return false;
    }

    if (compareResult == 1) {
        deleteRoute(tempRoute2);

        insertToList(map->routes[routeId]->path, tempRoute1);
        map->routes[routeId]->source = tempRoute1->source;
        map->routes[routeId]->minimalYear = min(map->routes[routeId]->minimalYear, tempRoute1->minimalYear);
        map->routes[routeId]->length += tempRoute1->length;
    }

    if (compareResult == -1) {
        deleteRoute(tempRoute2);

        insertToList(map->routes[routeId]->path, tempRoute2);
        map->routes[routeId]->destination = tempRoute1->destination;
        map->routes[routeId]->minimalYear = min(map->routes[routeId]->minimalYear, tempRoute1->minimalYear);
        map->routes[routeId]->length += tempRoute1->length;
    }

    return true;
}

//Nie usuwa miast!!!
void deleteRoute(Route *route) {
    if (route == NULL) {
        return;
    }

    deleteList(route->path, deleteCity);

    free(route);
}

