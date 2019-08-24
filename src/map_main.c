#include "map.h"
#include "definitions.h"
#include "dictionary.h"
#include "citiesAndRoads.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#undef NDEBUG

#include <assert.h>

//TODO jeszcze raz skopiować map main
int main() {
    char const *str;

    Map *m = newMap();
    assert(m);

    assert(addRoad(m, "Alinów", "Bór", 1, 2020));
    assert(addRoad(m, "Bór", "Cielińsk-Niekłańsk", 2, 2020));
    assert(addRoad(m, "Bór", "Dąb Stary", 3, 2020));
    assert(addRoad(m, "Cielińsk-Niekłańsk", "Emiliew", 4, 2020));
    assert(addRoad(m, "Dąb Stary", "Emiliew", 5, 2020));
    assert(addRoad(m, "Emiliew", "Bór", 8, 2020));
    assert(addRoad(m, "Emiliew", "Fraźnik Nowy", 3, 2020));
    assert(!repairRoad(m, "Emiliew", "Cielińsk-Niekłańsk", 2019));
    assert(repairRoad(m, "Emiliew", "Cielińsk-Niekłańsk", 2021));
    assert(!repairRoad(m, "Emiliew", "Alinów", 2020));
    assert(addRoad(m, "Fraźnik Nowy", "Grzegrzewo", 4, 2020));
    assert(addRoad(m, "Alinów", "Grzegrzewo", 10, 2020));
    assert(addRoad(m, "Homilcin", "Grzegrzewo", 5, 2020));
    assert(addRoad(m, "Fraźnik Nowy", "Cielińsk-Niekłańsk", 2, 2020));
    assert(!addRoad(m, "Fraźnik Nowy", "Cielińsk-Niekłańsk", 2, 2020));
    assert(!addRoad(m, "Cielińsk-Niekłańsk", "Fraźnik Nowy", 2, 2020));
    assert(!repairRoad(m, "Emiliew", "Bór", 2018));
    assert(repairRoad(m, "Emiliew", "Cielińsk-Niekłańsk", 2021));
    assert(repairRoad(m, "Emiliew", "Fraźnik Nowy", 2023));
    assert(addRoad(m, "Homilcin", "Cielińsk-Niekłańsk", 3, 2020));

/*  seeRoads(searchDictionary(m->cities, "Alinów"));
  seeRoads(searchDictionary(m->cities, "Bór"));
  seeRoads(searchDictionary(m->cities, "Cielińsk-Niekłańsk"));
  seeRoads(searchDictionary(m->cities, "Dąb Stary"));
  seeRoads(searchDictionary(m->cities, "Emiliew"));
  seeRoads(searchDictionary(m->cities, "Fraźnik Nowy"));
  seeRoads(searchDictionary(m->cities, "Grzegrzewo"));
*/

    assert(newRoute(m, 10, "Alinów", "Emiliew"));

    str = getRouteDescription(m, 10);
    printf("%s", str);
    assert(strcmp(str, "10;Alinów;1;2020;Bór;2;2020;Cielińsk-Niekłańsk;4;2021;Emiliew") == 0);
    free((void *) str);

    assert(extendRoute(m, 10, "Homilcin"));

    str = getRouteDescription(m, 10);
    assert(strcmp(str, "10;Alinów;1;2020;Bór;2;2020;Cielińsk-Niekłańsk;4;2021;Emiliew"
                       ";3;2023;Fraźnik Nowy;4;2020;Grzegrzewo;5;2020;Homilcin") == 0);
    free((void *) str);

    deleteMap(m);

    return 0;
}
