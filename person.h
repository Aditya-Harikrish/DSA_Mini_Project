#include "vector.h"
#ifndef PERSON_H
#define PERSON_H

#define NEGATIVE 0
#define POSITIVE 1
#define PRIMARY_CONTACT 2
#define SECONDARY_CONTACT 3

typedef struct person person;
struct person {
    int id; // starts from 0
    int status;
    vector stations_visited[15];
};
void init_person(person *p);
void add_travel(person* p, station *s, int day, int destination);
int getStatus(person* p);


#endif