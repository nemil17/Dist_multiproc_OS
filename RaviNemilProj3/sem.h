#include<stdio.h>
#include<stdlib.h>
#include "threads.h"


// Declare a semaphore type (name it Semaphore_t) . The semaphore is a counter and a queue of TCBs.
typedef struct Semaphore_t
{
	int counter;
	TCB_t *SemQ;// Semaphore Queue
}Semaphore_t;

//mallocs a semaphore structure, initializes it to the InitValue and returns the pointer to the semaphore.
Semaphore_t * CreateSem(int InputValue)
{
	//mallocs a semaphore
	Semaphore_t *semaphore = (Semaphore_t*)malloc (sizeof(Semaphore_t));
	//Initializes it to InputValue
	semaphore->counter = InputValue;
	InitQ(&(semaphore->SemQ));
	return semaphore;
}

// takes a pointer to a semaphore and performs P, i.e. decrements the semaphore, and if the value is less than zero then blocks the process in the queue associated with the semaphore.

void P(Semaphore_t *sem)
{
	//decrements a semaphore counter
	sem->counter --;
	if(sem->counter < 0)
	{
		//block the queue associated with Semaphore
		TCB_t *delItem = DelQ(&RunQ);
		AddQ(&(sem->SemQ), delItem);
		swapcontext(&(sem->SemQ->previous->context), &(RunQ->context));
	}
}
// increments the semaphore, and if the value is 0 or negative,
// then takes a PCB out of the semaphore queue and puts it into the run queue.
void V(Semaphore_t *sem)
{
	// increments the semaphore
	sem->counter++;
	if(sem->counter <= 0)
	{
		// take the PCB out of Semaphore Q
		TCB_t *delItem = DelQ(&(sem->SemQ));
		// and Puts it in the run queue
		AddQ(&RunQ, delItem);
	}
	// The V routine also "yields" to the next runnable process.
	yield();
}

