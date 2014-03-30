#ifndef THREADS_H_INCLUDED
#define THREADS_H_INCLUDED

#include <stdlib.h>
#include "q.h"

void start_thread(void (*function)(void))
{
// allocate a stack (via malloc) of a certain size (choose 8192)
    int size = 8192;
    void * stack = malloc(size);
// allocate a TCB (via malloc)
    TCB_t * tcb = malloc(sizeof(TCB_t));
// call init_TCB with appropriate arguments
    init_TCB(tcb, function, stack, size);
// call addQ to add this TCB into the ¡°RunQ¡± which is a global header pointer
    AddQ(&RunQ, tcb);
}

void run()
{
    // real code
    ucontext_t parent;     // get a place to store the main context, for faking
    getcontext(&parent);   // magic sauce
    swapcontext(&parent, &(RunQ->context));  // start the first thread
}

void yield() // similar to run
{
    // rotate the run Q;
    RotateQ(&RunQ);
    // swap the context, from previous thread to the thread pointed to by runQ
    swapcontext(&(RunQ->previous->context), &(RunQ->context));
}

#endif // THREADS_H_INCLUDED
