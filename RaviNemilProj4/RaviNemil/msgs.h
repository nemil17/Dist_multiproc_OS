#include "sem.h"

// Declare a message type. For simplicity, a message is an array of 10 integers.
typedef int Msg[10];

// Declare a port. A port is something that contains N messages, make N=10.
// There would be semaphores associated with the port, for controlling synchronization.
#define N 10
typedef struct Port
{
    Msg msgs[N];
    int in;
    int out;
    Semaphore_t * mutex_sem;
    Semaphore_t * space_sem;
    Semaphore_t * item_sem;
} Port;

// Declare a set (array of ports). The ports are numberered 0 to 99.
Port ports[100];
void Send(Port *p, Msg *m)
{
    int i=0;
    P(p->space_sem);
    P(p->mutex_sem);
    for ( i=0; i<10; i++)
    {
        ((int*)(p->msgs[p->in]))[i] = ((int*)(m))[i];
    }
    p->in = (p->in+1)%N;
    V(p->mutex_sem);
    V(p->item_sem);
}

// The receive is a “blocking” receive.
void Receive(Port * p, Msg * m)
{
    int i=0;
    P(p->item_sem);
    P(p->mutex_sem);
    for ( i=0; i<10; i++)
    {
        ((int*)(m))[i] = ((int*)(p->msgs[p->out]))[i];
    }
    p->out = (p->out+1)%N;
    V(p->mutex_sem);
    V(p->space_sem);
}

// Strategy 2: Same as 1, but a mutex per port.
void InitPort()
{
    int i;
    for (i=0; i<100; i++)
    {
        ports[i].mutex_sem = CreateSem(1);
        ports[i].space_sem = CreateSem(N);
        ports[i].item_sem = CreateSem(0);
        ports[i].in = 0;
        ports[i].out = 0;
    }
//    printf("100 ports are initialized\n");
}


