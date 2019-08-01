//
// Created by alicja on 01.08.2019.
//

#ifndef DROGI_LIST_H
#define DROGI_LIST_H

#include <stdbool.h>

typedef struct List List;

List *initializeList();

bool addToList(List *list, void *value);

bool exists(List *list, void *value, int compare(void *, void *));

void deleteList(List *list);

#endif //DROGI_LIST_H
