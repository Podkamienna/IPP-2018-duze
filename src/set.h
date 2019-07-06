/** @file
 * @author Antoni Żewierżejew <azewierzejew@gmail.com>
 * @date 05.07.2019
 */

#ifndef DROGI_SET_H
#define DROGI_SET_H

#include <stdbool.h>

typedef struct Set Set;

Set *initializeSet();

void *searchSet(bool isOk(void *));

bool *insertSet(void *content);

void freeSet();

#endif /* DROGI_SET_H */
