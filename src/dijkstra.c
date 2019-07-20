//
// Created by alicja on 13.07.19.
//
#include "dijkstra.h"
#include "heap.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct Graph Graph;
typedef struct City City;
typedef struct Road Road;

struct Graph {
    City *node;
    Road *edge; //droga wchodzaca do node
    Graph *prev; //do odzyskiwania najkrotszej sciezki
    int priority; //priority = najmn ob znana odl wierzch node od wierzcholka startowego
    int minYear; //minimalny rok ostatniego remontu na optymalnej sciezce od wierzch start do obecnego
};



Graph *getNewGraphNode(void *node, void *edge, int priority, int minYear, Graph *pr) {
    Graph *newGraph = NULL;
    newGraph = malloc(sizeof(Graph));

    if (newGraph == NULL) {
        return NULL;
    }

    newGraph->node = node;
    newGraph->priority = priority;
    newGraph->minYear = minYear;
    newGraph->prev = pr;
    newGraph->edge = edge;

    return newGraph;
}


int compare(Graph *a, Graph *b) { //function to compare to graphNodes, to get a good order in dijkstra
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

int getNonzeroMin(int a, int b) {
    if (a == 0)
        return b;

    if (b == 0)
        return a;

    if (a < b)
        return a;

    return b;
}

void getNeighbours(Graph *temp, Heap *heap) {
    Road *iter1 = NULL, *iter2 = NULL; //iter1 do chodzenia poziomo, iter2 do chodzenia pionowo
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
