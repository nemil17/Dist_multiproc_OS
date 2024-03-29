#include<memory.h>
#include<ucontext.h>
// A TCB_t is a structure that contains a next pointer, a previous pointer and a data item of the type “ucontext_t”.
typedef struct TCB_t
{
	struct TCB_t *previous;
	struct TCB_t *next;
	ucontext_t context;
}TCB_t;

void init_TCB (TCB_t *tcb, void *function, void *stackP, int stack_size)
// arguments to init_TCB are
//   1. pointer to the function, to be executed
//   2. pointer to the thread stack
//   3. size of the stack
{
	memset(tcb, 0, sizeof(TCB_t)); // wash, rinse
	getcontext(&tcb->context); // have to get parent context, else snow forms on hell
	tcb->context.uc_stack.ss_sp = stackP;
	tcb->context.uc_stack.ss_size = (size_t) stack_size;
	makecontext(&tcb->context, function, 0);// context is now cooked

}
