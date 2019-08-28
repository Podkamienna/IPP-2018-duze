/** @file
 * Implementacja struktury string, reprezentującej napis o zmiennej długości.
 */

#define _GNU_SOURCE

#include "string.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//stała reprezentująca maksymalną długość liczby
//w systemie dziesiętnym do zamiany na napis
const size_t MAX_NUMBER_LENGTH = 100;

/**
 * Struktura przechowująca napis.
 */
struct String {
    size_t size; ///< rozmiar napisu
    size_t maxSize; ///< rozmiar maksymalnego napisu
    char *data; ///< napis
};

/**
 * @brief Funkcja zwiększająca zadany napis.
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
    } else {
        free(string);
    }
}

char *getData(String *string) {
    return string->data;
}

char *intToString(int number) {
    char *text = calloc(MAX_NUMBER_LENGTH, sizeof(char));

    if (text == NULL) {
        return NULL;
    }

    if (snprintf(text, MAX_NUMBER_LENGTH, "%d", number) < 0) {
        free(text);

        return NULL;
    }

    return text;
}

char *unsignedToString(unsigned number) {
    char *text = calloc(MAX_NUMBER_LENGTH, sizeof(char));

    if (text == NULL) {
        return NULL;
    }

    if (snprintf(text, MAX_NUMBER_LENGTH, "%u", number) < 0) {
        free(text);

        return NULL;
    }

    return text;
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


