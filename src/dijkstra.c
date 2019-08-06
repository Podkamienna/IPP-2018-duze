//
// Created by alicja on 13.07.19.
//
#include "dijkstra.h"
#include "heap.h"
#include "citiesAndRoads.h"
#include "vector.h"
#include "list.h"
#include "route.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct routeSection routeSection;

struct routeSection {
    City *city;
    routeSection *prev;
    int priority; //priority = najmn ob znana odl wierzch city od wierzcholka startowego
    int minYear; //minimalny rok ostatniego remontu na najkrótszej sciezce od wierzch start do obecnego
};

routeSection *getNewRouteSection(City *node, int priority, int minYear, routeSection *pr) {
    routeSection *newPath = NULL;
    newPath = malloc(sizeof(routeSection));

    if (newPath == NULL) {
        return NULL;
    }

    newPath->city = node;
    newPath->priority = priority;
    newPath->minYear = minYear;
    newPath->prev = pr;

    return newPath;
}

void deleteRouteSection(routeSection *path) {
    free(path);
}

void deleteRouteDijkstra(routeSection *route) {
    if (route->prev == NULL) {
        return;
    }

    deleteRouteDijkstra(route->prev);
    free(route);
}

int compareRouteSection(routeSection *a, routeSection *b) {
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

int nonzeroMin(int a, int b) {
    if (a == 0)
        return b;

    if (b == 0)
        return a;

    if (a < b)
        return a;

    return b;
}

//tworzy listę, będącą drogą krajową
Route *createRoute(routeSection *tempRoute, City *source, City *destination) {
    Route *newRoute = getNewRoute();

    if (newRoute == NULL) {
        return NULL;
    }

    newRoute->length = tempRoute->priority;
    newRoute->minimalYear = tempRoute->minYear;
    newRoute->source = source;
    newRoute->destination = destination;

    routeSection *position = tempRoute;

    while (position->prev != NULL) {
        if (!addToList(newRoute->path, position->city)) {
            deleteRoute(newRoute);

            return NULL;
        }
    }

    return newRoute;
}

bool pushNeighbours(routeSection *graph, Heap *heap, Vector *routes, List *restrictedCities) {
    SetIterator *setIterator = getNewSetIterator(graph->city->roads);

    if (setIterator == NULL) {
        return false;
    }

    routeSection *temp1 = NULL;
    Road *road;
    City *neighbour;

    while (true) { //sprawdzic, czy temp1 != end, jesli tak, to jezeli jest lepszy od prevEnd
        road = getNextSetIterator(setIterator);
        neighbour = getNeighbour(road, graph->city);

        if (!exists(restrictedCities, neighbour) && !isVisited(neighbour)) {

            if (neighbour == NULL) {
                break;
            }

            temp1 = getNewRouteSection(neighbour, (graph->priority) + (road->length),
                                       nonzeroMin(road->year, graph->minYear), graph);

            if (temp1 == NULL) {
                deleteSetIterator(setIterator);

                return false;
            }

            if (!pushVector(routes, temp1)) {
                deleteSetIterator(setIterator);

                return false;
            }

            if (!pushHeap(heap, temp1)) {
                deleteSetIterator(setIterator);
                deleteRouteDijkstra(temp1);

                return false;
            }
        }
        }
    }

//TODO pozmieniać jakoś te wszystkie nazwy i wgl, nowa struktura na drogi krajowe (tu zrobić jakąś inną)
//i potem przepisac te drogi krajowe
//przy extend route uważać na minimalny rok!!!
//Może przekazywać napisy i mapę?
Route *dijkstra(City *source, City *destination, List *restrictedCities) {
    if (source == NULL || destination == NULL) {
        return NULL;
    }

    Heap *priorityQueue = initializeHeap(compareRouteSection);

    if (priorityQueue == NULL) {
        return NULL;
    }

    routeSection *temp = getNewRouteSection(source,  0, 0, NULL); //wierzcholek poczatkowy

    if (temp == NULL) {
        deleteHeap(priorityQueue, deleteRouteDijkstra);

        return NULL;
    }

    Vector *routes = initializeVector();

    if (routes == NULL) {
        deleteHeap(priorityQueue, NULL);
        deleteRouteDijkstra(temp);

        return NULL;
    }

    if (!pushHeap(temp, priorityQueue)) {
        deleteHeap(priorityQueue, NULL);
        deleteRouteDijkstra(temp);
        deleteVector(routes, deleteRouteDijkstra);

        return NULL;
    }

    if (!pushVector(routes, temp)) {
        deleteHeap(priorityQueue, NULL);
        deleteVector(routes, deleteRouteDijkstra);

        return NULL;
    }

    routeSection *worseEnd = NULL, *tempEnd = NULL; //zmienna przechowujaca graf koncowy przed ostatnia zmiana na lepsze (zeby sprawdzic, czy jednoznaczne)
    while (!isEmptyHeap(priorityQueue) && worseEnd == NULL) {
        temp = popHeap(priorityQueue);

        if(tempEnd != NULL && compareCities(temp->city, destination) == 0) {
            worseEnd = temp;
        }

        if(tempEnd == NULL && compareCities(temp->city, destination) == 0) {
            tempEnd = temp;
        }

        if (!pushNeighbours(temp->city, priorityQueue, routes, restrictedCities)) { //przechodzimy po sasiadach zadanego wierzcholka, jezeli nie byli odw to ich wrzucamy na kopiec
            deleteHeap(priorityQueue, NULL);
            deleteVector(routes, deleteRouteDijkstra);

            return NULL;
        }

        visit(temp->city); //ustawiamy obecny wierzcholek na odwiedzony
    }
    if (tempEnd == NULL || (worseEnd != NULL && compareRouteSection(worseEnd, tempEnd) == 0)) {//sprawdzenie czy jednoznaczne
        deleteHeap(priorityQueue, NULL);
        deleteVector(routes, deleteRouteDijkstra);

        return NULL;
    }

    Route *newRoute = createRoute(tempEnd, source, destination);

    if (newRoute == NULL) {
        deleteHeap(priorityQueue, NULL);
        deleteVector(routes, deleteRouteDijkstra);

        return NULL;
    }

    deleteHeap(priorityQueue, NULL);
    deleteVector(routes, deleteRouteDijkstra);

    return newRoute;
}
