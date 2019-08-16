//
// Created by alicja on 29.03.19.
//

#ifndef DROGI_DIJKSTRA_H
#define DROGI_DIJKSTRA_H

#include "map.h"
#include "definitions.h"
#include "list.h"


Route *dijkstra(Map *map, City *source, City *destination, List *restrictedPaths);

#endif //DROGI_DIJKSTRA_H
