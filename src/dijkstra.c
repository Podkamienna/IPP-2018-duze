//
// Created by alicja on 13.07.19.
//
#include "dijkstra.h"
#include "heap.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct Graph Graph;

struct Graph {
    void *node;
    void *road; //droga wchodzaca do node
    Graph *prev; //do odzyskiwania najkrotszej sciezki
    int priority; //priority = najmn ob znana odl wierzch node od wierzcholka startowego
    int minYear; //minimalny rok ostatniego remontu na optymalnej sciezce od wierzch start do obecnego
};

void clearVisited(city *mapa) {
    if(mapa == NULL)
        return;

    mapa->visited = 0;

    clearVisited(mapa->left);
    clearVisited(mapa->right);
}

graphNode *getNewGraphNode(city *mapa, int priority, int minYear, graphNode *pr, priorityQueue **prQu, roadNode *road) {
    graphNode *temp = NULL;
    temp = (graphNode *) malloc(sizeof(graphNode));

    temp->node = mapa;
    temp->priority = priority;
    temp->minYear = minYear;
    temp->prev = pr;
    temp->road = road;
    (*prQu)->maxAllocIndex++;

    return temp;
}

graphNode *getGraphNode(city *mapa, int priority, int minYear, graphNode *pr, graphNode *temp, roadNode *road) {
    temp->node = mapa;
    temp->priority = priority;
    temp->minYear = minYear;
    temp->prev = pr;
    temp->road = road;

    return temp;
}

int compare(graphNode *a, graphNode *b) { //function to compare to graphNodes, to get a good order in dijkstra
    if(a->priority > b->priority)
        return -1;

    if(a->priority == b->priority && a->minYear < b->minYear)
        return -1;

    if(a->priority == b->priority && a->minYear > b->minYear)
        return 1;

    if(a->priority == b->priority && a->minYear == b->minYear)
        return 0;

    return 1;
}

priorityQueue *pop(priorityQueue *prQu) {
    int i = 1;

    if (prQu->lastUsedIndex == 0)
        return NULL;

    graphNode *temp = NULL;

    (prQu->heap)[1] = (prQu->heap)[prQu->lastUsedIndex];
    (prQu->lastUsedIndex)--;
    while (true) {
        if (leftChild(i) > prQu->lastUsedIndex)
            break;


        if (rightChild(i) > prQu->lastUsedIndex) { //compare zwr 1 jesli pierwsze mniejsze od drugiego
            if (compare((prQu->heap)[i], (prQu->heap)[leftChild(i)]) >= 0) {
                break;
            }

            temp = (prQu->heap)[i];
            (prQu->heap)[i] = (prQu->heap)[leftChild(i)];
            (prQu->heap)[leftChild(i)] = temp;

            i = leftChild(i);
        }
        else if (compare((prQu->heap)[leftChild(i)], (prQu->heap)[rightChild(i)]) >= 0) {
            if (compare((prQu->heap)[i], (prQu->heap)[leftChild(i)]) >= 0)
                break;

            temp = (prQu->heap)[i];
            (prQu->heap)[i] = (prQu->heap)[leftChild(i)];
            (prQu->heap)[leftChild(i)] = temp;

            i = leftChild(i);
        }
        else {
            if (compare((prQu->heap)[i], (prQu->heap)[rightChild(i)]) >= 0)
                break;

            temp = (prQu->heap)[i];
            (prQu->heap)[i] = (prQu->heap)[rightChild(i)];
            (prQu->heap)[rightChild(i)] = temp;

            i = rightChild(i);
        }
    }
    return prQu;
}

void clear(city *root) { //ustawianie calej mapy na nieodwiedzone
    if (root == NULL)
        return;
    root->visited = 0;
    clear(root->left);
    clear(root->right);
}

int getNonzeroMin(int a, int b) {
    if (a == 0)
        return b;

    if (b == 0)
        return a;

    if (a < b)
        return a;

    return b;
}

void getNeighbours(graphNode *temp, priorityQueue **prQu) {
    roadNode *iter1 = NULL, *iter2 = NULL; //iter1 do chodzenia poziomo, iter2 do chodzenia pionowo
    iter1 = (temp->node)->road;
    graphNode *temp1 = NULL;

    while (iter1 != NULL) { //sprawdzic, czy temp1 != end, jesli tak, to jezeli jest lepszy od prevEnd
        iter2 = iter1;
        while (iter1 != NULL) {
            temp1 = getNewGraphNode(iter1->destination, (temp->priority) + (iter1->length),
                                    getNonzeroMin(iter1->year, temp->minYear), temp, prQu, iter1);

            if ((temp1->node)->visited != 1)
                insertHeap1(temp1, prQu);
            iter1 = iter1->nextSameHash;
        }
        iter1 = iter2;
        iter1 = iter1->next;
    }

}

/**
 *
 * @param prQu
 * @param start
 * @param end
 * @return graphNode containing city end and pointers, from which we'll read the route, NULL if sth goes wrong
 */
//zrobic tak, zeby roadNode byl w graphNode, lista drog krajowych na ktorej jest droga tez
//no i jeszcze ogarnac usuwanie
graphNode *dijkstra(city *start, city *end) { //ten dijkstra jest do nowych drog
    Heap *priorityQueue = initializeHeap();
    graphNode *temp = getNewGraphNode(start, 0, 0, NULL, &prQu, NULL); //wierzcholek poczatkowy
    insertHeap1(temp, &prQu); //wkladamy go na kopiec potem mozna wyifowac jeszcze
    graphNode *worseEnd = NULL, *tempEnd = NULL; //zmienna przechowujaca graf koncowy przed ostatnia zmiana na lepsze (zeby sprawdzic, czy jednoznaczne)
    while (prQu->lastUsedIndex != 0 && worseEnd == NULL) { //dopoki kopiec niepusty czy to najlepsy mozliwy warunek?
        graphNode *temp = NULL; //wierzcholek grafu przechowujacy to, co zdjelismy z grafu
        temp = top(prQu);

        if(tempEnd != NULL && temp->node == end)
            worseEnd = temp;

        if(tempEnd == NULL && temp->node == end)
            tempEnd = temp;

        seeHeap(prQu);
        prQu = pop(prQu); //wyrzucamy obecnie przetwarzany wierzcholek z grafu
        getNeighbours(temp, &prQu); //przechodzimy po sasiadach zadanego wierzcholka, jezeli nie byli odw to ich wrzucamy na kopiec
        seeHeap(prQu);
        (temp->node)->visited = 1; //ustawiamy obecny wierzcholek na odwiedzony
    }
    if (tempEnd == NULL || (worseEnd != NULL && compare(worseEnd, tempEnd) == 0)) //sprawdzenie czy jednoznaczne
        return NULL;
    return tempEnd;
}
