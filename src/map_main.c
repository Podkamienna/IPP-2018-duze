#include "map.h"
#include "definitions.h"

#include <stdio.h>



int main() {
    Map *map = newMap();

    FAIL_IF(map == NULL);

    failure:;
    deleteMap(map);
    return 0;
}