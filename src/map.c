#include "definitions.h"
#include "dictionary.h"
#include "route.h"
#include "citiesAndRoads.h"
#include "list.h"
#include "findPath.h"
#include "string.h"
#include "vector.h"
#include "set.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**
 * Struktura przechowująca mapę dróg krajowych.
 */
typedef struct Map Map;

Map *newMap() {
    Map *newMap = calloc(1, sizeof(Map));
    FAIL_IF(newMap == NULL);

    newMap->cities = initializeDictionary();
    FAIL_IF(newMap->cities == NULL);

    newMap->roads = initializeVector();
    FAIL_IF(newMap->roads == NULL);

    return newMap;

    failure:;
    deleteMap(newMap);

    return NULL;
}

void deleteMap(Map *map) {
    if (map == NULL) {
        return;
    }

    if (map->routes != NULL) {
        for (int i = 0; i < 1000; i++) {
            deleteRoute(map->routes[i], true);
        }
    }

    deleteDictionary(map->cities, (void (*)(void *)) deleteCity);
    deleteVector(map->roads, (void (*)(void *)) deleteRoad);

    free(map);
}

bool addRoad(Map *map, const char *city1, const char *city2,
             unsigned length, int builtYear) {
    FAIL_IF_LABELED(map == NULL, 4);
    FAIL_IF_LABELED(!isCityName(city1) || !isCityName(city2) || strcmp(city1, city2) == 0, 4);
    FAIL_IF_LABELED(length == 0 || builtYear == 0, 4);

    City *newCity1 = searchCity(map, city1);
    City *newCity2 = searchCity(map, city2);

    if (newCity1 == NULL) {
        newCity1 = getNewCity(map, city1);
        FAIL_IF_LABELED(newCity1 == NULL, 4);
    }

    if (newCity2 == NULL) {
        newCity2 = getNewCity(map, city2);
        FAIL_IF_LABELED(newCity2 == NULL, 4);
    }

    FAIL_IF_LABELED(searchRoad(map, newCity1, newCity2), 4);

    Road *newRoad = getNewRoad(builtYear, length, newCity1, newCity2);
    FAIL_IF_LABELED(newRoad == NULL, 4);

    FAIL_IF_LABELED(!pushToVector(map->roads, newRoad), 3);
    FAIL_IF_LABELED(!insertSet(newCity2->roads, newRoad), 2);
    FAIL_IF_LABELED(!insertSet(newCity1->roads, newRoad), 1);

    return true;

    failure1:;
    deleteFromSet(newCity2->roads, NULL, newRoad);

    failure2:;
    popFromVector(map->roads, NULL);

    failure3:;
    deleteRoad(newRoad);

    failure4:;
    return false;
}

bool repairRoad(Map *map, const char *city1, const char *city2, int repairYear) {
    FAIL_IF(map == NULL);
    FAIL_IF(!isCityName(city1) || !isCityName(city2));
    FAIL_IF(repairYear == 0);

    City *tmpCity1 = searchCity(map, city1), *tmpCity2 = searchCity(map, city2);
    FAIL_IF(tmpCity1 == NULL || tmpCity2 == NULL);

    Road *tmpRoad = searchRoad(map, tmpCity1, tmpCity2);
    FAIL_IF(tmpRoad == NULL);

    FAIL_IF(tmpRoad->year > repairYear);

    tmpRoad->year = repairYear;

    return true;

    failure:;
    return false;
}

bool newRoute(Map *map, unsigned routeId,
              const char *city1, const char *city2) {
    FindPathResult *findPathResult = NULL;

    FAIL_IF(map == NULL);
    FAIL_IF(city1 == NULL || city2 == NULL);
    FAIL_IF(!isCityName(city1) || !isCityName(city2) || strcmp(city1, city2) == 0);
    FAIL_IF(routeId < MINIMAL_ROUTE_ID || routeId > MAXIMAL_ROUTE_ID);
    FAIL_IF(map->routes[routeId] != NULL);

    findPathResult = findPath(map, searchDictionary(map->cities, city1), searchDictionary(map->cities, city2), NULL);

    FAIL_IF(!isCorrectPathResult(findPathResult)); //sprawdzam, czy udalo sie wyznaczyc droge i czy jednznacznie

    Route *newRoute = findPathResultToRoute(findPathResult);
    FAIL_IF(newRoute == NULL);
    findPathResult->path = NULL;

    map->routes[routeId] = newRoute;

    deleteFindPathResult(findPathResult);

    return true;

    failure:;
    deleteFindPathResult(findPathResult);

    return false;
}

