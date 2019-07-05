//
// Created by alicja on 03.07.19.
//

#include "dictionary.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct Dictionary Dictionary;
typedef struct Tree Tree;
typedef struct EntryList EntryList;

struct Dictionary {
    int id;
    Tree *tree;
};

struct Tree {
    EntryList *entries; //entries with certain hash
    uint64_t hash;
    Tree *next, *prev;
};

struct EntryList {
    EntryList *next;
    char *name;
    void *content;
};

static const uint64_t PRIME = 179425373; //big prime number
static const uint64_t GENERATOR = 259; //number that generates multiplicative group Z_MOD(PRIME)

/**
 * getHash is not NULL
 * @param text
 * @return
 */
static uint64_t getHash(const char *text) {
    size_t n = strlen(text);
    uint64_t coefficient = 1, tmpHash = 0;

    for (size_t i = 0; i < n; i++) {
        tmpHash += coefficient * text[i];
        coefficient *= GENERATOR;
        tmpHash %= PRIME;
        coefficient %= PRIME;
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
 * @param dictionary
 * @param name
 * @param hash
 * @return NULL if name is NULL or dictionary is NULL or the entry that is being searched
 * for doesn't exist
 */

void *searchDictionary(Dictionary *dictionary, const char *name) {
    if (dictionary == NULL || name == NULL)
        return NULL;
    Tree *treePosition = dictionary->tree;
    uint64_t hash = getHash(name);

    while (treePosition != NULL && hash != treePosition->hash) {
        treePosition = treePosition->next;
    }

    if (treePosition == NULL) {
        return NULL;
    }

    EntryList *listPosition = treePosition->entries;

    while (listPosition != NULL && strcmp(listPosition->name, name) != 0) {
        listPosition = listPosition->next;
    }

    if (listPosition == NULL) {
        return NULL;
    }

    return listPosition->content;
}
/**
 * the dictionary is not NULL
 * @param dictionary
 * @param name
 * @param getNewContent
 * @param hash
 * @return
 */
bool insertDictionary(Dictionary *dictionary, const char *name, void *content) {
    if (dictionary == NULL || name == NULL || content == NULL) {
        return false;
    }

    uint64_t hash = getHash(name);

    node->name = malloc(strlen(name) * sizeof(char));

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

void freeDictionary(Dictionary *dictionary, void *freeContent(void *)) {
    Tree *tmp1 = dictionary->tree, *tmp2;

    while (tmp1 != NULL) {
        freeContent(tmp1->content);
        tmp2 = tmp1;
        free(tmp1->name);
        tmp1 = tmp1->next;
        free(tmp2);
    }
}
