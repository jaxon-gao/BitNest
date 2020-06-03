#ifndef NEST_LOCK
#define NEST_LOCK
#include<stdio.h>
#include<stdlib.h>
#include<sys/sem.h>
union semun;
int set_semvalue(int sem_id, int val);
int lock_p(int sem);
int lock_v(int sem);
void del_semvalue(int sem);
#endif