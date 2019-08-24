//
// Created by alicja on 29.03.19.
//

#ifndef DROGI_FIND_PATH_H
#define DROGI_FIND_PATH_H

#include "definitions.h"
#include "list.h"

#include <stdbool.h>

typedef struct Distance Distance;
typedef struct HeapEntry HeapEntry;

extern const Distance BASE_DISTANCE;
extern const Distance WORST_DISTANCE;

typedef struct FindPathResult FindPathResult;

FindPathResult *getNewFindPathResult();

void deleteFindPathResult(FindPathResult *findPathResult);

bool isCorrectPathResult(FindPathResult *findPathResult);

Route *findPathResultToRoute(FindPathResult *findPathResult);

List *findPathResultToPath(FindPathResult *findPathResult);

/**
 * Nie umie NULLi!!!!
 * @param findPathResult1
 * @param findPathResult2
 * @return
 */
int compareFindPathResults(FindPathResult *findPathResult1, FindPathResult *findPathResult2);

/**
 * @brief Zwraca ścieżkę
 * @param map
 * @param source
 * @param destination
 * @param restrictedPath
 * @return Wartość @p NULL jeżeli nie udało się zaalokować pamięci,
 * strukturę z polem path ustawionym na NULL, jeżeli nie udało się jednoznacznie
 * wyznaczyć ścieżki, z polem isUnique ustawionym na false jeżeli ścieżka jest niejednoznaczna,
 * a z polem isUnique ustawionym na true jeżeli nie udało się znaleźć żadnej.
 */
FindPathResult *findPath(Map *map, City *source, City *destination, List *restrictedPath);

#endif //DROGI_FIND_PATH_H
