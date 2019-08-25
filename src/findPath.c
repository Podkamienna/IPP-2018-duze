//
// Created by alicja on 13.07.19.
//
#include "findPath.h"
#include "citiesAndRoads.h"
#include "definitions.h"
#include "heap.h"
#include "list.h"
#include "set.h"
#include "route.h"
#include "dictionary.h"

#include <stdbool.h>
#include <stdlib.h>
#include <inttypes.h>
#include <limits.h>

struct Distance {
    uint64_t length;
    int minYear;
};

struct FindPathResult {
    List *path;
    City *source, *destination;
    Distance distance;
    bool isUnique;
};

struct HeapEntry {
    City *city;
    Distance distance;
};

const Distance BASE_DISTANCE = (Distance) {0, INT_MAX};
const Distance WORST_DISTANCE = (Distance) {UINT64_MAX, INT_MIN};

/**
 * @brief Zwraca zmienną typu dystans, zawierającą informacje
 * o zadanej długości i roku minimalnym.
 * @param length — długość
 * @param minYear — minimalny rok
 * @return Utworzona zmienna
 */
static Distance getDistance(uint64_t length, int minYear);

/**
 * @brief Porównuje dystanse (po współrzędnych).
 * @param distance1 — pierwszy dystans do porównania
 * @param distance2 — drugi dystans do porównania
 * @return Wartość @p 1, jeżeli pierwszy dystans jest większy.
 * Wartość @p 0, jeżeli dystanse są równe.
 * Wartość @p -1 jeżeli drugi dystans jest mniejszy.
 */
static int compareDistances(Distance distance1, Distance distance2);

/**
 * @brief Dodaje do siebie dwa dystanse.
 * @param distance1 — pierwszy dystans do dodania
 * @param distance2 — drugi dystans do dodania
 * @return Wynik dodawania dystansów
 */
static Distance addDistances(Distance distance1, Distance distance2);

/**
 * @brief Dodaje drogę do dystansu.
 * @param distance — dystans do dodania
 * @param road — droga do dodania (nie równa NULL)
 * @return Wynik dodawania
 */
static Distance addRoadToDistance(Distance distance, Road *road);

/**
 * @brief Alokuje pamięć pod i zwraca nowe pole,
 * które jest wkładane na kopiec
 * przy wykonywaniu wariantu algorytmu Dijkstry
 * @param city — miasto od którego zależy zwracane pole (nie równe NULL)
 * @param distance — dystans od którego zależy zwracane pole (nie równe NULL)
 * @return Wskaźnik na nowo utworzoną strukturę lub NULL, jeżeli
 * nie udało się zaalokować pamięci
 */
static HeapEntry *getNewHeapEntry(City *city, Distance distance);

/**
 * @brief Usuwa pole wkładane na kopiec.
 * @param heapEntry — pole do usunięcia
 */
static void deleteHeapEntry(HeapEntry *heapEntry);

/**
 * @brief Porównuje 2 pola wkładane na kopiec.
 * @param heapEntry1 — pierwsze pole do porównania (nie NULL)
 * @param heapEntry2 — drugie pole do porównania (nie NULL)
 * @return Wartość @p 1 jeżeli pierwsze pole jest większe,
 * wartość @p 0 jeżeli pola są równe, wartość @p -1, jeżeli
 * drugie pole jest większe.
 */
static int compareHeapEntries(HeapEntry *heapEntry1, HeapEntry *heapEntry2);

/**
 * @brief Wkłada odwiedzalnych (nieodwiedzonych i takich do których wejście nie jest zabronione)
 * sąsiadów zadanego przez pole wkładane na kopiec miasta na zadany kopiec.
 * @param heapEntry — pole zadające miasto
 * @param heap — kopiec na który będą wkładani sąsiedzi
 * @param isVisited — tablica pozwalająca na sprawdzenie, czy miasto było odwiedzone
 * @param isRestricted — tablica pozwalająca na sprawdzanie, czy można wejść do danego miasta
 * @return Wartość @p false, jeżeli nie udało się zaalokować pamięci, wartość @p true, jeżeli wszystko
 * się powiodło.
 */
static bool pushNeighbours(HeapEntry *heapEntry, Heap *heap, const bool *isVisited, const bool *isRestricted);

/**
 * @brief Przy użyciu algorytmu Dijkstry oblicza odległość od źródła do celu, zapisuje uzyskane
 * przy tej okazji dystanse do poszczególnych miast i zwraca tablicę tak uzyskanych wyników.
 * @param map — mapa w której zawarte są miasta i drogi
 * @param source — miasto z którego szukana jest ścieżka
 * @param destination — miasto do którego szukana jest ścieżka
 * @param isRestricted — tablica pozwalająca na sprawdzenie, czy można wejść do danego miasta
 * @return Wartość @p NULL, jeżeli nie uda się zaalokować pamięci, obliczoną tablicę odległości
 * w przeciwnym razie. Jeżeli ścieżka nie istnieje, to na polu odpowiadającemu celowi będzie dystans
 * równy stałej WORST_DISTANCE.
 */
