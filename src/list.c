//
// Created by alicja on 01.08.2019.
//

#include "list.h"

#include <stdlib.h>
#include <stdbool.h>

typedef struct ListNode ListNode;
typedef struct List List;

struct ListNode {
    void *data;
    ListNode *next;
};

struct List {
    ListNode *listNode;
};

List *initializeList() {
    List *newList = malloc(sizeof(List));

    if (newList == NULL) {
        return NULL;
    }

    newList->listNode = NULL;

    return newList;
}

bool addToList(List *list, void *value) {
    if (list == NULL) {
        return false;
    }

    ListNode *newListNode = malloc(sizeof(ListNode));

    if (newListNode == NULL) {
        return false;
    }

    newListNode->next = list->listNode;
    newListNode->data = value;

    list->listNode = newListNode;

    return true;
}

bool exists(List *list, void *value, int compare(void *, void *)) {
    if (list == NULL) {
        return false;
    }

    if (value == NULL) {
        return false;
    }

    ListNode *position = list->listNode;

    while (position != NULL) {
        if (compare(value, position->data) == 0) {
            return true;
        }
    }

    return false;
}

void deleteList(List *list) {
    if (list == NULL) {
        return;
    }

    ListNode *position = list->listNode, *tmp;

    while(position->next != NULL) {
        tmp = position;
        free(tmp);

        position = position->next;
    }

    free(list);
}

