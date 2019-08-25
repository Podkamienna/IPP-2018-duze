//
// Created by alicja on 24.08.2019.
//

#include "functionForParsing.h"
#include "definitions.h"
#include "map.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

static bool stringToUnsigned(const char *string, unsigned *numberPtr);

static bool stringToInt(const char *string, int *numberPtr);

static bool executeNewRouteSpecified(Map *map, Vector *splittedVector);

static bool stringToUnsigned(const char *string, unsigned *numberPtr) {
    FAIL_IF(string == NULL);
    FAIL_IF(*string == '\0');
    FAIL_IF(*string == '-');
    FAIL_IF(isspace(*string));

    char *endPtr = NULL;

    errno = 0;
    long unsigned tmpNumber = strtoul(string, &endPtr, 10);

    FAIL_IF(errno != 0);
    FAIL_IF(*endPtr != '\0');

    FAIL_IF(tmpNumber > UINT_MAX);
    *numberPtr = (unsigned) tmpNumber;

    return true;

    failure:;
    return false;
}

static bool stringToInt(const char *string, int *numberPtr) {
    FAIL_IF(string == NULL);
    FAIL_IF(*string == '\0');
    FAIL_IF(isspace(*string));

    char *endPtr = NULL;

    errno = 0;
    long tmpNumber = strtol(string, &endPtr, 10);

    FAIL_IF(errno != 0);
    FAIL_IF(*endPtr != '\0');

    FAIL_IF(tmpNumber > INT_MAX || tmpNumber < INT_MIN);
    *numberPtr = (int) tmpNumber;

    return true;

    failure:;
    return false;
}

static bool executeNewRouteSpecified(Map *map, Vector *splittedVector) {
    char **cityNames = NULL;
    unsigned *lengths = NULL;
    int *years = NULL;

    char **arguments = (char **) splittedVector->data;
    size_t roadCount = splittedVector->size / 3;
    FAIL_IF(roadCount < 1);
    FAIL_IF(roadCount * 3 + 2 != splittedVector->size);

    unsigned routeId = 0;
    FAIL_IF(!stringToUnsigned(arguments[0], &routeId));

    cityNames = malloc((roadCount + 1) * sizeof(char *));
    FAIL_IF(cityNames == NULL);

    lengths = malloc(roadCount * sizeof(unsigned));
    FAIL_IF(lengths == NULL);

    years = malloc(roadCount * sizeof(int));
    FAIL_IF(years == NULL);

    for (size_t i = 0; i < roadCount + 1; i++) {
        size_t position = 3 * i + 1;

        cityNames[i] = arguments[position];
    }

    for (size_t i = 0; i < roadCount; i++) {
        size_t position = 3 * i + 2;

        FAIL_IF(!stringToUnsigned(arguments[position], &lengths[i]));
    }

    for (size_t i = 0; i < roadCount; i++) {
        size_t position = 3 * i + 3;

        FAIL_IF(!stringToInt(arguments[position], &years[i]));
    }

    FAIL_IF(!newRouteSpecified(map, routeId, cityNames, lengths, years, roadCount));

    free(cityNames);
    free(lengths);
    free(years);

    return true;

    failure:;
    free(cityNames);
    free(lengths);
    free(years);

    return false;
}

InputType checkInputCorrectness(char *string, size_t *stringLengthPtr) {
    if (string == NULL) {
        return INVALID;
    }

    if (string[0] == '\n') {
        return IGNORE;
    }

    if (string[0] == '#') {
        return IGNORE;
    }

    if (strlen(string) != *stringLengthPtr) {
        return INVALID;
    }

    string[--(*stringLengthPtr)] = '\0';

    return CORRECT;
}

bool split(Vector *splittedInput, char *stringToSplit) {
    resetVector(splittedInput);

    if (splittedInput == NULL) {
        return false;
    }

    if (stringToSplit == NULL) {
        return false;
    }

    char *position = stringToSplit;
    char *delimiter;

    while (true) {
        if (!pushToVector(splittedInput, position)) {
            return false;
        }

        delimiter = strchr(position, *DELIMITER);

        if (delimiter == NULL) {
            return true;
        }

        *delimiter = '\0';

        position = delimiter + 1;
    }
}

bool execute(Map *map, Vector *splittedVector) {
    FAIL_IF(splittedVector == NULL);

    char **argumentTable = (char **) splittedVector->data;

    if (strcmp(argumentTable[0], "addRoad") == 0) {
        FAIL_IF(splittedVector->size != 5);

        unsigned length = 0;
        FAIL_IF(!stringToUnsigned(argumentTable[3], &length));

        int year = 0;
        FAIL_IF(!stringToInt(argumentTable[4], &year));

        FAIL_IF(!addRoad(map, argumentTable[1], argumentTable[2], length, year));

    } else if (strcmp(argumentTable[0], "repairRoad") == 0) {
        FAIL_IF(splittedVector->size != 4);

        int year = 0;
        FAIL_IF(!stringToInt(argumentTable[3], &year));

        FAIL_IF(!repairRoad(map, argumentTable[1], argumentTable[2], year));
    } else if (strcmp(argumentTable[0], "newRoute") == 0) {
        FAIL_IF(splittedVector->size != 4);

        unsigned routeId = 0;
        FAIL_IF(!stringToUnsigned(argumentTable[1], &routeId));

        FAIL_IF(!newRoute(map, routeId, argumentTable[2], argumentTable[3]));
    } else if (strcmp(argumentTable[0], "extendRoute") == 0) {
        FAIL_IF(splittedVector->size != 3);

        unsigned routeId = 0;
        FAIL_IF(!stringToUnsigned(argumentTable[1], &routeId));

        FAIL_IF(!extendRoute(map, routeId, argumentTable[2]));
    } else if (strcmp(argumentTable[0], "removeRoad") == 0) {
        FAIL_IF(splittedVector->size != 3);

        FAIL_IF(!removeRoad(map, argumentTable[1], argumentTable[2]));
    } else if (strcmp(argumentTable[0], "getRouteDescription") == 0) {
        FAIL_IF(splittedVector->size != 2);

        unsigned routeId = 0;
        FAIL_IF(!stringToUnsigned(argumentTable[1], &routeId));

        const char *routeDescription = getRouteDescription(map, routeId);
        FAIL_IF(routeDescription == NULL);

        printf("%s\n", routeDescription);

        free((void *) routeDescription);
    } else if (strcmp(argumentTable[0], "removeRoute") == 0) {
        FAIL_IF(splittedVector->size != 2);

        unsigned routeId = 0;
        FAIL_IF(!stringToUnsigned(argumentTable[1], &routeId));

        FAIL_IF(!removeRoute(map, routeId));
    } else {
        FAIL_IF(!executeNewRouteSpecified(map, splittedVector));
    }

    return true;

    failure:;
    return false;
}
