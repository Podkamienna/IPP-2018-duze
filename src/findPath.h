//
// Created by alicja on 29.03.19.
//

#ifndef DROGI_FINDPATH_H
#define DROGI_FINDPATH_H

#include "map.h"
#include "definitions.h"
#include "list.h"

// TODO przeniesc do findPath,c
typedef struct Distance Distance;
typedef struct HeapEntry HeapEntry;

const Distance BASE_DISTANCE;
const Distance WORST_DISTANCE;

struct Distance { // TODO udostępnić
    uint64_t length;
    int minYear;
};

struct FindPathResult {
    List *path;
    City *source, *destination;
    Distance distance;
    bool isUnique;
};

// TODO przeniesc do findPath,c kinec


typedef struct FindPathResult FindPathResult;

FindPathResult *getNewFindPathResult();

void deleteFindPathResult(FindPathResult *findPathResult, bool deletePath);

bool isCorrectPathResult(FindPathResult *findPathResult);

Route *findPathResultToRoute(FindPathResult *findPathResult);

int compareFindPathResults(FindPathResult *findPathResult1, FindPathResult *findPathResult2);

/**
 * @brief Zwraca ścieżkę
 * @param map
 * @param source
 * @param destination
 * @param restrictedPaths
 * @return Wartość @p NULL jeżeli nie udało się zaalokować pamięci,
 * strukturę z polem path ustawionym na NULL, jeżeli nie udało się jednoznacznie
 * wyznaczyć ścieżki, z polem isUnique ustawionym na false jeżeli ścieżka jest niejednoznaczna,
 * a z polem isUnique ustawionym na true jeżeli nie udało się znaleźć żadnej.
 */
FindPathResult *findPath(Map *map, City *source, City *destination, List *restrictedPaths);

#endif //DROGI_FINDPATH_H
