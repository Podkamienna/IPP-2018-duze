//
// Created by alicja on 14.08.2019.
//

#include "string.h"
#include "stdio.h"
int main() {
    String *a = initializeString();
    char *b = calloc(1,5);
    b = "abcd";
    char *c = calloc(1, 3);
    c = "fg";
    concatenateString(a, b);
    concatenateString(a, c);
    printf("%s", getData(a));
}
