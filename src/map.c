#include "definitions.h"
#include "dictionary.h"
#include "route.h"
#include "citiesAndRoads.h"
#include "list.h"
#include "dijkstra.h"
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

/** @brief Tworzy nową strukturę.
 * Tworzy nową, pustą strukturę niezawierającą żadnych miast, odcinków dróg ani
 * dróg krajowych.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
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

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p map.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param[in] map        – wskaźnik na usuwaną strukturę.
 */
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
    DijkstraReturnValue *dijkstraResult = NULL;

    FAIL_IF(map == NULL);
    FAIL_IF(city1 == NULL || city2 == NULL);
    FAIL_IF(!isCityName(city1) || !isCityName(city2) || strcmp(city1, city2) == 0);
    FAIL_IF(routeId < MINIMAL_ROUTE_ID || routeId > MAXIMAL_ROUTE_ID);
    FAIL_IF(map->routes[routeId] != NULL);

    dijkstraResult = dijkstra(map, searchDictionary(map->cities, city1), searchDictionary(map->cities, city2), NULL);

    FAIL_IF(!isCorrectDijkstraReturnValue(dijkstraResult)); //sprawdzam, czy udalo sie wyznaczyc droge i czy jednznacznie

    Route *newRoute = dijkstraReturnValueToRoute(dijkstraResult);
    map->routes[routeId] = newRoute;

    deleteDijkstraReturnValue(dijkstraResult, false);

    return true;

    failure:;
    deleteDijkstraReturnValue(dijkstraResult, true);

    return false;
}

