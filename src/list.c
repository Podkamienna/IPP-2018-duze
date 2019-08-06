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
    int (*compare)(void *, void *)
};

List *initializeList(int compare(void *, void *)) {
    if (compare == NULL) {
        return NULL;
    }

    List *newList = malloc(sizeof(List));

    if (newList == NULL) {
        return NULL;
    }

    newList->listNode = NULL;
    newList->compare = compare;

    return newList;
}

bool addToList(List *list, void *value) {
    if (list == NULL) {
        return false;
    }

    if (value == NULL) {
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

bool exists(List *list, void *value) {
    if (list == NULL) {
        return false;
    }

    if (value == NULL) {
        return false;
    }

    ListNode *position = list->listNode;

    while (position != NULL) {
        if (list->compare(value, position->data) == 0) {
            return true;
        }
    }

    return false;
}

bool insertToList(List *list, List *toInsert, void *beginning, void *end) {
    if (list == NULL || toInsert == NULL) {
        return false;
    }

    if (list->listNode == NULL && beginning == NULL && end == NULL) {
        list->listNode = toInsert->listNode;

        return true;
    }

    if (beginning == NULL && end == NULL) {
        return false;
    }

    if (list->listNode == NULL) {
        return false;
    }

    if (toInsert->listNode == NULL) {
        return true;
    }

    ListNode *position = list->listNode;
    ListNode *endToInsert = toInsert->listNode;

    while (endToInsert->next != NULL) {
        endToInsert = endToInsert->next;
    }

    if (beginning == NULL) {
        endToInsert->next = list->listNode;
        list->listNode = toInsert->listNode;

        return true;
    }

    if (end == NULL) {
        while (position->next != NULL) {
            position = position->next;
        }

        position->next = toInsert->listNode;

        return true;
    }

    while (true) {
        if (position == NULL || position->next == NULL) {
            return false;
        }

        if (list->compare(position->data, beginning) == 0 && list->compare(position->next->data, end) == 0) {
            ListNode *positionToInsert = toInsert->listNode;

            position->next = toInsert->listNode;
            endToInsert->next = end;

            return true;
        }

        position = position->next;
    }
}

void deleteList(List *list, void deleteValue(void *)) {
    if (list == NULL) {
        return;
    }

    ListNode *position = list->listNode, *tmp;

    while(position->next != NULL) {
        tmp = position;
        if (deleteValue != NULL) {
            deleteValue(tmp->data);
        }

        free(tmp);
        position = position->next;
    }

    free(list);
}

