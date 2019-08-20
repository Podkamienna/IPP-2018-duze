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

    newList->beginning = NULL;
    newList->end = NULL;
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

    newListNode->next = list->beginning;

    if (list->beginning != NULL) {
        list->beginning->prev = newListNode;
    }

    newListNode->prev = NULL;
    newListNode->data = value;

    list->beginning = newListNode;

    if (list->end == NULL) {
        list->end = newListNode;
    }

    return true;
}

//TODO chyba usunąć
/*
void reverseList(List *list) {
    if (list == NULL) {
        return;
    }

    if (list->listNode == NULL) {
        return;
    }

    ListNode *position = list->listNode;
    ListNode *prevPosition = NULL;
    ListNode *nextPosition = position->next;

    while (position != NULL) {
        position->next = prevPosition;
        prevPosition = position;
        position = nextPosition;
        if (nextPosition != NULL) {
            nextPosition = nextPosition->next;
        }
    }

    list->listNode = prevPosition;
}
*/
bool exists(List *list, void *value) {
    if (list == NULL) {
        return false;
    }

    if (value == NULL) {
        return false;
    }

    ListNode *position = list->beginning;

    while (position != NULL) {
        if (list->compare(value, position->data) == 0) {
            return true;
        }
        position = position->next;
    }

    return false;
}

void deleteListNode(ListNode *listNode, void deleteValue(void *)) {
    if (listNode == NULL) {
        return;
    }

    if (deleteValue != NULL) {
        deleteValue(listNode->data);
    }

    free(listNode);
}

//zakładam, że listy mają przynajmniej długość 2
bool insertAtTheBeginning(List *list, List *toInsert, void deleteValue(void *)) {
    if (list == NULL || toInsert == NULL) {
        return false;
    }

    if (list->compare(toInsert->end->data, list->beginning->data) != 0) {
        return false;
    }

    toInsert->end->prev->next = list->beginning;
    list->beginning->prev = toInsert->end->prev;

    deleteListNode(list->beginning, deleteValue);

    list->beginning = toInsert->beginning;

    return true;
}

bool insertAtTheEnd(List *list, List *toInsert, void deleteValue(void *)) {
    if (list == NULL || toInsert == NULL) {
        return false;
    }

    if (list->compare(toInsert->beginning->data, list->end->data) != 0) {
        return false;
    }

    list->end->prev->next = toInsert->beginning;
    toInsert->beginning->prev = list->end->prev;

    deleteListNode(list->end, deleteValue);

    list->end = toInsert->end;

    return true;
}

bool insertToList(List *list, List *toInsert, void deleteValue(void *)) {
    if (list == NULL || toInsert == NULL) {
        return false;
    }

    ListNode *position = list->beginning;

    if (list->beginning == list->end || toInsert->beginning == toInsert->end) {
        return false;
    }

    while (position->next != NULL) {
        if (list->compare(position->data, toInsert->beginning) == 0) {
            if (list->compare(position->next->data, toInsert->end) == 0) {
                if (position->prev == NULL) { //Jeżeli jest wstawiane na początku
                    ListNode *toDelete1 = toInsert->end;
                    ListNode *toDelete2 = list->beginning;

                    toInsert->end->prev->next = list->beginning->next;
                    list->beginning = toInsert->beginning;

                    deleteListNode(toDelete1, deleteValue);
                    deleteListNode(toDelete2, deleteValue);

                    return true;
                }

                ListNode *toDelete1 = toInsert->end;
                ListNode *toDelete2 = position;

                position->prev->next = toInsert->beginning;
                toInsert->end->prev->next = position->next;

                deleteListNode(toDelete1, deleteValue);
                deleteListNode(toDelete2, deleteValue);

                return true;
            }
        }
        position = position->next;
    }

    return false;
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

void *getLast(List *list) {
    if (list == NULL) {
        return NULL;
    }

    return list->beginning->data;
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

void *getNextListIterator(ListIterator *listIterator) {//TODO pozmieniać resztę, żeby było tak samo
    if (listIterator == NULL) {                         //TODO użyć tam gdzieś
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