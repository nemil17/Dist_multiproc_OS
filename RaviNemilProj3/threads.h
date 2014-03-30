// Same as Project 1 
#include<stdlib.h>
#include "q.h"
void start_thread(void (*function)(void))
{
	// begin pseudo code
  	// allocate a stack (via malloc) of a certain size (choose 8192)
	void *stack = malloc(8192);
  	// allocate a TCB (via malloc)
	TCB_t *tcb = malloc(sizeof(TCB_t));
  	//   call init_TCB with appropriate arguments
	init_TCB(tcb, function, stack, 8192);
  	//   call addQ to add this TCB into the “RunQ” which is a global header pointer
	AddQ(&RunQ, tcb); // change
  	//end pseudo code
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
   	//rotate the run Q;
	RotateQ(&RunQ);
   	//swap the context, from previous thread to the thread pointed to by runQ
	swapcontext(&(RunQ->previous->context),&(RunQ->context));
}
