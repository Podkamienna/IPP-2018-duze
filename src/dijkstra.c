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
#include <inttypes.h>
#include <limits.h>

typedef struct Distance Distance;
typedef struct HeapEntry HeapEntry;

const Distance BASE_DISTANCE;
const Distance WORST_DISTANCE;

struct Distance {
    uint64_t length;
    int minYear;
};

struct HeapEntry {
    City *city;
    Distance distance;
};

const Distance BASE_DISTANCE = (Distance) {0, INT_MAX};
const Distance WORST_DISTANCE = (Distance) {UINT64_MAX, INT_MIN};

static Distance getDistance(uint64_t length, int minYear) {
    return (Distance) {length, minYear};
}

static int compareDistance(Distance distance1, Distance distance2) {
    if (distance1.length > distance2.length) {
        return 1;
    }

    if (distance1.length < distance2.length) {
        return -1;
    }

    if (distance1.minYear > distance2.minYear) {
        return 1;
    }

    if (distance1.minYear < distance2.minYear) {
        return -1;
    }

    return 0;
}

static Distance addDistance(Distance distance1, Distance distance2) {
    Distance result = distance1;

    result.length += distance2.length;

    if (result.minYear > distance2.minYear) {
        result.minYear = distance2.minYear;
    }

    return result;
}

static Distance addRoadToDistance(Distance distance, Road *road) {
    return addDistance(distance, getDistance(road->length, road->year));
}

static HeapEntry *getNewHeapEntry(City *city, Distance distance) {
    HeapEntry *newHeapEntry = malloc(sizeof(HeapEntry));

    if (newHeapEntry == NULL) {
        return NULL;
    }

    newHeapEntry->city = city;
    newHeapEntry->distance = distance;

    return newHeapEntry;
}

static int compareHeapEntry(HeapEntry *heapEntry1, HeapEntry *heapEntry2) {
    return compareDistance(heapEntry1->distance, heapEntry2->distance);
}

static void deleteHeapEntry(HeapEntry *heapEntry) {
    free(heapEntry);
}

bool pushNeighbours(HeapEntry *entry, Heap *heap, bool *isVisited, bool *isRestricted) {
    SetIterator *setIterator = getNewSetIterator(entry->city->roads);

    if (setIterator == NULL) {
        return false;
    }

    HeapEntry *newEntry = NULL;

    for(Road *road = getNextSetIterator(setIterator); road != NULL; road = getNextSetIterator(setIterator)) {
        City *neighbour = getNeighbour(road, entry->city);

        if (neighbour == NULL) {
            continue;
        }

        if (isRestricted[neighbour->id] || isVisited[neighbour->id]) {
            continue;
        }

        newEntry = getNewHeapEntry(neighbour, addRoadToDistance(entry->distance, road));

        if (newEntry == NULL) {
            deleteSetIterator(setIterator);

            return false;
        }

        if (!pushHeap(heap, newEntry)) {
            deleteSetIterator(setIterator);
            deleteHeapEntry(newEntry);

            return false;
        }
    }

    deleteSetIterator(setIterator);

    return true;
}

