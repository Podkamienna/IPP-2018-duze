//
// Created by alicja on 11.08.2019.
//
#define _GNU_SOURCE

#include "string.h"
#include "definitions.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const size_t MAX_NUMBER_LENGTH = 100;

static bool resizeString(String *string);

static bool resizeString(String *string) {
    if (string == NULL) {
        return false;
    }

    size_t newSize = 2 * string->maxSize + 4;

    char *newData = realloc(string->data, newSize);

    if (newData == NULL) {
        return false;
    }

    string->data = newData;
    string->maxSize = newSize;

    return true;
}

String *initializeString() {
    String *newString = malloc(sizeof(String));

    if (newString == NULL) {
        return NULL;
    }

    newString->data = NULL;


    newString->size = 0;
    newString->maxSize = 0;

    return newString;
}

char *getData(String *string) {
    return string->data;
}

char *intToString(int number) {
    char *tmpText = calloc(MAX_NUMBER_LENGTH, sizeof(char));

    if (tmpText == NULL) {
        return NULL;
    }

    if (snprintf(tmpText, MAX_NUMBER_LENGTH, "%d", number) == -1)  {
        free (tmpText);

        return NULL;
    }

    char *finalText = strdup(tmpText);

    free(tmpText);

    return finalText;
}

char *unsignedToString(unsigned number) {
    char *tmpText = calloc(MAX_NUMBER_LENGTH, sizeof(char));

    if (tmpText == NULL) {
        return NULL;
    }

    if (snprintf(tmpText, MAX_NUMBER_LENGTH, "%u", number) == -1)  {
        free (tmpText);

        return NULL;
    }

    char *finalText = strdup(tmpText);

    free(tmpText);

    return finalText;
}


bool concatenateString(String *string1, const char *string2) {
    if (string1 == NULL || string2 == NULL) {
        return false;
    }

    size_t lengthString2 = strlen(string2);

    while (string1->size + lengthString2 + 1 > string1->maxSize) {
        if (!resizeString(string1)) {
            return false;
        }
    }

    strcpy(&string1->data[string1->size], string2);
    string1->size += lengthString2;

    return true;
}

void deleteString(String *string, bool deleteData) {
    if (string == NULL) {
        return;
    }

    if (deleteData) {
        free(string->data);
        free(string);
    }

    else {
        free(string);
    }
}