bool extendRoute(Map *map, unsigned routeId, const char *cityName) {
    DijkstraReturnValue *dijkstraResult1 = NULL;
    DijkstraReturnValue *dijkstraResult2 = NULL;

    FAIL_IF_LABELED(map == NULL, 1);
    FAIL_IF_LABELED(cityName == NULL, 1);
    FAIL_IF_LABELED(!isCityName(cityName), 1);
    FAIL_IF_LABELED(routeId < MINIMAL_ROUTE_ID || routeId > MAXIMAL_ROUTE_ID, 1);
    FAIL_IF_LABELED(map->routes[routeId] == NULL, 1);

    City *city = searchCity(map, cityName);

    if (city == NULL) {
        return false;
    }

    PathNode *path = getNewPathNode(city, NULL);

    if (path == NULL) {
        deleteCity(city);

        return false;
    }

    if (searchList(map->routes[routeId]->path, path, (int (*)(void *, void *)) comparePathNodes)) {
        deletePathNode(path);

        return false;
    }

    deletePathNode(path);

    dijkstraResult1 = dijkstra(map, city, map->routes[routeId]->source,
                                                    map->routes[routeId]->path);

    if (dijkstraResult1 == NULL) {
        return false;
    }

    dijkstraResult2 = dijkstra(map, map->routes[routeId]->destination, city, map->routes[routeId]->path);

    if (dijkstraResult2 == NULL) {
        deleteDijkstraReturnValue(dijkstraResult1, false);

        return false;
    }

    int compareResult = compareDijkstraReturnValues(dijkstraResult1, dijkstraResult2);

    if (compareResult == 0) {
        deleteDijkstraReturnValue(dijkstraResult1, true);
        deleteDijkstraReturnValue(dijkstraResult2, true);

        return false;
    }

    if (compareResult == 1) {
        deleteDijkstraReturnValue(dijkstraResult2, true);

        if (!isCorrectDijkstraReturnValue(dijkstraResult1)) {
            deleteDijkstraReturnValue(dijkstraResult1, true);

            return false;
        }

        insertAtTheBeginningList(map->routes[routeId]->path, dijkstraResult1->path, (void (*)(void *)) deletePathNode,
                                 (int (*)(void *, void *)) comparePathNodes);
        map->routes[routeId]->source = dijkstraResult1->source;

        free(dijkstraResult1->path);
        deleteDijkstraReturnValue(dijkstraResult1, false);
    }

    if (compareResult == -1) {
        deleteDijkstraReturnValue(dijkstraResult1, true);

        if (!isCorrectDijkstraReturnValue(dijkstraResult2)) {
            deleteDijkstraReturnValue(dijkstraResult2, true);

            return false;
        }

        insertAtTheEndList(map->routes[routeId]->path, dijkstraResult2->path, (void (*)(void *)) deletePathNode,
                           (int (*)(void *, void *)) comparePathNodes);
        map->routes[routeId]->destination = dijkstraResult2->destination;

        free(dijkstraResult2->path);
        deleteDijkstraReturnValue(dijkstraResult2, false);
    }

    return true;

    failure1:;
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

    if (tmpCity1 == NULL || tmpCity2 == NULL) {
        return false;
    }

    Road *toRemove = searchRoad(map, tmpCity1, tmpCity2);

    if (toRemove == NULL) {
        return false;
    }

    blockRoad(toRemove);

    PathNode *pathNode = getNewPathNode(NULL, toRemove);

    if (pathNode == NULL) {
        unblockRoad(toRemove);

        return false;
    }

    List **routesToInsert = calloc(ROUTE_COUNT, sizeof(List));

    if (routesToInsert == NULL) {
        deletePathNode(pathNode);
        unblockRoad(toRemove);

        return false;
    }

    for (unsigned i = MINIMAL_ROUTE_ID; i <= MAXIMAL_ROUTE_ID; i++) {
        if (map->routes[i] == NULL) {
            continue;
        }

        List *restrictedPaths = map->routes[i]->path;
        PathNode *searchResult = searchList(restrictedPaths, pathNode, (int (*)(void *, void *)) comparePathNodes);
        DijkstraReturnValue *dijkstraResult;

        if (searchResult != NULL) {
            if (compareCities(tmpCity1, searchResult->city) == 0) {
                dijkstraResult = dijkstra(map, tmpCity1, tmpCity2, restrictedPaths);
            }
            else {
                dijkstraResult = dijkstra(map, tmpCity2, tmpCity1, restrictedPaths);
            }

            if (!isCorrectDijkstraReturnValue(dijkstraResult)) {
                if (dijkstraResult != NULL) {
                    deleteDijkstraReturnValue(dijkstraResult, true);
                }

                for (unsigned j = i; j >= MINIMAL_ROUTE_ID; j--) {
                    deleteList(routesToInsert[j], (void(*)(void*))deletePathNode);
                }

                deletePathNode(pathNode);
                free(routesToInsert);
                unblockRoad(toRemove);

                return false;
            }

            routesToInsert[i] = dijkstraResult->path;
            //TODO ustawić reversed routes i posprawdza
        }
    }

    for (unsigned i = MINIMAL_ROUTE_ID; i <= MAXIMAL_ROUTE_ID; i++) {
        if (map->routes[i] != NULL) {
            insertToList(map->routes[i]->path, routesToInsert[i], (void (*)(void *)) deletePathNode, (int(*)(void *, void *))comparePathNodes2);
            insertToList(map->routes[i]->path, routesToInsert[i], (void (*)(void *)) deletePathNode, (int(*)(void *, void *))comparePathNodes2);
        }
    }

    deleteRoadFromMap(map, tmpCity1, tmpCity2);

    deletePathNode(pathNode);
    free(routesToInsert);

    return true;
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
    const char *length = NULL, *year = NULL;

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

            free((void *) length);
            free((void *) year);
        }
        position = position->next;
    }

    char *returnValue = getData(string);
    deleteString(string, false);
    free((void *) length);
    free((void *) year);
    free((void *) routeIdString);

    return returnValue;

    failure:;
    deleteString(string, true);
    free((void *) length);
    free((void *) year);
    free((void *) routeIdString);

    return NULL;
}


