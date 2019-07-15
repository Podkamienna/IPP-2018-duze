//
// Created by alicja on 29.03.19.
//

#ifndef DROGI_DIJKSTRA_H
#define DROGI_DIJKSTRA_H

#include "map.h"
#define INITIAL_HEAP_SIZE 100

typedef struct priorityQueue priorityQueue;
typedef struct graphNode graphNode;
typedef struct roadNode roadNode;
typedef struct city city;


void clearVisited(city *mapa);

graphNode *getNewGraphNode(struct city *mapa, int priority, int minYear, graphNode *pr, priorityQueue **prQu, struct roadNode *road);

graphNode *getGraphNode(struct city *mapa, int priority, int minYear, graphNode *pr, graphNode *temp, struct roadNode *iteroad);

graphNode *dijkstra(priorityQueue *prQu, struct city *start, struct city *end);

priorityQueue *initializeHeap();

void insertHeap(graphNode *node, priorityQueue **prQueu);

void seeHeap(priorityQueue *prQu);

priorityQueue *pop(priorityQueue *prQu);

#endif //DROGI_DIJKSTRA_H
