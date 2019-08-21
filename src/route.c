//
// Created by alicja on 19.07.19.
//

#include "route.h"
#include "definitions.h"
#include "citiesAndRoads.h"
#include "dictionary.h"
#include "list.h"

#include <stdlib.h>

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

int comparePathNodes(PathNode *a, PathNode *b) {
    if (a->road == NULL || b->road == NULL) {
        return compareCities(a->city, b->city);
    }

    return compareRoads(a->road, b->road);
}

int comparePathNodes2(PathNode *a, PathNode *b) {
    return compareCities(a->city, b->city);
}

Route *getNewRoute() {
    Route *newRoute = malloc(sizeof(Route));

    if (newRoute == NULL) {
        return NULL;
    }

    newRoute->path = NULL;

    return newRoute;
}

void deleteRoute(Route *route, bool deletePath) {
    if (route == NULL) {
        return;
    }

    if (deletePath) {
        deleteList(route->path, (void (*)(void *))deletePathNode);
    }

    free(route);
}

