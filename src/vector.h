/** @file
 */

#ifndef DROGI_VECTOR_H
#define DROGI_VECTOR_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct Vector Vector;

struct Vector {
    size_t size;
    size_t maxSize;
    void **data;
};

bool resizeVector(Vector *vector);

Vector *initializeVector(size_t initialSize);

bool pushVector(Vector *vector, void *value);

void deleteVector(Vector *vector, void deleteValue(void *));


#endif /* DROGI_VECTOR_H */
