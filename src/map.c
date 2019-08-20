#include "definitions.h"
#include "dictionary.h"
#include "route.h"
#include "citiesAndRoads.h"
#include "list.h"
#include "dijkstra.h"
#include "string.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**
 * Struktura przechowująca mapę dróg krajowych.
 */
typedef struct Map Map;

//TODO to gdzieś indziej. Gdzie?
static int min(int a, int b) {
    if (a > b) {
        return b;
    }

    return a;
}

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

/** @brief Dodaje do mapy odcinek drogi między dwoma różnymi miastami.
 * Jeśli któreś z podanych miast nie istnieje, to dodaje go do mapy, a następnie
 * dodaje do mapy odcinek drogi między tymi miastami.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] length     – długość w km odcinka drogi;
 * @param[in] builtYear  – rok budowy odcinka drogi.
 * @return Wartość @p true, jeśli odcinek drogi został dodany.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * wartość, obie podane nazwy miast są identyczne, odcinek drogi między tymi
 * miastami już istnieje lub nie udało się zaalokować pamięci.
 */
bool addRoad(Map *map, const char *city1, const char *city2,
             unsigned length, int builtYear) {
    if (map == NULL) {
        return false;
    }

    if (!isCityName(city1) || !isCityName(city2) || strcmp(city1, city2) == 0) {
        return false;
    }

    if (length == 0 || builtYear == 0) {
        return false;
    }

    return addNewRoad(map, city1, city2, builtYear, length);
}

/** @brief Modyfikuje rok ostatniego remontu odcinka drogi.
 * Dla odcinka drogi między dwoma miastami zmienia rok jego ostatniego remontu
 * lub ustawia ten rok, jeśli odcinek nie był jeszcze remontowany.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] repairYear – rok ostatniego remontu odcinka drogi.
 * @return Wartość @p true, jeśli modyfikacja się powiodła.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * wartość, któreś z podanych miast nie istnieje, nie ma odcinka drogi między
 * podanymi miastami, podany rok jest wcześniejszy niż zapisany dla tego odcinka
 * drogi rok budowy lub ostatniego remontu.
 */
bool repairRoad(Map *map, const char *city1, const char *city2, int repairYear) {
    if (map == NULL) {
        return false;
    }

    if (!isCityName(city1) || !isCityName(city2)) {
        return false;
    }

    if (repairYear == 0) {
        return false;
    }

    return updateYearRoad(map, city1, city2, repairYear);
}

/** @brief Łączy dwa różne miasta drogą krajową.
 * Tworzy drogę krajową pomiędzy dwoma miastami i nadaje jej podany numer.
 * Wśród istniejących odcinków dróg wyszukuje najkrótszą drogę. Jeśli jest
 * więcej niż jeden sposób takiego wyboru, to dla każdego wariantu wyznacza
 * wśród wybranych w nim odcinków dróg ten, który był najdawniej wybudowany lub
 * remontowany i wybiera wariant z odcinkiem, który jest najmłodszy.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId    – numer drogi krajowej;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta.
 * @return Wartość @p true, jeśli droga krajowa została utworzona.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * wartość, istnieje już droga krajowa o podanym numerze, któreś z podanych
 * miast nie istnieje, obie podane nazwy miast są identyczne, nie można
 * jednoznacznie wyznaczyć drogi krajowej między podanymi miastami lub nie udało
 * się zaalokować pamięci.
 */
bool newRoute(Map *map, unsigned routeId,
              const char *city1, const char *city2) {
    if (map == NULL) {
        return false;
    }

    if (city1 == NULL || city2 == NULL) {
        return false;
    }

    if (!isCityName(city1) || !isCityName(city2)) {
        return false;
    }

    if (routeId < MINIMAL_ROUTE_ID || routeId > MAXIMAL_ROUTE_ID) {
        return false;
    }

    Route *newRoute = dijkstra(map, searchDictionary(map->cities, city1), searchDictionary(map->cities, city2), NULL);

    if (!isCorrectRoute(newRoute)) { //sprawdzam, czy udalo sie wyznaczyc droge i czy jednznacznie
        deleteRoute(newRoute, true);

        return false;
    }

    map->routes[routeId] = newRoute;

    return true;
}

/** @brief Wydłuża drogę krajową do podanego miasta.
 * Dodaje do drogi krajowej nowe odcinki dróg do podanego miasta w taki sposób,
 * aby nowy fragment drogi krajowej był najkrótszy. Jeśli jest więcej niż jeden
 * sposób takiego wydłużenia, to dla każdego wariantu wyznacza wśród dodawanych
 * odcinków dróg ten, który był najdawniej wybudowany lub remontowany i wybiera
 * wariant z odcinkiem, który jest najmłodszy.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId    – numer drogi krajowej;
 * @param[in] city       – wskaźnik na napis reprezentujący nazwę miasta.
 * @return Wartość @p true, jeśli droga krajowa została wydłużona.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * nazwę, nie istnieje droga krajowa o podanym numerze, nie ma miasta o podanej
 * nazwie, przez podane miasto już przechodzi droga krajowa o podanym numerze,
 * podana droga krajowa kończy się w podanym mieście, nie można jednoznacznie
 * wyznaczyć nowego fragmentu drogi krajowej lub nie udało się zaalokować
 * pamięci.
 */
