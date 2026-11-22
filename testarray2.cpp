#include <string>
using namespace std;

#include "testarray2.idl"

#include <cstdio>
#include "c150debug.h"

using namespace C150NETWORK;  // for all the comp150 utilities 

int sqrt(int x[24], int y[24][15], int z[24][15]) {
    printf("sqrt() invoked!\n");
    c150debug->printf(C150RPCDEBUG, "testarray2.cpp: sqrt() invoked");
    return (x[0] + y[0][1] + z[2][2]);
}