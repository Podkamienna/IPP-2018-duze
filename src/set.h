/** @file
 */

#ifndef DROGI_SET_H
#define DROGI_SET_H

#include <stdbool.h>

typedef struct Set Set;

Set *initializeSet();

void *searchSet(Set *set, bool isOk(void *));

bool insertSet(Set *set, void *value);

void deleteSet(Set *set, void deleteValue(void *));

#endif /* DROGI_SET_H */
