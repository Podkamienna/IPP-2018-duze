//
// Created by alicja on 24.08.2019.
//

#ifndef DROGI_FUNCTION_FOR_PARSING_H
#define DROGI_FUNCTION_FOR_PARSING_H

#include "vector.h"

typedef enum InputType InputType;

typedef struct Map Map;

enum InputType {
    INVALID,
    IGNORE,
    CORRECT
};

InputType checkInputCorrectness(char *string, size_t *stringLengthPtr);

bool split(Vector *splittedInput, char *stringToSplit);

bool execute(Map *map, Vector *splittedVector);

#endif //DROGI_FUNCTION_FOR_PARSING_H
