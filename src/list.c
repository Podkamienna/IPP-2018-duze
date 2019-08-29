/**
 * @file Implementacja listy wskaźnikowej dwukierunkowej.
 */

#include "list.h"

#include <stdlib.h>
#include <stdbool.h>

typedef struct ListNode ListNode;
typedef struct List List;

static ListNode *getNewListNode(ListNode *next, void *value) {
    ListNode *newListNode = malloc(sizeof(ListNode));

    if (newListNode == NULL) {
        return NULL;
    }

    newListNode->next = next;

    if (next != NULL) {
        next->prev = newListNode;
    }

    newListNode->prev = NULL;
    newListNode->data = value;

    return newListNode;
}

static void deleteListNode(ListNode *listNode, void deleteValue(void *)) {
    if (listNode == NULL) {
        return;
    }

    if (deleteValue != NULL) {
        deleteValue(listNode->data);
    }

    free(listNode);
}

List *initializeList() {
    List *newList = malloc(sizeof(List));

    if (newList == NULL) {
        return NULL;
    }

    newList->beginning = NULL;
    newList->end = NULL;

    return newList;
}

void deleteList(List *list, void deleteValue(void *)) {
    if (list == NULL) {
        return;
    }

    ListNode *position = list->beginning, *tmp;

    while (position != NULL) {
        tmp = position->next;

        if (deleteValue != NULL) {
            deleteValue(position->data);
        }

        free(position);
        position = tmp;
    }

    free(list);
}


bool addToList(List *list, void *value) {
    if (list == NULL) {
        return false;
    }

    if (value == NULL) {
        return false;
    }

    ListNode *newListNode = getNewListNode(list->beginning, value);

    if (newListNode == NULL) {
        return false;
    }

    list->beginning = newListNode;

    if (list->end == NULL) {
        list->end = newListNode;
    }

    return true;
}

void *searchList(List *list, void *value, bool areEqual(void *, void *)) {
    if (list == NULL) {
        return NULL;
    }

    if (value == NULL) {
        return NULL;
    }

    ListNode *position = list->beginning;

    while (position != NULL) {
        if (areEqual(value, position->data)) {
            return position->data;
        }
        position = position->next;
    }

    return NULL;
}

bool insertAtTheBeginningList(List *list, List *toInsert, void (*deleteValue)(void *)) {
    if (list == NULL || toInsert == NULL) {
        return false;
    }

    toInsert->end->prev->next = list->beginning;
    list->beginning->prev = toInsert->end->prev;

    deleteListNode(toInsert->end, deleteValue);

    list->beginning = toInsert->beginning;

    free(toInsert);
    return true;
}

bool insertAtTheEndList(List *list, List *toInsert, void (*deleteValue)(void *)) {
    if (list == NULL || toInsert == NULL) {
        return false;
    }

    list->end->prev->next = toInsert->beginning;
    toInsert->beginning->prev = list->end->prev;

    deleteListNode(list->end, deleteValue);

    list->end = toInsert->end;

    free(toInsert);
    return true;
}

bool insertToList(List *list, List *toInsert, void deleteValue(void *), bool areEqual(void *, void *)) {
    if (list == NULL || toInsert == NULL) {
        return false;
    }

    ListNode *position = list->beginning;

    if (list->beginning == list->end || toInsert->beginning == toInsert->end) {
        return false;
    }

    while (position->next != NULL) {
        if (areEqual(position->data, toInsert->beginning->data)) {
            if (areEqual(position->next->data, toInsert->end->data)) {
                ListNode *toDelete1 = toInsert->end;
                ListNode *toDelete2 = position;

                toInsert->end->prev->next = position->next;
                toInsert->beginning->prev = position->prev;
                position->next->prev = toInsert->end->prev;

                if (position->prev == NULL) {
                    list->beginning = toInsert->beginning;
                } else {
                    position->prev->next = toInsert->beginning;
                }

                deleteListNode(toDelete1, deleteValue);
                deleteListNode(toDelete2, deleteValue);

                free(toInsert);
                return true;
            }
        }
        position = position->next;
    }

    return false;
}

ListIterator *getNewListIterator(List *list) {
    if (list == NULL) {
        return NULL;
    }

    ListIterator *newListIterator = malloc(sizeof(ListIterator));
    if (newListIterator == NULL) {
        return NULL;
    }

    newListIterator->listNode = list->beginning;

    return newListIterator;
}

void *getNextListIterator(ListIterator *listIterator) {
    if (listIterator == NULL) {
        return NULL;
    }

    if (listIterator->listNode == NULL) {
        return NULL;
    }

    void *returnValue = listIterator->listNode->data;

    listIterator->listNode = listIterator->listNode->next;

    return returnValue;
}

void deleteListIterator(ListIterator *listIterator) {
    free(listIterator);
}