static Distance *calculateDistances(Map *map, City *source, City *destination, bool *isRestricted);

/**
 * @brief Na podstawie tablicy obliczonej przez funkcję calculateDistances odtwarza najkrótszą
 * ścieżkę. Zakłada, że jakaś ścieżka istnieje.
 * @param source — miasto z którego szukana jest ścieżka
 * @param destination — miasto do którego szukana jest ścieżka
 * @param distances — obliczona tablica dystansów
 * @return Wartość @p NULL, jeżeli nie udało się zaalokować pamięci, obliczoną ścieżkę w innym razie.
 * Jeżeli ścieżka nie jest wyznaczona jednoznacznie, zwraca sufiks ścieżki, który jest wyznaczony jednoznacznie.
 */
static FindPathResult *reconstructPath(City *source, City *destination, Distance *distances);

static Distance getDistance(uint64_t length, int minYear) {
    return (Distance) {length, minYear};
}

static int compareDistances(Distance distance1, Distance distance2) {
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

static Distance addDistances(Distance distance1, Distance distance2) {
    Distance result = distance1;

    result.length += distance2.length;

    // Jeżeli suma byłaby większa niż maksymalna wartość przechowywana przez typ, to zwraca możliwie najbliższy jej dystans.
    if (result.length < distance1.length || result.length < distance2.length) {
        return WORST_DISTANCE;
    }

    if (result.minYear > distance2.minYear) {
        result.minYear = distance2.minYear;
    }

    return result;
}

static Distance addRoadToDistance(Distance distance, Road *road) {
    return addDistances(distance, getDistance(road->length, road->year));
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

static int compareHeapEntries(HeapEntry *heapEntry1, HeapEntry *heapEntry2) {
    return compareDistances(heapEntry1->distance, heapEntry2->distance);
}

static bool pushNeighbours(HeapEntry *heapEntry, Heap *heap, const bool *isVisited, const bool *isRestricted) {
    HeapEntry *newEntry = NULL;
    SetIterator *setIterator = getNewSetIterator(heapEntry->city->roads);

    FAIL_IF(setIterator == NULL);

    for (Road *road = getNextSetIterator(setIterator); road != NULL; road = getNextSetIterator(setIterator)) {
        City *neighbour = getNeighbour(road, heapEntry->city);

        if (neighbour == NULL) {
            continue;
        }

        if (isRestricted[neighbour->id] || isVisited[neighbour->id]) {
            continue;
        }

        newEntry = getNewHeapEntry(neighbour, addRoadToDistance(heapEntry->distance, road));
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

    heap = initializeHeap((int (*)(void *, void *)) compareHeapEntries);
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
            deleteHeapEntry(heapEntry);
            continue;
        }

        isVisited[heapEntry->city->id] = true;
        distances[heapEntry->city->id] = heapEntry->distance;

        if (areEqualCities(heapEntry->city, destination)) {
            deleteHeapEntry(heapEntry);

            break;
        }

        FAIL_IF(!pushNeighbours(heapEntry, heap, isVisited, isRestricted));

        deleteHeapEntry(heapEntry);
    }

    deleteHeap(heap, (void (*)(void *)) deleteHeapEntry);
    free(isVisited);

    return distances;

    failure:;
    deleteHeap(heap, (void (*)(void *)) deleteHeapEntry);
    deleteHeapEntry(heapEntry);
    free(isVisited);
    free(distances);

    return NULL;

}

static FindPathResult *reconstructPath(City *source, City *destination, Distance *distances) {
    FindPathResult *result = getNewFindPathResult();
    List *path = NULL;
    PathNode *pathNode = NULL;

    FAIL_IF(result == NULL);

    result->source = source;
    result->destination = destination;
    result->distance = distances[destination->id];
    if (compareDistances(result->distance, WORST_DISTANCE) == 0) {
        return result;
    }

    path = initializeList();
    FAIL_IF(path == NULL);

    // Wrzucanie celu na ścieżkę.
    pathNode = getNewPathNode(destination, NULL);
    FAIL_IF(pathNode == NULL);

    FAIL_IF(!addToList(path, pathNode));
    pathNode = NULL;

    City *position = destination;
    City *potentialNextPosition;
    Distance potentialNewDistance = WORST_DISTANCE;
    Distance currentDistance = BASE_DISTANCE;

    while (!areEqualCities(position, source)) {
        potentialNextPosition = NULL;

        SetIterator *setIterator = getNewSetIterator(position->roads);
        FAIL_IF(setIterator == NULL);

        for (Road *road = getNextSetIterator(setIterator); road != NULL; road = getNextSetIterator(setIterator)) {
            City *neighbour = getNeighbour(road, position);

            if (neighbour == NULL) {
                continue;
            }

            Distance distanceThroughNeighbour = addDistances(currentDistance,
                                                             addRoadToDistance(distances[neighbour->id], road));

            if (compareDistances(distances[destination->id], distanceThroughNeighbour) == 0) {
                if (potentialNextPosition != NULL) {
                    deleteSetIterator(setIterator);
                    deleteList(path, (void (*)(void *)) deletePathNode);

                    result->isUnique = false;
                    return result;
                }

                potentialNextPosition = neighbour;
                potentialNewDistance = addRoadToDistance(currentDistance, road);

                pathNode = getNewPathNode(neighbour, road);

                if(!addToList(path, pathNode)) {
                    deleteSetIterator(setIterator);

                    FAIL;
                }
                pathNode = NULL;
            }

        }

        position = potentialNextPosition;
        currentDistance = potentialNewDistance;

        deleteSetIterator(setIterator);
    }


    result->path = path;

    return result;

    failure:;
    deletePathNode(pathNode);
    deleteFindPathResult(result);
    deleteList(path, (void (*)(void *)) deletePathNode);

    return NULL;
}

FindPathResult *getNewFindPathResult() {
    FindPathResult *newFindPathResult = malloc(sizeof(FindPathResult));

    if (newFindPathResult == NULL) {
        return NULL;
    }

    newFindPathResult->path = NULL;
    newFindPathResult->isUnique = true;

    return newFindPathResult;
}

void deleteFindPathResult(FindPathResult *findPathResult) {
    if (findPathResult == NULL) {
        return;
    }

    deleteList(findPathResult->path, (void (*)(void *)) deletePathNode);

    free(findPathResult);
}

bool isCorrectPathResult(FindPathResult *findPathResult) {
    if (findPathResult == NULL) {
        return false;
    }

    if (findPathResult->path == NULL) {
        return false;
    }

    return findPathResult->isUnique;
}

Route *findPathResultToRoute(FindPathResult *findPathResult) {
    if (findPathResult == NULL) {
        return NULL;
    }

    Route *newRoute = getNewRoute(findPathResult->path, findPathResult->source, findPathResult->destination);

    if (newRoute == NULL) {
        return NULL;
    }

    findPathResult->path = NULL;
    deleteFindPathResult(findPathResult);
    return newRoute;
}


List *findPathResultToPath(FindPathResult *findPathResult) {
    if (findPathResult == NULL) {
        return NULL;
    }

    List *path = findPathResult->path;

    findPathResult->path = NULL;
    deleteFindPathResult(findPathResult);

    return path;
}

int compareFindPathResults(FindPathResult *findPathResult1, FindPathResult *findPathResult2) {
    if (!isCorrectPathResult(findPathResult1) && !isCorrectPathResult(findPathResult2)) {
        return 0;
    }

    if (!isCorrectPathResult(findPathResult1)) {
        if (compareDistances(findPathResult1->distance, findPathResult2->distance) <= 0) {
            return 0;
        }

        return 1;
    }

    if (!isCorrectPathResult(findPathResult2)) {
        if (compareDistances(findPathResult1->distance, findPathResult2->distance) >= 0) {
            return 0;
        }

        return -1;
    }

    return compareDistances(findPathResult1->distance, findPathResult2->distance);
}

FindPathResult *findPath(Map *map, City *source, City *destination, List *restrictedPath) {
    ListIterator *listIterator = NULL;
    Distance *distances = NULL;
    bool *isRestricted = NULL;

    FAIL_IF(source == NULL || destination == NULL);

    listIterator = getNewListIterator(restrictedPath);
    FAIL_IF(listIterator == NULL && restrictedPath != NULL);

    size_t cityCount = getId(map->cities);

    isRestricted = calloc(cityCount, sizeof(bool));
    FAIL_IF(isRestricted == NULL);

    for (PathNode *path = getNextListIterator(listIterator); path != NULL; path = getNextListIterator(listIterator)) {
        isRestricted[path->city->id] = true;
    }

    isRestricted[source->id] = false;
    isRestricted[destination->id] = false;

    distances = calculateDistances(map, source, destination, isRestricted);
    FAIL_IF(distances == NULL);

    FindPathResult *result = reconstructPath(source, destination, distances);
    FAIL_IF(result == NULL);

    free(distances);
    free(isRestricted);
    deleteListIterator(listIterator);

    return result;

    failure:;
    free(distances);
    free(isRestricted);
    deleteListIterator(listIterator);

    return NULL;
}
