/** @file
 * Implementacja struktury string, reprezentującej napis o zmiennej długości.
 */

#define _GNU_SOURCE

#include "string.h"
#include "definitions.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//stała reprezentująca maksymalną długość liczby
//w systemie dziesiętnym do zamiany na napis
const size_t MAX_NUMBER_LENGTH = 100;

struct String {
    size_t size;
    size_t maxSize;
    char *data;
};

/**
 * @brief Funckja zwiększająca zadany napis.
 * @param string — wektor do zwiększenia
 * @return Wartość @p true jeżeli wszystko się powiodło.
 * Wartość @p false jeżeli nie udało się zaalokować pamięci,
 * lub zadany napis był NULLem.
 */
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

    //zamiana napisu na taki, który nie ma z tyłu za wiele zaalokowanej pamięci
    //by uniknąć błędów pamięci przy konkatynacji
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

    //zamiana napisu na taki, który nie ma z tyłu za wiele zaalokowanej pamięci
    //by uniknąć błędów pamięci przy konkatynacji
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


