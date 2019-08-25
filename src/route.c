//
// Created by alicja on 19.07.19.
//

#include "route.h"
#include "definitions.h"
#include "citiesAndRoads.h"
#include "list.h"

#include <stdbool.h>
#include <stdlib.h>

/** Stałe z minimalnym i maksymalnym numerem drogi krajowej */
const unsigned MINIMAL_ROUTE_ID = 1;
const unsigned MAXIMAL_ROUTE_ID = 999;

PathNode *getNewPathNode(City *city, Road *road) {
    PathNode *pathNode = malloc(sizeof(PathNode));

    if (pathNode == NULL) {
        return NULL;
    }

    pathNode->city = city;
    pathNode->road = road;

    return pathNode;
}

void deletePathNode(PathNode *pathNode) {
    free(pathNode);
}

bool areEqualPathNodes(PathNode *a, PathNode *b) {
    if (a->road == NULL || b->road == NULL) {
        return areEqualCities(a->city, b->city);
    }

    return areEqualRoads(a->road, b->road);
}

bool areEqualPathNodesByCities(PathNode *a, PathNode *b) {
    return areEqualCities(a->city, b->city);
}

Route *getNewRoute(List *path, City *source, City *destination) {
    Route *newRoute = malloc(sizeof(Route));

    if (newRoute == NULL) {
        return NULL;
    }

    newRoute->path = path;
    newRoute->destination = destination;
    newRoute->source = source;

    return newRoute;
}

void deleteRoute(Route *route) {
    if (route == NULL) {
        return;
    }

    deleteList(route->path, (void (*)(void *)) deletePathNode);

    free(route);
}

