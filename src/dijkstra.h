//
// Created by alicja on 29.03.19.
//

#ifndef DROGI_DIJKSTRA_H
#define DROGI_DIJKSTRA_H

#include "map.h"
#include "definitions.h"
#include "list.h"

DijkstraReturnValue *getNewDijkstraReturnValue();

bool isCorrectDijkstraReturnValue(DijkstraReturnValue *dijkstraReturnValue);

Route *dijkstraReturnValueToRoute(DijkstraReturnValue *dijkstraReturnValue);

int compareDijkstraReturnValues(DijkstraReturnValue *a, DijkstraReturnValue *b);

void deleteDijkstraReturnValue(DijkstraReturnValue *dijkstraReturnValue, bool deletePath);

DijkstraReturnValue *dijkstra(Map *map, City *source, City *destination, List *restrictedCities);

#endif //DROGI_DIJKSTRA_H