bool extendRoute(Map *map, unsigned routeId, const char *city) {
    if (map == NULL) {
        return false;
    }

    if (city == NULL) {
        return false;
    }

    if (map->routes[routeId] == NULL) {
        return false;
    }

    Route *tempRoute1 = dijkstra(map, searchDictionary(map->cities, city), map->routes[routeId]->source,
                                 map->routes[routeId]->path);

    if (tempRoute1 == NULL) {
        return false;
    }

    Route *tempRoute2 = dijkstra(map, map->routes[routeId]->destination, searchDictionary(map->cities, city),
                                 map->routes[routeId]->path);

    if (tempRoute2 == NULL) {
        deleteRoute(tempRoute1, false);

        return false;
    }

    int compareResult = compareRoute(tempRoute1, tempRoute2);

    if (compareResult == 0) {
        deleteRoute(tempRoute1,true);
        deleteRoute(tempRoute2,true);

        return false;
    }

    if (compareResult == 1) {
        deleteRoute(tempRoute2, true);

        if (!isCorrectRoute(tempRoute1)) {
            deleteRoute(tempRoute1, true);

            return false;
        }

        insertAtTheBeginning(map->routes[routeId]->path,tempRoute1->path, (void(*)(void*))deletePathNode);
        map->routes[routeId]->source = tempRoute1->source;
        map->routes[routeId]->minimalYear = min(map->routes[routeId]->minimalYear, tempRoute1->minimalYear);
        map->routes[routeId]->length += tempRoute1->length;

        free(tempRoute1->path);
        deleteRoute(tempRoute1, false);
    }

    if (compareResult == -1) {
        deleteRoute(tempRoute1,true);

        if (!isCorrectRoute(tempRoute2)) {
            deleteRoute(tempRoute2, true);

            return false;
        }

        insertAtTheEnd(map->routes[routeId]->path,tempRoute2->path, (void(*)(void*))deletePathNode);
        map->routes[routeId]->destination = tempRoute2->destination;
        map->routes[routeId]->minimalYear = min(map->routes[routeId]->minimalYear, tempRoute2->minimalYear);
        map->routes[routeId]->length += tempRoute2->length;

        free(tempRoute2->path);
        deleteRoute(tempRoute2, false);
    }

    return true;
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

    Path *pathNode = getNewPathNode(NULL, toRemove);

    if (pathNode == NULL) {
        return false;
    }

    List **RoutesToInsert = calloc(1000, sizeof(List)); // TODO 1000->stała

    if (RoutesToInsert == NULL) {
        deletePathNode(pathNode);

        return false;
    }

    for (unsigned i = MINIMAL_ROUTE_ID; i <= MAXIMAL_ROUTE_ID; i++) {
        List *restrictedPaths = map->routes[i]->path;
        if (exists(restrictedPaths, pathNode)) {
            Route *newRoute = dijkstra(map, tmpCity1, tmpCity2, restrictedPaths);

            if (!isCorrectRoute(newRoute)) {
                if (newRoute != NULL) {
                    deleteRoute(newRoute, true);
                }

                for (unsigned j = i; j >= MINIMAL_ROUTE_ID; j--) {
                    deleteList(RoutesToInsert[j], (void *) deletePathNode);
                }

                deletePathNode(pathNode);
                free(RoutesToInsert);

                return false;
            }

            RoutesToInsert[i] = newRoute->path;
        }
    }

    for (unsigned i = MINIMAL_ROUTE_ID; i <= MAXIMAL_ROUTE_ID; i++) {
        insertToList(map->routes[i]->path, RoutesToInsert[i], (void(*)(void*))deletePathNode);
    }

    removeSomeRoad(map, tmpCity1, tmpCity2);

    deletePathNode(pathNode);
    free(RoutesToInsert);

    return true;
    //TODO dokończyć!!! Przejrzeć wszystkie drogi krajowe i powkładać tam rzeczy
}

/** @brief Udostępnia informacje o drodze krajowej.
 * Zwraca wskaźnik na napis, który zawiera informacje o drodze krajowej. Alokuje
 * pamięć na ten napis. Zwraca pusty napis, jeśli nie istnieje droga krajowa
 * o podanym numerze. Zaalokowaną pamięć trzeba zwolnić za pomocą funkcji free.
 * Informacje wypisywane są w formacie:
 * numer drogi krajowej;nazwa miasta;długość odcinka drogi;rok budowy lub
 * ostatniego remontu;nazwa miasta;długość odcinka drogi;rok budowy lub
 * ostatniego remontu;nazwa miasta;…;nazwa miasta.
 * Kolejność miast na liście jest taka, aby miasta @p city1 i @p city2, podane
 * w wywołaniu funkcji @ref newRoute, które utworzyło tę drogę krajową, zostały
 * wypisane w tej kolejności.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId    – numer drogi krajowej.
 * @return Wskaźnik na napis lub NULL, gdy nie udało się zaalokować pamięci.
 */
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
    Path *pathNode = position->data;
    char* routeIdString = unsignedToString(routeId);

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


