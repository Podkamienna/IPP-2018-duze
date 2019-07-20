//
// Created by alicja on 29.03.19.
//

#ifndef DROGI_DIJKSTRA_H
#define DROGI_DIJKSTRA_H

#include "map.h"
#include "definitions.h"

typedef struct Graph Graph;
typedef struct City City;

Graph *dijkstra(City *start, City *end);

#endif //DROGI_DIJKSTRA_H
