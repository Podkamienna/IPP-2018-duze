/**
 * @file Program pozwalający na wykonywanie operacji na mapie i zawartych
 * w niej miastach, drogach i drogach krajowych.
 */

#define _GNU_SOURCE

#include "map.h"
#include "definitions.h"
#include "vector.h"
#include "functionForParsing.h"

#include <stdio.h>

static void printErrorMessage(size_t lineNumber) {
    fprintf(stderr, "ERROR %zu\n", lineNumber);
}

int main() {
    Vector *splittedInput = NULL;
    Map *map = NULL;
    char *buffer = NULL;

    map = newMap();
    FAIL_IF(map == NULL);

    splittedInput = initializeVector();
    FAIL_IF(splittedInput == NULL);

    buffer = NULL;
    size_t bufferSize = 0;

    __ssize_t readLength = getline(&buffer, &bufferSize, stdin);

    for (size_t i = 1; readLength != -1; readLength = getline(&buffer, &bufferSize, stdin), i++) {
        size_t length = readLength;
        switch (checkInputCorrectness(buffer, &length)) {
            case INVALID:
                printErrorMessage(i);
                continue;
            case IGNORE:
                continue;
            case CORRECT:
                break;
        }

        FAIL_IF(!split(splittedInput, buffer));

        if (!execute(map, splittedInput)) {
            printErrorMessage(i);
        }
    }

    failure:;
    deleteMap(map);
    deleteVector(splittedInput, NULL);
    free(buffer);

    return 0;
}