bool extendRoute(Map *map, unsigned routeId, const char *cityName) {
    FindPathResult *findPathResult1 = NULL;
    FindPathResult *findPathResult2 = NULL;
    PathNode *path = NULL;

    FAIL_IF(map == NULL);
    FAIL_IF(cityName == NULL);
    FAIL_IF(!isCityName(cityName));
    FAIL_IF(routeId < MINIMAL_ROUTE_ID || routeId > MAXIMAL_ROUTE_ID);
    FAIL_IF(map->routes[routeId] == NULL);

    City *city = searchCity(map, cityName);
    FAIL_IF(city == NULL);

    path = getNewPathNode(city, NULL);
    FAIL_IF(path == NULL);

    FAIL_IF(searchList(map->routes[routeId]->path, path, (int (*)(void *, void *)) comparePathNodes));

    deletePathNode(path);
    path = NULL;

    findPathResult1 = findPath(map, city, map->routes[routeId]->source, map->routes[routeId]->path);
    FAIL_IF(findPathResult1 == NULL);

    findPathResult2 = findPath(map, map->routes[routeId]->destination, city, map->routes[routeId]->path);
    FAIL_IF(findPathResult2 == NULL);

    int compareResult = compareFindPathResults(findPathResult1, findPathResult2);

    FAIL_IF(compareResult == 0);

    if (compareResult == 1) {
        deleteFindPathResult(findPathResult2);
        findPathResult2 = NULL;

        FAIL_IF(!isCorrectPathResult(findPathResult1));

        insertAtTheBeginningList(map->routes[routeId]->path, findPathResult1->path, (void (*)(void *)) deletePathNode,
                                 (int (*)(void *, void *)) comparePathNodes);
        findPathResult1->path = NULL;
        map->routes[routeId]->source = findPathResult1->source;

        deleteFindPathResult(findPathResult1);
    }

    if (compareResult == -1) {
        deleteFindPathResult(findPathResult1);
        findPathResult1 = NULL;

        FAIL_IF(!isCorrectPathResult(findPathResult2));

        insertAtTheEndList(map->routes[routeId]->path, findPathResult2->path, (void (*)(void *)) deletePathNode,
                           (int (*)(void *, void *)) comparePathNodes);
        findPathResult2->path = NULL;
        map->routes[routeId]->destination = findPathResult2->destination;

        deleteFindPathResult(findPathResult2);
    }

    return true;

    failure:;
    deletePathNode(path);
    deleteFindPathResult(findPathResult1);
    deleteFindPathResult(findPathResult2);
    return false;
}

/** @brief Usuwa odcinek drogi między dwoma różnymi miastami.
 * Usuwa odcinek drogi między dwoma miastami. Jeśli usunięcie tego odcinka drogi
 * powoduje przerwanie ciągu jakiejś drogi krajowej, to uzupełnia ją
 * istniejącymi odcinkami dróg w taki sposób, aby była najkrótsza. Jeśli jest
 * więcej niż jeden sposób takiego uzupełnienia, to dla każdego wariantu
 * wyznacza wśród dodawanych odcinków drogi ten, który był najdawniej wybudowany
 * lub remontowany i wybiera wariant z odcinkiem, który jest najmłodszy.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta.
 * @return Wartość @p true, jeśli odcinek drogi został usunięty.
 * Wartość @p false, jeśli z powodu błędu nie można usunąć tego odcinka drogi:
 * któryś z parametrów ma niepoprawną wartość, nie ma któregoś z podanych miast,
 * nie istnieje droga między podanymi miastami, nie da się jednoznacznie
 * uzupełnić przerwanego ciągu drogi krajowej lub nie udało się zaalokować
 * pamięci.
 */
