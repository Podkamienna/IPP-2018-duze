//
// Created by alicja on 11.08.2019.
//

#include "string.h"
#include "definitions.h"

#include <string.h>

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

char *numberToString(int number) {
    int n = number;

    String *string = initializeString();

    if (n < 0) {
        if (!concatenateString(string, MINUS)) {
            deleteString(string, true);

            return NULL;
        }
    }

    while (n != 0) {
        char m = n%10 - ZERO; //czy tu nie trzeba zaalokować? chyba nie.

        if (!concatenateString(string, &m)){
            deleteString(string, true);

            return NULL;
        }

        n = n/10;
    }

    char *value = string->data;
    deleteString(string, false);
    return value;
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
    if (deleteData) {
        free(string->data);
        free(string);
    }

    else {
        free(string);
    }
}
