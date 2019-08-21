//
// Created by alicja on 19.07.19.
//

#include "route.h"
#include "definitions.h"
#include "dijkstra.h"
#include "citiesAndRoads.h"
#include "dictionary.h"

#include <stdlib.h>
#include <string.h>

int comparePathNodes(Path *a, Path *b) {
    if (a->road == NULL || b->road == NULL) {
        return compareCities(a->city, b->city);
    }

    return compareRoads(a->road, b->road);
}

int comparePathNodes2(Path *a, Path *b) {
    return compareCities(a->city, b->city);
}

Route *getNewRoute() {
    Route *newRoute = malloc(sizeof(Route));

    if (newRoute == NULL) {
        return NULL;
    }

    newRoute->path = NULL;
    newRoute->isUnique = true;

    return newRoute;
}

bool isCorrectRoute(Route *route) {
    if (route == NULL) {
        return false;
    }

    if (route->path == NULL) {
        return false;
    }

    if (route->minimalYear == 0) {
        return false;
    }

    return route->isUnique;
}

int compareRoute(Route *route1, Route *route2) {
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

Path *getNewPathNode(City *city, Road *road) {
    Path *pathNode = malloc(sizeof(Path));

    if (pathNode == NULL) {
        return NULL;
    }

    pathNode->city = city;
    pathNode->road = road;

    return pathNode;
}

void deletePathNode(Path *pathNode) {
    free(pathNode);
}

void deleteRoute(Route *route, bool deletePath) {
    if (route == NULL) {
        return;
    }

    if (deletePath) {
        deleteList(route->path, (void (*)(void *)) deletePathNode);
    }

    free(route);
}

