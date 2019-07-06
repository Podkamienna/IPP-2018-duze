/** @file
 */

#include "set.h"

#include <stdlib.h>

struct Set {
    Set *next;
    void *content;
};

Set *initializeSet() {
    Set *newSet = malloc(sizeof(Set));

    if (newSet == NULL) {
        return NULL;
    }

    newSet->next = NULL;
    newSet->content =
}

void *searchSet(bool isOk(void *));

bool *insertSet(void *content);

void freeSet();