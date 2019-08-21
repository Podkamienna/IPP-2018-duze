//
// Created by alicja on 19.07.19.
//

#ifndef DROGI_ROUTE_H
#define DROGI_ROUTE_H

#include "definitions.h"

#include <stdbool.h>

static const unsigned MINIMAL_ROUTE_ID = 1;
static const unsigned MAXIMAL_ROUTE_ID = 999;

typedef struct Path Path;

int comparePathNodes(Path *a, Path *b);

int comparePathNodes2(Path *a, Path *b);

Path *getNewPathNode(City *city, Road *road);

void deletePathNode(Path *pathNode);

Route *getNewRoute();

bool isCorrectRoute(Route *route);

int compareRoute(Route *route1, Route *route2);

void deleteRoute(Route *route, bool deletePath);


#endif //DROGI_ROUTE_H