//TODO pozmieniać jakoś te wszystkie nazwy i wgl, nowa struktura na drogi krajowe (tu zrobić jakąś inną)
//i potem przepisac te drogi krajowe
//przy extend route uważać na minimalny rok!!!
//Może przekazywać napisy i mapę?
Distance *calculateDistances(Map *map, City *source, City *destination, bool *isRestricted) {
    if (source == NULL || destination == NULL) {
        return NULL;
    }

    Heap *heap = initializeHeap((int (*)(void *, void *)) compareHeapEntry);

    if (heap == NULL) {
        return NULL;
    }

    HeapEntry *temp = getNewHeapEntry(source, BASE_DISTANCE); //wierzcholek poczatkowy

    if (temp == NULL) {
        deleteHeap(heap, (void (*)(void *)) deleteHeapEntry);

        return NULL;
    }

    if (!pushHeap(heap, temp)) {
        deleteHeap(heap, (void (*)(void *)) deleteHeapEntry);
        deleteHeapEntry(temp);

        return NULL;
    }

    size_t cityCount = getId(map->cities);
    bool *isVisited = calloc(cityCount, sizeof(bool));
    Distance *distances = malloc(cityCount * sizeof(Distance));

    if (isVisited == NULL || distances == NULL) {
        deleteHeap(heap, (void (*)(void *)) deleteHeapEntry);
        free(isVisited);
        free(distances);

        return NULL;
    }

    for (size_t i = 0; i < cityCount; i++) {
        distances[i] = WORST_DISTANCE;
    }

    while (!isEmptyHeap(heap)) {
        temp = popHeap(heap);

        if (isVisited[temp->city->id]) {
            continue;
        }

        isVisited[temp->city->id] = true;
        distances[temp->city->id] = temp->distance;

        if (compareCities(temp->city, destination) == 0) {
            deleteHeapEntry(temp);

            break;
        }

        if (!pushNeighbours(temp, heap, isVisited,
                            isRestricted)) { //przechodzimy po sasiadach zadanego wierzcholka, jezeli nie byli odw to ich wrzucamy na kopiec
            deleteHeap(heap, (void(*)(void *)) deleteHeapEntry);

            return NULL;
        }

        deleteHeapEntry(temp);
    }

    deleteHeap(heap, (void(*)(void *)) deleteHeapEntry);
    free(isVisited);

    return distances;
}

List *reconstructRoute(Map *map, City *source, City *destination, Distance *distances) {
    List *path = initializeList((int(*)(void *, void *))comparePathNodes);

    if (path == NULL) {
        free(distances);

        return NULL;
    }

    City *position = destination;
    City *potentialNextPosition;

    addToList(path, getNewPathNode(position, NULL));

    while (true) {
        potentialNextPosition = NULL;
        SetIterator *setIterator = getNewSetIterator(position->roads);

        if (setIterator == NULL) {
            deleteList(path, (void(*)(void *))deletePathNode);

            return NULL;
        }

        for (Road *road = getNextSetIterator(setIterator); road != NULL; road = getNextSetIterator(setIterator)) {
            City *neighbour = getNeighbour(road, position);

            if (neighbour == NULL) {
                continue;
            }

            if (compareDistance(distances[position->id], addRoadToDistance(distances[neighbour->id], road)) == 0) {
                if (potentialNextPosition != NULL) {
                    deleteSetIterator(setIterator);

                    return path;
                }

                potentialNextPosition = neighbour;
                addToList(path, getNewPathNode(neighbour, road));
            }

            if (compareCities(neighbour, source) == 0) {
                deleteSetIterator(setIterator);

                return path;
            }
        }

        position = potentialNextPosition;
        deleteSetIterator(setIterator);
    }
}

Route *dijkstra(Map *map, City *source, City *destination, List *restrictedPaths) {
    ListIterator *listIterator = getNewListIterator(restrictedPaths);

    if (listIterator == NULL && restrictedPaths != NULL) {
        return NULL;
    }

    size_t cityCount = getId(map->cities);
    bool *isRestricted = calloc(cityCount, sizeof(bool));

    while (true) {
        Path *path = getNextListIterator(listIterator);

        if (path == NULL) {
            break;
        }

        if (compareCities(path->city, source) == 0 || compareCities(path->city, destination) == 0) {
            continue;
        }

        isRestricted[path->city->id] = true;
    }

    Distance *distances = calculateDistances(map, source, destination, isRestricted);
    Route *route = getNewRoute();

    if (route == NULL) {
        return NULL;
    }

    if (compareDistance(distances[destination->id], WORST_DISTANCE) == 0) { //Jeżeli nie ma ścieżki
        free(distances);
        free(isRestricted);
        deleteListIterator(listIterator);
        route->minimalYear = 0;

        return route;
    }

    List *list = reconstructRoute(map, source, destination, distances);
    Path *endOfList = getLast(list);

    route->source = source;
    route->destination = destination;
    route->minimalYear = distances[destination->id].minYear;
    route->length = distances[destination->id].length;

    if (compareCities(endOfList->city, source) != 0) {
        free(distances);
        free(isRestricted);
        deleteListIterator(listIterator);

        route->path = NULL;

        return route; //!!!!!!!!!!!!!trzeba najpierw wyifować, czy jest UNIQUE po wywołaniu
    }

    free(distances);
    free(isRestricted);
    deleteListIterator(listIterator);

    route->path = list;

    return route;
}
