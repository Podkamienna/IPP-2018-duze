//
// Created by alicja on 01.08.2019.
//

#include "list.h"

#include <stdlib.h>
#include <stdbool.h>

typedef struct ListNode ListNode;
typedef struct List List;

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
        position = position->next;
    }

    return false;
}

bool insertToList(List *list, List *toInsert) {
    if (list == NULL || toInsert == NULL) {
        return false;
    }

    if (toInsert->listNode == NULL) {
        return false;
    }

    if (list->listNode == NULL) {
        return false;
    }

    ListNode *beginInsert = toInsert->listNode;
    ListNode *endInsert = toInsert->listNode;

    ListNode *position = list->listNode;
    ListNode *endList = NULL;

    while (endInsert->next != NULL) {
        endInsert = endInsert->next;
    }

    if (list->compare(position, endInsert) == 0) {
        free(position);
        endInsert->next = position->next;
        list->listNode = beginInsert;

        return true;
    }

    while (position != NULL) {
        endList = position;
        if (list->compare(beginInsert->data, position->data) == 0) {
            if (position->next == NULL) {
                position->next = beginInsert->next;
                free(beginInsert);

                return true;
            }

            if (list->compare(endInsert->data, position->next->data) == 0) {
                endInsert->next = position->next->next;
                free(position->next);

                position->next = beginInsert->next;
                free(beginInsert);

                return true;
            }
        }

        position = position->next;
    }

    if (list->compare(endList->data, beginInsert->data) == 0) {
        endList->next = beginInsert->next;
        free(beginInsert);
    }

    return false;
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

