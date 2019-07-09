//
// Created by alicja on 03.07.19.
//

#ifndef DROGI_DICTIONARY_H
#define DROGI_DICTIONARY_H

#include <stdbool.h>

typedef struct Dictionary Dictionary;

Dictionary *initializeDictionary();

void *searchDictionary(Dictionary *dictionary, const char *name);

bool insertDictionary(Dictionary *dictionary, const char *name, void *value, void deleteValue(void *));

void deleteDictionary(Dictionary *dictionary, void deleteValue(void *));

#endif //DROGI_DICTIONARY_H
