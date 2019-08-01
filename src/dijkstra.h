//
// Created by alicja on 29.03.19.
//

#ifndef DROGI_DIJKSTRA_H
#define DROGI_DIJKSTRA_H

#include "map.h"
#include "definitions.h"

void deleteRoute(Route *route);

Route *dijkstra(City *start, City *end);

#endif //DROGI_DIJKSTRA_H
