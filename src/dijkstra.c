//
// Created by alicja on 13.07.19.
//
#include "dijkstra.h"
#include "heap.h"
#include "citiesAndRoads.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct Route Route;
typedef struct City City;
typedef struct Road Road;

Route *getNewRoute(City *node, Road *edge, int priority, int minYear, Route *pr) {
    Route *newRoute = NULL;
    newRoute = malloc(sizeof(Route));

    if (newRoute == NULL) {
        return NULL;
    }

    newRoute->city = node;
    newRoute->priority = priority;
    newRoute->minYear = minYear;
    newRoute->prev = pr;
    newRoute->edge = edge;

    return newRoute;
}


int compare(Route *a, Route *b) { //function to compare to graphNodes, to get a good order in dijkstra
    if(a->priority > b->priority)
        return -1;

    if(a->priority == b->priority && a->minYear < b->minYear)
        return -1;

    if(a->priority == b->priority && a->minYear > b->minYear)
        return 1;

    if(a->priority == b->priority && a->minYear == b->minYear)
        return 0;

    return 1;
}

int getNonzeroMin(int a, int b) {
    if (a == 0)
        return b;

    if (b == 0)
        return a;

    if (a < b)
        return a;

    return b;
}

void pushNeighbours(Route *graph, Heap *heap) {
    SetIterator *setIterator = getNewSetIterator(graph->city->roads);
    Route *temp1 = NULL;
    Road *road;
    City *neighbour;

    while (true) { //sprawdzic, czy temp1 != end, jesli tak, to jezeli jest lepszy od prevEnd
        road = getNextSetIterator(setIterator);
        neighbour = getNeighbour(road, graph->city);

        if (neighbour == NULL) {
            break;
        }

            temp1 = getNewRoute(neighbour, road, (graph->priority) + (road->length),
                                    getNonzeroMin(road->year, graph->minYear), graph);

            if (!isVisited(temp1->city)) {
                pushHeap(temp1, heap);
            }
        }
    }

//zrobic tak, zeby roadNode byl w graphNode, lista drog krajowych na ktorej jest droga tez
//no i jeszcze ogarnac usuwanie
Route *dijkstra(City *start, City *end) {
    Heap *priorityQueue = initializeHeap();
    Route *temp = getNewRoute(start, NULL, 0, 0,  NULL); //wierzcholek poczatkowy
    pushHeap(temp, priorityQueue);
    Route *worseEnd = NULL, *tempEnd = NULL; //zmienna przechowujaca graf koncowy przed ostatnia zmiana na lepsze (zeby sprawdzic, czy jednoznaczne)
    while (!isEmptyHeap(priorityQueue) && worseEnd == NULL) {
        Route *temp = NULL;
        temp = popHeap(priorityQueue);

        if(tempEnd != NULL && compareCities(temp->city, end) == 0) {
            worseEnd = temp;
        }

        if(tempEnd == NULL && compareCities(temp->city, end) == 0) {
            tempEnd = temp;
        }

        pushNeighbours(temp->city, priorityQueue); //przechodzimy po sasiadach zadanego wierzcholka, jezeli nie byli odw to ich wrzucamy na kopiec
        visit(temp->city); //ustawiamy obecny wierzcholek na odwiedzony
        free(temp);
    }
    if (tempEnd == NULL || (worseEnd != NULL && compare(worseEnd, tempEnd) == 0)) //sprawdzenie czy jednoznaczne
        return NULL;
    return tempEnd;
}
