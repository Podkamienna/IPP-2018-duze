/** @file
 * Implementacja klasy przechowującej mapę dróg krajowych
 *
 * @author Alicja Ziarko <ala.ziarko@gmail.com>
 * @date 02.07.2019
 */

#include "map.h"
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

    for (int i = 0; i < 1000; i++) {
        deleteRoute(map->routes[i]);
    }

    deleteDictionary(map->cities, (void (*)(void *)) deleteCity);
    deleteVector(map->roads, (void (*)(void *)) deleteRoad);

    free(map);
}

bool addRoad(Map *map, const char *city1, const char *city2,
             unsigned length, int builtYear) {
    Road *newRoad = NULL;

    FAIL_IF(map == NULL);
    FAIL_IF(!isCityName(city1) || !isCityName(city2) || strcmp(city1, city2) == 0);
    FAIL_IF(length == 0 || builtYear == 0);

    City *newCity1 = searchCity(map, city1);
    City *newCity2 = searchCity(map, city2);

    if (newCity1 == NULL) {
        newCity1 = getNewCity(map, city1);
        FAIL_IF(newCity1 == NULL);
    }

    if (newCity2 == NULL) {
        newCity2 = getNewCity(map, city2);
        FAIL_IF(newCity2 == NULL);
    }

    FAIL_IF(searchRoad(map, newCity1, newCity2));

    newRoad = getNewRoad(builtYear, length, newCity1, newCity2);
    FAIL_IF(newRoad == NULL);

    FAIL_IF(!pushToVector(map->roads, newRoad));

    if (!insertSet(newCity2->roads, newRoad)) {
        popFromVector(map->roads, NULL);

        FAIL;
    }

    if (!insertSet(newCity1->roads, newRoad)) {
        deleteFromSet(newCity2->roads, NULL, newRoad);
        popFromVector(map->roads, NULL);

        FAIL;
    }

    return true;

    failure:;
    deleteRoad(newRoad);
    return false;
}

bool repairRoad(Map *map, const char *cityName1, const char *cityName2, int repairYear) {
    FAIL_IF(map == NULL);
    FAIL_IF(!isCityName(cityName1) || !isCityName(cityName2));
    FAIL_IF(repairYear == 0);

    City *city1 = searchCity(map, cityName1), *city2 = searchCity(map, cityName2);
    FAIL_IF(city1 == NULL || city2 == NULL);

    Road *road = searchRoad(map, city1, city2);
    FAIL_IF(road == NULL);

    FAIL_IF(road->year > repairYear);

    road->year = repairYear;

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

    FAIL_IF(!isCorrectPathResult(findPathResult)); // Sprawdzam, czy udało sie wyznaczyć drogę i czy jednoznacznie

    Route *newRoute = findPathResultToRoute(findPathResult);
    FAIL_IF(newRoute == NULL);
    findPathResult = NULL;

    map->routes[routeId] = newRoute;

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

    FAIL_IF(searchList(map->routes[routeId]->path, path, (bool (*)(void *, void *)) areEqualPathNodes));

    deletePathNode(path);
    path = NULL;

    findPathResult1 = findPath(map, city, map->routes[routeId]->source, map->routes[routeId]->path);
    FAIL_IF(findPathResult1 == NULL);

    findPathResult2 = findPath(map, map->routes[routeId]->destination, city, map->routes[routeId]->path);
    FAIL_IF(findPathResult2 == NULL);

    int compareResult = compareFindPathResults(findPathResult1, findPathResult2);

    // Jeżeli ścieżki są równie dobre, to ścieżka nie jest jednoznaczna.
    FAIL_IF(compareResult == 0);

    // Wybieranie lepszej ścieżki
    if (compareResult < 0) {
        deleteFindPathResult(findPathResult2);
        findPathResult2 = NULL;

        FAIL_IF(!isCorrectPathResult(findPathResult1));

        insertAtTheBeginningList(map->routes[routeId]->path, findPathResultToPath(findPathResult1),
                                 (void (*)(void *)) deletePathNode);

        map->routes[routeId]->source = city;
    }

    if (compareResult > 0) {
        deleteFindPathResult(findPathResult1);
        findPathResult1 = NULL;

        FAIL_IF(!isCorrectPathResult(findPathResult2));

        insertAtTheEndList(map->routes[routeId]->path, findPathResultToPath(findPathResult2),
                           (void (*)(void *)) deletePathNode);

        map->routes[routeId]->destination = city;
    }

    return true;

    failure:;
    deletePathNode(path);
    deleteFindPathResult(findPathResult1);
    deleteFindPathResult(findPathResult2);
    return false;
}

