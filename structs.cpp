#include <string>
using namespace std;

#include "structs.idl"

#include <cstdio>
#include "c150debug.h"

using namespace C150NETWORK;  // for all the comp150 utilities 

int area(rectangle r) {
    printf("area() invoked\n");
    c150debug->printf(C150RPCDEBUG,"structs.cpp: area() invoked");
    return r.x * r.y;
}

Person findPerson(ThreePeople tp)
{
    Person to_return;
    printf("findPerson() invoked\n");
    c150debug->printf(C150RPCDEBUG,"structs.cpp: findPerson() invoked");
    if (tp.p1.firstname == "Norman" && tp.p1.lastname == "Ramsey") {
        to_return = tp.p1;
    } else {
        Person non_norman;
        non_norman.firstname = "Not Norman";
        non_norman.lastname = "Not Ramsey";
        non_norman.age = 1001;
        to_return = non_norman;
    }

    return to_return;
}

int ageSum(StructWithArrays swa)
{
    int sum = 0;
    for (int i = 0; i < 10; i++) {
        sum += swa.people[i].age;
    }

    return sum;
}

