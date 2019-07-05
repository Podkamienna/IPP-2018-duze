//
// Created by alicja on 03.07.19.
//

#ifndef DROGI_DICTIONARY_H
#define DROGI_DICTIONARY_H

#include "definitions.h"

#include <stdbool.h>


Dictionary *initializeDictionary(int hash);

void *searchDictionary(Dictionary *dictionary, char *name);

bool *insertDictionary(Dictionary *dictionary, char *name, void *getNewNode(void));

bool deleteDictionary(Dictionary *dictionary, char *name);

void freeDictionary(Dictionary *dictionary, void freeContent());

#endif //DROGI_DICTIONARY_H