bool removeRoad(Map *map, const char *cityName1, const char *cityName2) {
    City *city1 = searchCity(map, cityName1);
    City *city2 = searchCity(map, cityName2);
    List **routesToInsert = NULL;
    PathNode *pathNode = NULL;
    Road *toRemove = NULL;

    FAIL_IF(city1 == NULL || city2 == NULL);

    toRemove = searchRoad(map, city1, city2);
    FAIL_IF(toRemove == NULL);

    blockRoad(toRemove);

    pathNode = getNewPathNode(NULL, toRemove);
    FAIL_IF(pathNode == NULL);

    routesToInsert = calloc(ROUTE_COUNT, sizeof(List));
    FAIL_IF(routesToInsert == NULL);

    // Dla każdego możliwego numeru drogi krajowej sprawdzam, czy droga do usunięcia jest w niej zawarta
    // Jeżeli tak, to sprawdzam, czy da się da ją jednoznacznie zastąpić przez inną ścieżkę
    // Jeżeli się nie da, to odblokowuję drogę do usunięcia. Jeżeli dla każdej się da, to usuwam drogę do usunięcia.
    for (unsigned i = MINIMAL_ROUTE_ID; i <= MAXIMAL_ROUTE_ID; i++) {
        if (map->routes[i] == NULL) {
            continue;
        }

        List *restrictedPath = map->routes[i]->path;
        PathNode *searchResult = searchList(restrictedPath, pathNode, (bool (*)(void *, void *)) areEqualPathNodes);
        FindPathResult *findPathResult;

        if (searchResult != NULL) {
            if (areEqualCities(city1, searchResult->city)) {
                findPathResult = findPath(map, city1, city2, restrictedPath);
            } else {
                findPathResult = findPath(map, city2, city1, restrictedPath);
            }

            if (!isCorrectPathResult(findPathResult)) {
                if (findPathResult != NULL) {
                    deleteFindPathResult(findPathResult);
                }

                FAIL;
            }

            routesToInsert[i] = findPathResultToPath(findPathResult);
        }
    }

    for (unsigned i = MINIMAL_ROUTE_ID; i <= MAXIMAL_ROUTE_ID; i++) {
        if (map->routes[i] != NULL) {
            insertToList(map->routes[i]->path, routesToInsert[i], (void (*)(void *)) deletePathNode,
                         (bool (*)(void *, void *)) areEqualPathNodesByCities);
        }
    }

    deleteRoadFromMap(map, city1, city2);

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
    char *length = NULL, *year = NULL, *routeIdString = NULL;
    String *string = NULL;

    FAIL_IF(map == NULL);

    if (routeId < MINIMAL_ROUTE_ID || routeId > MAXIMAL_ROUTE_ID) {
        return calloc(1, sizeof(char));
    }

    if (map->routes[routeId] == NULL) {
        return calloc(1, sizeof(char));
    }

    string = initializeString();
    ListNode *position = map->routes[routeId]->path->beginning;
    PathNode *pathNode = NULL;
    routeIdString = unsignedToString(routeId);

    FAIL_IF(!concatenateString(string, routeIdString));
    FAIL_IF(!concatenateString(string, DELIMITER));

    while (position != NULL) {
        year = NULL;
        length = NULL;

        pathNode = position->data;

        FAIL_IF(!concatenateString(string, pathNode->city->name));

        if (pathNode->road != NULL) {
            FAIL_IF(!concatenateString(string, DELIMITER));

            year = intToString(pathNode->road->year);
            FAIL_IF(year == NULL);

            length = unsignedToString(pathNode->road->length);
            FAIL_IF(length == NULL);

            FAIL_IF(!concatenateString(string, length));
            FAIL_IF(!concatenateString(string, DELIMITER));
            FAIL_IF(!concatenateString(string, year));
            FAIL_IF(!concatenateString(string, DELIMITER));

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

bool newRouteSpecified(Map *map, size_t routeId, char **cityNames, unsigned *lengths, int *years, size_t roadCount) {
    Road **roads = NULL;
    City **cities = NULL;
    bool *exists = NULL;
    List *path = NULL;
    PathNode *pathNode = NULL;

    FAIL_IF(map == NULL);
    FAIL_IF(cityNames == NULL || lengths == NULL || years == NULL);

    FAIL_IF(routeId < MINIMAL_ROUTE_ID || routeId > MAXIMAL_ROUTE_ID);
    FAIL_IF(map->routes[routeId] != NULL);

    size_t cityCount = roadCount + 1;

    cities = calloc(cityCount, sizeof(City *));
    FAIL_IF(cities == NULL);

    for (size_t i = 0; i < cityCount; i++) {
        cities[i] = searchCity(map, cityNames[i]);

        if (cities[i] == NULL) {
            cities[i] = getNewCity(map, cityNames[i]);
            FAIL_IF(cities[i] == NULL);
        }
    }

    exists = calloc(getId(map->cities), sizeof(bool));
    FAIL_IF(exists == NULL);

    for (size_t i = 0; i < cityCount; i++) {
        FAIL_IF(exists[cities[i]->id]);

        exists[cities[i]->id] = true;
    }

    roads = malloc(roadCount * sizeof(Road *));
    FAIL_IF(roads == NULL);

    for (size_t i = 0; i < roadCount; i++) {
        FAIL_IF(lengths[i] == 0 || years[i] == 0);
        roads[i] = searchRoad(map, cities[i], cities[i + 1]);

        if (roads[i] == NULL) {
            continue;
        }

        FAIL_IF(roads[i]->length != lengths[i]);
        FAIL_IF(roads[i]->year > years[i]);
    }

    for (size_t i = 0; i < roadCount; i++) {
        if (roads[i] == NULL) {
            FAIL_IF(!addRoad(map, cityNames[i], cityNames[i + 1], lengths[i], years[i]));

            roads[i] = searchRoad(map, cities[i], cities[i + 1]);

            continue;
        }

        FAIL_IF(!repairRoad(map, cityNames[i], cityNames[i + 1], years[i]));
    }

    path = initializeList();
    FAIL_IF(path == NULL);

    pathNode = getNewPathNode(cities[cityCount - 1], NULL);
    FAIL_IF(pathNode == NULL);
    FAIL_IF(!addToList(path, pathNode));
    pathNode = NULL;

    for (size_t i = 1; i <= roadCount; i++) {
        pathNode = getNewPathNode(cities[roadCount - i], roads[roadCount - i]);
        FAIL_IF(pathNode == NULL);
        FAIL_IF(!addToList(path, pathNode));
        pathNode = NULL;
    }

    Route *route = malloc(sizeof(Route));
    FAIL_IF(route == NULL);

    route->path = path;
    route->source = cities[0];
    route->destination = cities[cityCount - 1];

    map->routes[routeId] = route;

    free(roads);
    free(cities);
    free(exists);

    return true;

    failure:;
    free(roads);
    free(cities);
    free(exists);
    deleteList(path, (void (*)(void *)) deletePathNode);
    deletePathNode(pathNode);

    return false;
}

bool removeRoute(Map *map, unsigned routeId) {
    FAIL_IF(map == NULL);
    FAIL_IF(routeId < MINIMAL_ROUTE_ID || routeId > MAXIMAL_ROUTE_ID);
    FAIL_IF(map->routes[routeId] == NULL);

    deleteRoute(map->routes[routeId]);
    map->routes[routeId] = NULL;

    return true;

    failure:;
    return false;
}


