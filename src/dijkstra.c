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

typedef struct RouteSection RouteSection;

struct RouteSection {
    City *city;
    Road *road; //droga, którą weszliśmy do city
    RouteSection *prev;
    int priority; //priority = najmn ob znana odl wierzch city od wierzcholka startowego
    int minYear; //minimalny rok ostatniego remontu na najkrótszej sciezce od wierzch start do obecnego
};

RouteSection *getNewRouteSection(City *city, Road *road, int priority, int minYear, RouteSection *pr) {
    RouteSection *newPath = NULL;
    newPath = malloc(sizeof(RouteSection));

    if (newPath == NULL) {
        return NULL;
    }

    newPath->city = city;
    newPath->road = road;
    newPath->priority = priority;
    newPath->minYear = minYear;
    newPath->prev = pr;

    return newPath;
}

void deleteRouteSection(RouteSection *path) {
    free(path);
}

void deleteRouteDijkstra(RouteSection *route) {
    if (route->prev == NULL) {
        return;
    }

    deleteRouteDijkstra(route->prev);
    free(route);
}

int compareRouteSection(RouteSection *a, RouteSection *b) {
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
Route *createRoute(RouteSection *tempRoute, City *source, City *destination) {
    Route *newRoute = getNewRoute();

    if (newRoute == NULL) {
        return NULL;
    }

    newRoute->length = tempRoute->priority;
    newRoute->minimalYear = tempRoute->minYear;
    newRoute->source = source;
    newRoute->destination = destination;

    RouteSection *position = tempRoute;

    while (position->prev != NULL) {
        if (!addToList(newRoute->path, getNewPathNode(position->city, position->road))) {
            deleteRoute(newRoute);

            return NULL;
        }
    }

    return newRoute;
}

bool pushNeighbours(RouteSection *graph, Heap *heap, Vector *routes, List *restrictedPaths) {
    SetIterator *setIterator = getNewSetIterator(graph->city->roads);

    if (setIterator == NULL) {
        return false;
    }

    RouteSection *temp1 = NULL;
    Road *road;
    City *neighbour;

    while (true) { //sprawdzic, czy temp1 != end, jesli tak, to jezeli jest lepszy od prevEnd
        road = getNextSetIterator(setIterator);

        if (road == NULL) {
            break;
        }

        neighbour = getNeighbour(road, graph->city);

        PathNode *pathNode = getNewPathNode(neighbour, road); //tu jeszcze trzeba ogarnąć, czy jest ok

        if (pathNode == NULL) {
            return false;
        }

        if (!exists(restrictedPaths, pathNode) && !isVisited(neighbour)) {

            if (neighbour == NULL) {
                break;
            }

            temp1 = getNewRouteSection(neighbour, road, (graph->priority) + (road->length),
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
        return true;
    }

//TODO pozmieniać jakoś te wszystkie nazwy i wgl, nowa struktura na drogi krajowe (tu zrobić jakąś inną)
//i potem przepisac te drogi krajowe
//przy extend route uważać na minimalny rok!!!
//Może przekazywać napisy i mapę?
Route *dijkstra(City *source, City *destination, List *restrictedPaths) {
    if (source == NULL || destination == NULL) {
        return NULL;
    }

    Heap *priorityQueue = initializeHeap(compareRouteSection);

    if (priorityQueue == NULL) {
        return NULL;
    }

    RouteSection *temp = getNewRouteSection(source, NULL, 0, 0, NULL); //wierzcholek poczatkowy

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
        deleteHeap(priorityQueue, NULL); //TODO jak memleaki, to pewnie tu
        deleteVector(routes, deleteRouteDijkstra);

        return NULL;
    }

    RouteSection *worseEnd = NULL, *tempEnd = NULL; //zmienna przechowujaca graf koncowy przed ostatnia zmiana na lepsze (zeby sprawdzic, czy jednoznaczne)
    while (!isEmptyHeap(priorityQueue) && worseEnd == NULL) {
        temp = popHeap(priorityQueue);

        if(tempEnd != NULL && compareCities(temp->city, destination) == 0) {
            worseEnd = temp;
        }

        if(tempEnd == NULL && compareCities(temp->city, destination) == 0) {
            tempEnd = temp;
        }

        if (!pushNeighbours(temp->city, priorityQueue, routes, restrictedPaths)) { //przechodzimy po sasiadach zadanego wierzcholka, jezeli nie byli odw to ich wrzucamy na kopiec
            deleteHeap(priorityQueue, NULL);
            deleteVector(routes, deleteRouteDijkstra);

            return NULL;
        }

        visit(temp->city); //ustawiamy obecny wierzcholek na odwiedzony
    }

    Route *newRoute = createRoute(tempEnd, source, destination);

    if (newRoute == NULL) {
        deleteHeap(priorityQueue, NULL);
        deleteVector(routes, deleteRouteDijkstra);

        return NULL;
    }

    if (tempEnd == NULL || (worseEnd != NULL && compareRouteSection(worseEnd, tempEnd) == 0)) {//sprawdzenie czy jednoznaczne
        newRoute->minimalYear = 0;
    }

    deleteHeap(priorityQueue, NULL);
    deleteVector(routes, deleteRouteDijkstra);

    return newRoute;
}
