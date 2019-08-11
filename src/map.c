#include "definitions.h"
#include "dictionary.h"
#include "route.h"
#include "citiesAndRoads.h"
#include "list.h"
#include "dijkstra.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**
 * Struktura przechowująca mapę dróg krajowych.
 */
typedef struct Map Map;

//TODO to gdzieś indziej. Gdzie?
int min(int a, int b) {
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
Map *newMap(void) {
    Map *newMap = malloc(sizeof(Map));

    if (newMap == NULL) {
        return NULL;
    }

    newMap->cities = initializeDictionary();

    if (newMap->cities == NULL) {
        free(newMap);
        return NULL;
    }

    for (int i = 0; i < 1000; i++) {
        newMap->routes[i] = NULL;
    }

    return newMap;
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

    for (int i = 0; i < 1000; i++) {
        deleteRoute(map->routes[i]);
    }

    deleteDictionary(map->cities, deleteCity);

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

    Route *newRoute = dijkstra(searchDictionary(map->cities, city1), searchDictionary(map->cities, city2), NULL);

    if (!isCorrectRoute(newRoute)) { //sprawdzam, czy udalo sie wyznaczyc droge i czy jednznacznie
        deleteRoute(newRoute);

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
//TODO porobić zmienne na miasta/restricted itd
    Route *tempRoute1 = dijkstra(searchDictionary(map->cities, city), map->routes[routeId]->source, map->routes[routeId]->path);

    if (tempRoute1 == NULL) {
        return false;
    }

    Route *tempRoute2 = dijkstra(map->routes[routeId]->destination, searchDictionary(map->cities, city), map->routes[routeId]->path);

    if (tempRoute2 == NULL) {
        deleteRoute(tempRoute1);

        return false;
    }

    int compareResult = compareRoute(tempRoute1, tempRoute2);

    if (compareResult == 0) {
        deleteRoute(tempRoute1);
        deleteRoute(tempRoute2);

        return false;
    }

    if (compareResult == 1) {
        deleteRoute(tempRoute2);

        insertToList(map->routes[routeId]->path, tempRoute1); //działa, bo w każdej drodze krajowej każde miasto występuje
        map->routes[routeId]->source = tempRoute1->source; //conajwyżej raz
        map->routes[routeId]->minimalYear = min(map->routes[routeId]->minimalYear, tempRoute1->minimalYear);
        map->routes[routeId]->length += tempRoute1->length;
    }

    if (compareResult == -1) {
        deleteRoute(tempRoute2);

        insertToList(map->routes[routeId]->path, tempRoute2); //jak wyżej
        map->routes[routeId]->destination = tempRoute1->destination;
        map->routes[routeId]->minimalYear = min(map->routes[routeId]->minimalYear, tempRoute1->minimalYear);
        map->routes[routeId]->length += tempRoute1->length;
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

    PathNode *pathNode = getNewPathNode(NULL, toRemove);

    if (pathNode == NULL) {
        return false;
    }

    List **RoutesToInsert = calloc(sizeof(List), 1000);

    if (RoutesToInsert == NULL) {
        deletePathNode(pathNode);

        return false;
    }

    for (unsigned int i = MINIMAL_ROUTE_ID; i <= MAXIMAL_ROUTE_ID; i++) {
        List *restrictedPaths = map->routes[i]->path;
        if (exists(restrictedPaths, pathNode)) {
            Route *newRoute = dijkstra(tmpCity1, tmpCity2, restrictedPaths);

            if (!isCorrectRoute(newRoute)) {
                if (newRoute != NULL) {
                    deleteRoute(newRoute);
                }

                for (unsigned j = i; j >= 0; j--) {
                    deleteList(RoutesToInsert[j], deletePathNode);
                }

                deletePathNode(pathNode);
                free(RoutesToInsert);

                return false;
            }

            RoutesToInsert[i] = newRoute->path;
        }
    }

    for (unsigned i = MINIMAL_ROUTE_ID; i <= MAXIMAL_ROUTE_ID; i++) {
        insertToList(map->routes[i]->path, RoutesToInsert[i]);
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
    if (map->routes[routeId] == NULL) {
        return NULL;
    }

    const char *string;
    size_t maximalStringSize = 10;
    size_t stringSize = 0;

    string = malloc(maximalStringSize*sizeof(char));

    if (stringSize > )



}


