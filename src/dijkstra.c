//
// Created by alicja on 13.07.19.
//
#include "dijkstra.h"
#include "heap.h"
#include "citiesAndRoads.h"
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

    if (distance1.minYear < distance2.minYear) {
        return 1;
    }

    if (distance1.minYear > distance2.minYear) {
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

static void deleteHeapEntry(HeapEntry *heapEntry) {
    free(heapEntry);
}

static int compareHeapEntry(HeapEntry *heapEntry1, HeapEntry *heapEntry2) {
    return compareDistance(heapEntry1->distance, heapEntry2->distance);
}

static bool pushNeighbours(HeapEntry *entry, Heap *heap, bool *isVisited, bool *isRestricted) {
    HeapEntry *newEntry = NULL;
    SetIterator *setIterator = getNewSetIterator(entry->city->roads);

    FAIL_IF(setIterator == NULL);

    for(Road *road = getNextSetIterator(setIterator); road != NULL; road = getNextSetIterator(setIterator)) {
        City *neighbour = getNeighbour(road, entry->city);

        if (neighbour == NULL) {
            continue;
        }

        if (isRestricted[neighbour->id] || isVisited[neighbour->id]) {
            continue;
        }

        newEntry = getNewHeapEntry(neighbour, addRoadToDistance(entry->distance, road));

        FAIL_IF(newEntry == NULL);

        FAIL_IF(!pushHeap(heap, newEntry));
    }

    deleteSetIterator(setIterator);

    return true;

    failure:;
    deleteSetIterator(setIterator);
    deleteHeapEntry(newEntry);

    return false;

}

static Distance *calculateDistances(Map *map, City *source, City *destination, bool *isRestricted) {
    Heap *heap = NULL;
    HeapEntry *heapEntry = NULL;
    bool *isVisited = NULL;
    Distance *distances = NULL;

    FAIL_IF(source == NULL || destination == NULL);

    heap = initializeHeap((int (*)(void *, void *))compareHeapEntry);
    FAIL_IF(heap == NULL);

    heapEntry = getNewHeapEntry(source, BASE_DISTANCE);
    FAIL_IF(heapEntry == NULL);
    FAIL_IF(!pushHeap(heap, heapEntry));
    heapEntry = NULL;

    size_t cityCount = getId(map->cities);
    isVisited = calloc(cityCount, sizeof(bool));
    distances = malloc(cityCount * sizeof(Distance));
    FAIL_IF(isVisited == NULL || distances == NULL);

    for (size_t i = 0; i < cityCount; i++) {
        distances[i] = WORST_DISTANCE;
    }

    while (!isEmptyHeap(heap)) {
        heapEntry = popHeap(heap);

        if (isVisited[heapEntry->city->id]) {
            continue;
        }

        isVisited[heapEntry->city->id] = true;
        distances[heapEntry->city->id] = heapEntry->distance;

        if (compareCities(heapEntry->city, destination) == 0) {
            deleteHeapEntry(heapEntry);

            break;
        }

        FAIL_IF(!pushNeighbours(heapEntry, heap, isVisited, isRestricted));

        deleteHeapEntry(heapEntry);
    }

    deleteHeap(heap, (void(*)(void *))deleteHeapEntry);
    free(isVisited);

    return distances;

    failure:;
    deleteHeap(heap, (void(*)(void *))deleteHeapEntry);
    deleteHeapEntry(heapEntry);
    free(isVisited);
    free(distances);

    return NULL;

}

static List *reconstructRoute(Map *map, City *source, City *destination, Distance *distances) {
    List *path = initializeList();

    if (path == NULL) {
        free(distances);

        return NULL;
    }

    City *position = destination;
    City *potentialNextPosition;
    Distance potentialNewDistance;
    Distance currentDistance = BASE_DISTANCE;

    addToList(path, getNewPathNode(position, NULL));

    while (compareCities(position, source) != 0) {
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

            Distance distanceThroughNeighbour = addDistance(currentDistance, addRoadToDistance(distances[neighbour->id], road));

            if (compareDistance(distances[destination->id], distanceThroughNeighbour) == 0) {
                if (potentialNextPosition != NULL) {
                    deleteSetIterator(setIterator);

                    return path;
                }

                potentialNextPosition = neighbour;
                potentialNewDistance = addRoadToDistance(currentDistance, road);

                addToList(path, getNewPathNode(neighbour, road));
            }

        }

        position = potentialNextPosition;
        currentDistance = potentialNewDistance;

        deleteSetIterator(setIterator);
    }

    return path;
}

DijkstraReturnValue *getNewDijkstraReturnValue() {
    DijkstraReturnValue *newDijkstraReturnValue = malloc(sizeof(DijkstraReturnValue));

    if (newDijkstraReturnValue == NULL) {
        return NULL;
    }

    newDijkstraReturnValue->path = NULL;
    newDijkstraReturnValue->isUnique = true;

    return newDijkstraReturnValue;
}

void deleteDijkstraReturnValue(DijkstraReturnValue *dijkstraReturnValue, bool deletePath) {
    if (dijkstraReturnValue == NULL) {
        return;
    }

    if (deletePath) {
        deleteList(dijkstraReturnValue->path, (void (*)(void *))deletePathNode);
    }

    free(dijkstraReturnValue);
}

bool isCorrectDijkstraReturnValue(DijkstraReturnValue *dijkstraReturnValue) {
    if (dijkstraReturnValue == NULL) {
        return false;
    }

    if (dijkstraReturnValue->path == NULL) {
        return false;
    }

    if (dijkstraReturnValue->minimalYear == 0) {
        return false;
    }

    return dijkstraReturnValue->isUnique;
}

Route *dijkstraReturnValueToRoute(DijkstraReturnValue *dijkstraReturnValue) {
    Route *newRoute = getNewRoute();

    if (newRoute == NULL || dijkstraReturnValue == NULL) {
        return NULL;
    }

    newRoute->path = dijkstraReturnValue->path;
    newRoute->destination = dijkstraReturnValue->destination;
    newRoute->source = dijkstraReturnValue->source;

    return newRoute;
}

int compareDijkstraReturnValues(DijkstraReturnValue *a, DijkstraReturnValue *b) {
    if (!isCorrectDijkstraReturnValue(a) && !isCorrectDijkstraReturnValue(b)) {
        return 0;
    }

    if (!isCorrectDijkstraReturnValue(a)) {
        return -1;
    }

    if (!isCorrectDijkstraReturnValue(b)) {
        return 1;
    }

    if (a->length > b->length) {
        return -1;
    }

    if (b->length > a->length) {
        return 1;
    }

    if (a->minimalYear > b->minimalYear) {
        return 1;
    }

    if (b->minimalYear > a->minimalYear) {
        return -1;
    }

    return 0;
}

DijkstraReturnValue *dijkstra(Map *map, City *source, City *destination, List *restrictedPaths) {
    ListIterator *listIterator = getNewListIterator(restrictedPaths);

    if (listIterator == NULL && restrictedPaths != NULL) {
        return NULL;
    }

    size_t cityCount = getId(map->cities);
    bool *isRestricted = calloc(cityCount, sizeof(bool));

    while (true) {
        PathNode *path = getNextListIterator(listIterator);

        if (path == NULL) {
            break;
        }

        if (compareCities(path->city, source) == 0 || compareCities(path->city, destination) == 0) {
            continue;
        }

        isRestricted[path->city->id] = true;
    }

    Distance *distances = calculateDistances(map, source, destination, isRestricted);
    DijkstraReturnValue *route = getNewDijkstraReturnValue();

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
    PathNode *endOfList = getLast(list);

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
