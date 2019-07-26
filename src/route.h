//
// Created by alicja on 19.07.19.
//

#ifndef DROGI_ROUTE_H
#define DROGI_ROUTE_H

#include "definitions.h"

static const int MINIMAL_ROUTE_ID = 1;
static const int MAXIMAL_ROUTE_ID = 999;

Route *initializeRoute();

void deleteRoute(Route route);

#endif //DROGI_ROUTE_H
