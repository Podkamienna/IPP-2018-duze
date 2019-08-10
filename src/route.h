//
// Created by alicja on 19.07.19.
//

#ifndef DROGI_ROUTE_H
#define DROGI_ROUTE_H

#include "definitions.h"

static const unsigned MINIMAL_ROUTE_ID = 1;
static const unsigned MAXIMAL_ROUTE_ID = 999;

typedef struct PathNode PathNode;

Route *getNewRoute();

PathNode *getNewPathNode(City *city, Road *road);

bool addNewRoute(Map *map, unsigned routeId, const char *city1, const char *city2);

bool insertToRoute(Map *map, unsigned routeId, const char *city1, const char *city2);

bool addToRoute(Map *map, unsigned routeId, const char *city);

void deleteRoute(Route *route);


#endif //DROGI_ROUTE_H
