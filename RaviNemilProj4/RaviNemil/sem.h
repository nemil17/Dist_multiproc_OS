#ifndef SEM_H_INCLUDED
#define SEM_H_INCLUDED

#include <stdlib.h>
#include "threads.h"

typedef struct Semaphore_t
{
    int count;
    TCB_t * SemQ;
} Semaphore_t;

// mallocs a semaphore structure, initializes it to the InitValue and returns the pointer to the semaphore.
Semaphore_t * CreateSem(int InputValue)
{
    Semaphore_t * sem = (Semaphore_t*)malloc (sizeof (Semaphore_t));
    sem->count = InputValue;
    InitQ(&(sem->SemQ));
//    printf("CreateSem %p -> %d\n", sem, sem->count);
    return sem;
}

// takes a pointer to a semaphore and performs P, i.e. decrements the semaphore,
// and if the value is less than zero then blocks the process in the queue associated with the semaphore.
void P(Semaphore_t * sem)
{
    sem->count--;
//    printf("P %p -> %d\n", sem, sem->count);
    if (sem->count<0)
    {
        TCB_t * t = DelQ(&RunQ);
        AddQ(&(sem->SemQ), t);
        swapcontext(&(sem->SemQ->previous->context), &(RunQ->context));
    }
}

// increments the semaphore, and if the value is 0 or negative,
// then takes a PCB out of the semaphore queue and puts it into the run queue.
// Note: The V routine also "yields" to the next runnable process.
void V(Semaphore_t * sem)
{
    sem->count++;
//    printf("V %p -> %d\n", sem, sem->count);
    if (sem->count<=0)
    {
        TCB_t * t = DelQ(&(sem->SemQ));
        AddQ(&RunQ, t);
    }
    yield();
}

#endif // SEM_H_INCLUDED

