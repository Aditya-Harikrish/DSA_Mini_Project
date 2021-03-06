#include "header.h"
#include "vector.h"
#ifndef _STATION_H
#define _STATION_H

typedef struct station station;
struct station {
    int id;
    double dangerValue;
    //vector persons_visited[15]; // Discarded
    int* array_people;
    int* Arraytotal;
    vector** array_time_personVisit;//P1:1  P2:2
    vector** array_time_personLeft; //P1:3
    int transitNum;//this should become 0 of all the stations as the day passes
    int positive;
    int primary;
    int secondary;
};


void init_station(station* station_object, int id, int K);
//O(1)



#endif