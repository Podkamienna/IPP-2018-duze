//
// Created by alicja on 04.07.19.
//

#ifndef DROGI_DEFINITIONS_H
#define DROGI_DEFINITIONS_H

#define PRIME 786433 //big prime number
#define GENERATOR 10 //number that generates multiplicative group Z_{786433}

typedef struct Dictionary Dictionary;
typedef struct Tree Tree;
typedef struct City City;
typedef struct Road Road;
typedef struct Route Route;

struct Map {
    Route routes[1000];
    Dictionary *cities;
};

struct Dictionary {
    int id;
    Tree *tree;
};

struct Tree {
    void *content;
    int hash;
    char *name;
    Tree *next, *prev;

};

struct City {
    int id;
    char *name;
    Tree roads;
};

struct Road {
    int length;
    int year;
    City city1, city2;
};

struct Route {

};

#endif //DROGI_DEFINITIONS_H
