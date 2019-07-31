//
// Created by alicja on 13.07.19.
//
#include "dijkstra.h"
#include "heap.h"
#include "citiesAndRoads.h"
#include "vector.h"

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

Route *copyRoute(Route *route) {
    if (route == NULL) {
        return NULL;
    }

    return getNewRoute(route->city, route->edge, route->priority, route->minYear, route->prev);
}

void deleteRoute(Route *route) {
    free(route);
}

void deleteRoute2(Route *route) {
    if (route->prev == NULL) {
        return;
    }

    deleteRoute2(route->prev);
    free(route);
}

int compare(Route *a, Route *b) {
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

Route *extractRoute(Route *tempRoute) {
    Route *newRoute = copyRoute(tempRoute);

    if (newRoute == NULL) {
        return NULL;
    }

    Route *position = newRoute;

    while (position->prev != NULL) {
        position->prev = copyRoute(position->prev);

        if (position->prev == NULL) {
            deleteRoute2(newRoute);

            return NULL;
        }
    }

    return newRoute;
}

bool pushNeighbours(Route *graph, Heap *heap, Vector *routes) {
    SetIterator *setIterator = getNewSetIterator(graph->city->roads);

    if (setIterator == NULL) {
        return false;
    }

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

        if (temp1 == NULL) {
            deleteSetIterator(setIterator);

            return false;
        }

        if (!pushVector(routes, temp1)) {
            deleteSetIterator(setIterator);

            return false;
        }

        if (!isVisited(temp1->city) && !(pushHeap(heap, temp1))) {
            deleteSetIterator(setIterator);
            deleteRoute(temp1);

            return false;
        }
        }
    }

//TODO pozmieniać jakoś te wszystkie nazwy i wgl, nowa struktura na drogi krajowe (tu zrobić jakąś inną)
//i potem przepisac te drogi krajowe
//przy extend route uważać na minimalny rok!!!
//Może przekazywać napisy i mapę?
Route *dijkstra(City *start, City *end) {
    if (start == NULL || end == NULL) {
        return NULL;
    }

    Heap *priorityQueue = initializeHeap(compare);

    if (priorityQueue == NULL) {
        return NULL;
    }

    Route *temp = getNewRoute(start, NULL, 0, 0,  NULL); //wierzcholek poczatkowy

    if (temp == NULL) {
        deleteHeap(priorityQueue, deleteRoute);

        return NULL;
    }

    Vector *routes = initializeVector();

    if (routes == NULL) {
        deleteHeap(priorityQueue, NULL);
        deleteRoute(temp);

        return NULL;
    }

    if (!pushHeap(temp, priorityQueue)) {
        deleteHeap(priorityQueue, NULL);
        deleteRoute(temp);
        deleteVector(routes, deleteRoute);

        return NULL;
    }

    if (!pushVector(routes, temp)) {
        deleteHeap(priorityQueue, NULL);
        deleteVector(routes, deleteRoute);

        return NULL;
    }

    Route *worseEnd = NULL, *tempEnd = NULL; //zmienna przechowujaca graf koncowy przed ostatnia zmiana na lepsze (zeby sprawdzic, czy jednoznaczne)
    while (!isEmptyHeap(priorityQueue) && worseEnd == NULL) {
        temp = popHeap(priorityQueue);

        if(tempEnd != NULL && compareCities(temp->city, end) == 0) {
            worseEnd = temp;
        }

        if(tempEnd == NULL && compareCities(temp->city, end) == 0) {
            tempEnd = temp;
        }

        if (!pushNeighbours(temp->city, priorityQueue, routes)) { //przechodzimy po sasiadach zadanego wierzcholka, jezeli nie byli odw to ich wrzucamy na kopiec
            deleteHeap(priorityQueue, NULL);
            deleteVector(routes, deleteRoute);

            return NULL;
        }

        visit(temp->city); //ustawiamy obecny wierzcholek na odwiedzony
    }
    if (tempEnd == NULL || (worseEnd != NULL && compare(worseEnd, tempEnd) == 0)) {//sprawdzenie czy jednoznaczne
        deleteHeap(priorityQueue, NULL);
        deleteVector(routes, deleteRoute);

        return NULL;
    }

    Route *newRoute = extractRoute(tempEnd);//TODO sprawdzić to wszystko, jestem zmęczona

    if (newRoute == NULL) {
        deleteHeap(priorityQueue, NULL);
        deleteVector(routes, deleteRoute);

        return NULL;
    }

    deleteHeap(priorityQueue, NULL);
    deleteVector(routes, deleteRoute);

    return newRoute;
}
