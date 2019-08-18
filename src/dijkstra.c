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

            return false;
        }
    }

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

        return NULL;
    }

    isVisited[source->id] = true;

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
            break;
        }

        if (!pushNeighbours(temp, heap, isVisited,
                            isRestricted)) { //przechodzimy po sasiadach zadanego wierzcholka, jezeli nie byli odw to ich wrzucamy na kopiec
            deleteHeap(heap, (void(*)(void *)) deleteHeapEntry);

            return NULL;
        }

    }

    deleteHeap(heap, (void(*)(void *)) deleteHeapEntry);
    free(isVisited);

    return distances;
}

Route *reconstructRoute(Map *map, City *source, City *destination, Distance *distances) {
    List *path = initializeList((int(*)(void *, void *))comparePathNodes); //sprawdzić wszystkie compare

    if (path == NULL) {
        free(distances);

        return NULL;
    }

    Route *result = getNewRoute();

    if (result == NULL) {
        free(distances);
        deleteList(path, NULL);

        return NULL;
    }

    City *position = destination;

    Distance currentDistance = BASE_DISTANCE;
    Distance endDistance = distances[destination->id];

    while (compareCities(position, source) != 0) {
        City *newPosition = NULL;
        Distance newCurrentDistance = WORST_DISTANCE;
        SetIterator *setIterator = getNewSetIterator(position->roads);

        for (Road *road = getNextSetIterator(setIterator); road != NULL; road = getNextSetIterator(setIterator)) {
            City *neighbour = getNeighbour(road, position);

            if (neighbour == NULL) {
                continue;
            }

            Distance newDistance = addRoadToDistance(distances[neighbour->id], road);
            newDistance = addDistance(newDistance, currentDistance);

            if (compareDistance(newDistance, endDistance) == 0) {
                if (newPosition != NULL) {
                    result->isUnique = false;

                    return result;
                }

                if (!addToList(path, getNewPathNode(position, road))) {
                    return NULL;
                }

                newPosition = neighbour;
                newCurrentDistance = addRoadToDistance(currentDistance, road);
            }
        }

        if (newPosition == NULL) {
            result->minimalYear = 0;

            return result;
        }

        position = newPosition;
        currentDistance = newCurrentDistance;
    }

    result->path = path;
    return result;
}

Route *dijkstra(Map *map, City *source, City *destination, List *restrictedPaths) {
    ListIterator *listIterator = getNewListIterator(restrictedPaths);

    if (listIterator == NULL) {
        return NULL;
    }

    size_t cityCount = getId(map->cities);
    bool *isRestricted = calloc(cityCount, sizeof(bool));

    while (true) {
        Path *path = getNextListIterator(listIterator);

        if (path == NULL) {
            break;
        }

        isRestricted[path->city->id] = true;
    }

    Distance *distances = dijkstra(map, source, destination, isRestricted);

    return reconstructRoute(map, source, destination, distances);
}