bool removeRoad(Map *map, const char *city1, const char *city2) {
    City *tmpCity1 = searchCity(map, city1);
    City *tmpCity2 = searchCity(map, city2);
    List **routesToInsert = NULL;
    PathNode *pathNode = NULL;

    FAIL_IF(tmpCity1 == NULL || tmpCity2 == NULL);

    Road *toRemove = searchRoad(map, tmpCity1, tmpCity2);
    FAIL_IF(toRemove == NULL);

    blockRoad(toRemove);

    pathNode = getNewPathNode(NULL, toRemove);
    FAIL_IF(pathNode == NULL);

    routesToInsert = calloc(ROUTE_COUNT, sizeof(List));
    FAIL_IF(routesToInsert == NULL);

    for (unsigned i = MINIMAL_ROUTE_ID; i <= MAXIMAL_ROUTE_ID; i++) {
        if (map->routes[i] == NULL) {
            continue;
        }

        List *restrictedPath = map->routes[i]->path;
        PathNode *searchResult = searchList(restrictedPath, pathNode, (int (*)(void *, void *)) comparePathNodes);
        FindPathResult *findPathResult;

        if (searchResult != NULL) {
            if (compareCities(tmpCity1, searchResult->city) == 0) {
                findPathResult = findPath(map, tmpCity1, tmpCity2, restrictedPath);
            } else {
                findPathResult = findPath(map, tmpCity2, tmpCity1, restrictedPath);
            }

            if (!isCorrectPathResult(findPathResult)) {
                if (findPathResult != NULL) {
                    deleteFindPathResult(findPathResult);
                }

                FAIL;
            }

            routesToInsert[i] = findPathResult->path;
            findPathResult->path = NULL;
            deleteFindPathResult(findPathResult);
        }
    }

    for (unsigned i = MINIMAL_ROUTE_ID; i <= MAXIMAL_ROUTE_ID; i++) {
        if (map->routes[i] != NULL) {
            insertToList(map->routes[i]->path, routesToInsert[i], (void (*)(void *)) deletePathNode,
                         (int (*)(void *, void *)) comparePathNodes2);
        }
    }

    deleteRoadFromMap(map, tmpCity1, tmpCity2);

    deletePathNode(pathNode);
    free(routesToInsert);

    return true;

    failure:;
    if (routesToInsert != NULL) {
        for (unsigned i = MINIMAL_ROUTE_ID; i <= MAXIMAL_ROUTE_ID; i++) {
            deleteList(routesToInsert[i], (void (*)(void *)) deletePathNode);
        }
    }

    free(routesToInsert);

    deletePathNode(pathNode);
    unblockRoad(toRemove);
    return false;
}

char const *getRouteDescription(Map *map, unsigned routeId) {
    if (map == NULL) {
        return NULL;
    }

    if (routeId < MINIMAL_ROUTE_ID || routeId > MAXIMAL_ROUTE_ID) {
        return NULL;
    }

    if (map->routes[routeId] == NULL) {
        return calloc(1, sizeof(char));
    }

    String *string = initializeString();
    char *length = NULL, *year = NULL;

    ListNode *position = map->routes[routeId]->path->beginning;
    PathNode *pathNode = position->data;
    char *routeIdString = unsignedToString(routeId);

    FAIL_IF(!concatenateString(string, routeIdString));
    FAIL_IF(!concatenateString(string, SEMICOLON));

    while (position != NULL) {
        year = NULL;
        length = NULL;

        pathNode = position->data;

        FAIL_IF(!concatenateString(string, pathNode->city->name));

        if (pathNode->road != NULL) {
            FAIL_IF(!concatenateString(string, SEMICOLON));

            year = intToString(pathNode->road->year);
            FAIL_IF(year == NULL);

            length = intToString(pathNode->road->length);
            FAIL_IF(length == NULL);

            FAIL_IF(!concatenateString(string, length));
            FAIL_IF(!concatenateString(string, SEMICOLON));
            FAIL_IF(!concatenateString(string, year));
            FAIL_IF(!concatenateString(string, SEMICOLON));

            free(length);
            free(year);
        }
        position = position->next;
    }

    char *returnValue = getData(string);
    deleteString(string, false);
    free(length);
    free(year);
    free(routeIdString);

    return returnValue;

    failure:;
    deleteString(string, true);
    free(length);
    free(year);
    free(routeIdString);

    return NULL;
}


