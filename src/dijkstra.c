//
// Created by alicja on 13.07.19.
//
#include "dijkstra.h"
#include "heap.h"
#include "citiesAndRoads.h"
#include "vector.h"
#include "list.h"
#include "set.h"
#include "route.h"
#include "dictionary.h"

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

void deleteRouteDijkstra(void *value) {
    if (value == NULL) {
        return;
    }

    RouteSection *route = value;

    if (route->prev == NULL) {
        return;
    }

    deleteRouteDijkstra(route->prev);

    free(route);
}

int compareRouteSection(void *a, void *b) {
    RouteSection *routeSection1 = a;
    RouteSection *routeSection2 = b;

    if(routeSection1->priority > routeSection2->priority)
        return 1;

    if(routeSection1->priority == routeSection2->priority && routeSection1->minYear < routeSection2->minYear)
        return 1;

    if(routeSection1->priority == routeSection2->priority && routeSection1->minYear > routeSection2->minYear)
        return -1;

    if(routeSection1->priority == routeSection2->priority && routeSection1->minYear == routeSection2->minYear)
        return 0;

    return -1;
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

    while(position != NULL) {
        if (!addToList(newRoute->path, getNewPathNode(position->city, position->road))) {
            deleteRoute(newRoute);

            return NULL;
        }

        position = position->prev;
    }

   // reverseList(newRoute->path);

    return newRoute;
}

bool pushNeighbours(RouteSection *graph, Heap *heap, Vector *routes, List *restrictedPaths, bool *isVisited) {
    SetIterator *setIterator = getNewSetIterator(graph->city->roads);

    if (setIterator == NULL) {
        return false;
    }

    RouteSection *temp1 = NULL;
    Road *road;
    City *neighbour;

    while (true) { //sprawdzic, czytemp1 != end, jesli tak, to jezeli jest lepszy od prevEnd
        road = getNextSetIterator(setIterator);

        if (road == NULL) {
            break;
        }

        neighbour = getNeighbour(road, graph->city);

        PathNode *pathNode = getNewPathNode(neighbour, road); //tu jeszcze trzeba ogarnąć, czy jest ok

        if (pathNode == NULL) {
            return false;
        }

        if (neighbour == NULL) {
            break;
        }

        if (!exists(restrictedPaths, pathNode) && !isVisited[neighbour->id]) {

            temp1 = getNewRouteSection(neighbour, road, (graph->priority) + (road->length),
                                       nonzeroMin(road->year, graph->minYear), graph);

            if (temp1 == NULL) {
                deleteSetIterator(setIterator);

                return false;
            }

            if (!pushToVector(routes, temp1)) {
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
Route *dijkstra(Map *map, City *source, City *destination, List *restrictedPaths) {
    if (source == NULL || destination == NULL) {
        return NULL;
    }

    Heap *heap = initializeHeap(compareRouteSection);

    if (heap == NULL) {
        return NULL;
    }

    RouteSection *temp = getNewRouteSection(source, NULL, 0, 0, NULL); //wierzcholek poczatkowy

    if (temp == NULL) {
        deleteHeap(heap, deleteRouteDijkstra);

        return NULL;
    }

    Vector *routes = initializeVector();

    if (routes == NULL) {
        deleteHeap(heap, NULL);
        deleteRouteDijkstra(temp);

        return NULL;
    }

    if (!pushHeap(heap, temp)) {
        deleteHeap(heap, NULL);
        deleteRouteDijkstra(temp);
        deleteVector(routes, deleteRouteSection);

        return NULL;
    }

    if (!pushToVector(routes, temp)) {
        deleteHeap(heap, NULL); //TODO jak memleaki, to pewnie tu
        deleteVector(routes, deleteRouteSection);

        return NULL;
    }

    size_t visitedSize = getId(map->cities);
    bool *isVisited = calloc(visitedSize, sizeof(bool));

    if (isVisited == NULL) {
        deleteHeap(heap, NULL); //TODO jak memleaki, to pewnie tu
        deleteVector(routes, deleteRouteSection);

        return NULL;
    }


    RouteSection *worseEnd = NULL, *tempEnd = NULL; //zmienna przechowujaca graf koncowy przed ostatnia zmiana na lepsze (zeby sprawdzic, czy jednoznaczne)
    while (!isEmptyHeap(heap)) {
        temp = popHeap(heap);

        if (isVisited[temp->city->id]) {
            continue;

        }

        if(tempEnd != NULL && compareCities(temp->city, destination) == 0) {
            worseEnd = temp;
            break;
        }

        if(tempEnd == NULL && compareCities(temp->city, destination) == 0) {
            tempEnd = temp;
        }

        if (!pushNeighbours(temp, heap, routes, restrictedPaths, isVisited)) { //przechodzimy po sasiadach zadanego wierzcholka, jezeli nie byli odw to ich wrzucamy na kopiec
            deleteHeap(heap, NULL);
            deleteVector(routes, deleteRouteSection);

            return NULL;
        }

        isVisited[temp->city->id] = true; //ustawiamy obecny wierzcholek na odwiedzony
    }

    Route *newRoute = createRoute(tempEnd, source, destination);

    if (newRoute == NULL) {
        deleteHeap(heap, NULL);
        deleteVector(routes, deleteRouteSection);

        return NULL;
    }

    if (tempEnd == NULL || (worseEnd != NULL && compareRouteSection(worseEnd, tempEnd) == 0)) {//sprawdzenie czy jednoznaczne
        newRoute->minimalYear = 0;
    }

    deleteHeap(heap, NULL);
    deleteVector(routes, deleteRouteSection);

    return newRoute;
}
