//
// Created by alicja on 03.07.19.
//

#include "dictionary.h"
#include "definitions.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>


int getHash(char *text) {
    int n = strlen(text);
    int tmp = 1, tmpHash = 0;
    for (int i = 0; i < n; i++) {
        tmpHash += tmp * text[i];
        tmp *= GENERATOR;
        tmpHash %= PRIME;
        tmp %= PRIME;
    }
    return tmpHash;
}

Dictionary *initializeDictionary() {
    Dictionary *newDictionary = malloc(sizeof(Dictionary));

    if (newDictionary == NULL)
        return NULL;

    newDictionary->tree = NULL;

    return newDictionary;
}
/**
 * The dictionary is not NULL, returns the list node that was searched for or NULL if there was nothing found
 * @param dictionary
 * @param name
 * @param hash
 * @return
 */
void *searchDictionary(Dictionary *dictionary, char *name) {
    Tree *tmp = dictionary->tree;
    int hash = getHash(name);

    while (tmp != NULL || (hash != tmp->hash || strcmp(name, tmp->name) != 0)) {
        tmp = tmp->next;
    }

    return tmp;
}
/**
 * the dictionary is not NULL
 * @param dictionary
 * @param name
 * @param getNewContent
 * @param hash
 * @return
 */
bool *insertDictionary(Dictionary *dictionary, char *name, void *getNewContent(void)) {
    Tree *node = malloc(sizeof(Tree));
    int hash = getHash(name);

    if (node == NULL) {
        return false;
    }

    void *newContent = getNewContent();

    if (newContent == NULL) {
        return false;
    }

    node->name = malloc(strlen(name)*sizeof(char));

    for (int i = 0; i < strlen(name); i++) {
        node->name[i] = name[i];
    }

    node->hash = getHash(name);
    node->prev = NULL;
    node->next = dictionary->tree;
    node->content = newContent;
    dictionary->tree->prev = node;
    dictionary->tree = node;

    return true;
}

/**
 * dictionary is not NULL
 * @param dictionary
 * @param name
 * @return
 */
bool deleteDictionary(Dictionary *dictionary, char *name) {
    Tree *tmp = searchDictionary(dictionary, name);

    if (tmp == NULL) {
        return false;
    }

    if (tmp->prev == NULL && tmp->next == NULL) {
        free(tmp);
        dictionary->tree = NULL;

        return true;
    }

    if (tmp->prev == NULL) {
        tmp->next->prev = NULL;
        dictionary->tree = tmp->next;
        free(tmp);

        return true;
    }

    if (tmp->next == NULL) {
        tmp->prev->next = NULL;
        free(tmp);

        return true;
    }

    tmp->prev->next = tmp->next;
    tmp->next->prev = tmp->prev;
    free(tmp);
    return true;

}

void freeDictionary(Dictionary *dictionary, void freeContent()) {
    Tree *tmp1 = dictionary->tree, *tmp2;

    while (tmp1 != NULL) {
        freeContent(tmp1->content);
        tmp2 = tmp1;
        free(tmp1->name);
        tmp1 = tmp1->next;
        free(tmp2);
    }
}
