#include "person_queries.h"
void status(person p)
{
    if (p.status == 0)
        printf("Negative\n");
    else if (p.status == 1)
        printf("Positive\n");
    else if (p.status == 2)
        printf("Primary Contact\n");
    else if (p.status == 3)
        printf("Secondary Contact\n");
}

int list_positive(person* p, int k, char func)
{
    vector v,v1;
    char ch;
    init_vector(&v);
    init_vector(&v1);
    int s = 0,s1=0;

    for (int i = 0; i < k; i++) {
        if (p[i].status == 1) {
            pushback(&v, p[i].id);
            s++;
        }
        if(p[i].status==4){
            s1++;
            pushback(&v1,p[i].id);
        }
    }
    if (func == 'P')
    {
        if(v.size == 0)
        {
            printf("There are no covid positive persons\n");
            return 0;
        }
        for (int i = 0; i < v.size; i++)
        {
            printf("%d ", v.arr[i]);
        }
        printf("\n");
        printf("Do you wish to print the list of quarintined person(y/n):\n");
        scanf("%c",&ch);
        if(ch=='y' || ch== 'Y')
        {
            if(v1.size == 0)
            {
                printf("There are no quarintined persons\n");
                return 0;
            }
            for (int i = 0; i < v1.size; i++)
            {
                printf("%d ", v1.arr[i]);
            }
        }
    }

    if (func == 'R')
        return s;
}

int list_primary(person* p, int k, char func)
{
    vector v;
    init_vector(&v);
    int s = 0;

    for (int i = 0; i < k; i++) {
        if (p[i].status == 2) {
            pushback(&v, p[i].id);
            s++;
        }
    }
    if (func == 'P')
    {
        if(v.size == 0)
        {
            printf("There are no primary contacts\n");
            return 0;
        }
        for (int i = 0; i < v.size; i++)
        {
            printf("%d ", v.arr[i]);
        }
        printf("\n");
    }

    if (func == 'R')
        return s;
}

int list_secondary(person* p, int k, char func)
{
    vector v;
    init_vector(&v);
    int s = 0;

    for (int i = 0; i < k; i++) {
        if (p[i].status == 3) {
            pushback(&v, p[i].id);
            s++;
        }
    }
    if (func == 'P')
    {
        if(v.size == 0)
        {
            printf("There are no secondary contacts\n");
            return 0;
        }
        for (int i = 0; i < v.size; i++)
        {
            printf("%d ", v.arr[i]);
        }
        printf("\n");
    }

    if (func == 'R')
        return s;
}

int list_primary_at_s(station s, person* p, int k, char func)
{

    vector v;
    init_vector(&v);
    int sum = 0;

    for (int i = 0; i < k; i++) {
        if (s.array_people[i] == 1 && p[i].status == 2) {
            pushback(&v, p[i].id);
            sum++;
        }
    }
    if (func == 'P')
    {
        if(v.size == 0)
        {
            printf("There are no primary contacts at %d\n",s.id);
            return 0;
        }
        for (int i = 0; i < v.size; i++)
        {
            printf("%d ", v.arr[i]);
        }
        printf("\n");
    }

    if (func == 'R')
        return sum;
}

int list_secondary_at_s(station s, person* p, int k, char func)
{

    vector v;
    init_vector(&v);
    int sum = 0;

    for (int i = 0; i < k; i++) {
        if (s.array_people[i] == 1 && p[i].status == 3) {
            pushback(&v, p[i].id);
            sum++;
        }
    }
    if (func == 'P')
    {
        if(v.size == 0)
        {
            printf("There are no secondary contacts at %d\n",s.id);
            return 0;
        }
        for (int i = 0; i < v.size; i++)
        {
            printf("%d ", v.arr[i]);
        }
        printf("\n");
    }

    if (func == 'R')
        return sum;
}

int list_positive_at_s(station s, person* p, int k, char func)
{
    char ch;
    vector v,v1;
    init_vector(&v);
    init_vector(&v1);
    int sum = 0,s1=0;

    for (int i = 0; i < k; i++) {
        if (s.array_people[i] == 1 && p[i].status == 1) {
            pushback(&v, p[i].id);
            sum++;
        }
    }
    for (int i = 0; i < k; i++) {
        if (s.array_people[i] == 1 && p[i].status == 4) {
            pushback(&v1, p[i].id);
            s1++;
        }
    }
    if (func == 'P')
    {
        if(v.size == 0)
        {
            printf("There are no covid positive person at %d\n",s.id);
            return 0;
        }
        for (int i = 0; i < v.size; i++)
        {
            printf("%d ", v.arr[i]);
        }
        printf("\n");
        printf("Do you wish to print the list of quarintined person(y/n):\n");
        scanf("%c",&ch);
        if(ch=='y' || ch== 'Y')
        {
            if(v1.size == 0)
            {
                printf("There are no quarintined persons\n");
                return 0;
            }
            for (int i = 0; i < v1.size; i++)
            {
                printf("%d ", v1.arr[i]);
            }
        }
    }

    if (func == 'R')
        return sum;
}

int location(person p, int curr_day, char func)
{
    curr_day = curr_day % 15;
    vector v = p.stations_visited[curr_day];
    if (func == 'R')
        return v.arr[v.size - 1];
    if (func == 'P')
        printf("%d\n", v.arr[v.size - 1]);
}

double danger_value(station s, person* p, int K)
{
    int pos = list_positive_at_s(s, p, K, 'R');
    int prim = list_primary_at_s(s, p, K, 'R');
    int sec = list_secondary_at_s(s, p, K, 'R');
    double x = (double)pos + (double)prim / 5 + (double)sec / 10;
    //printf("Danger_val = %lf\n", x);
    return x;
}