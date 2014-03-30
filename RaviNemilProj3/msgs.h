#include "sem.h"
#define N 10

// Declare a port. A port is something that contains N messages, make N=10. There would be s//emaphores associated with the port, for controlling synchronization. The port “contains” m//essages hence a port may be a pointer to a Q of messages, or an array of messages.


// test it with just a single int message and then modify it for the array of message
int mess = 10;

//Declare a message type. For simplicity, a message is an array of 10 integers.
typedef int Message[10];
typedef struct Port
{
	Message message[N];
	Semaphore_t * mutex;
	Semaphore_t *open;
	Semaphore_t *close;
	int in;
	int out;
}Port;

//Declare a set (array of ports). The ports are numberered 0 to 99.
Port ports[100];

void printMessage(Message *m)
{
	int i;
	for(i=0;i<N;i++)
	{
		printf("%d \t", ((int *)(m))[i]);
	}
	printf("\n");
}

// Send (with appropriate arguments). The send is a “blocking” asynchronous send that is: it// places the message in the port and returns but blocks if port is full.
void Send(Port *p, Message *m)
{
	printf("\n\tSending the Message to Port %d\n", p-ports);
	printMessage(m);
        P(p->open);
        P(p->mutex);
        // printf("Sender %d: Sending item#%d, into port #%d\n", *arg, item_num, in);
        // buffer[in] = item_num++; in = (in+1) % N;
	memcpy(p->message[p->in], m, 10*sizeof(int));
	p->in = ((p->in) + 1)%N;
        V(p->mutex);
        V(p->close);
        
}

//Receive (with appropriate arguments). The receive is a “blocking” receive.
void Receive(Port *p, Message *m)
{
	//printf(" Consumer ready to consume\n");
        P(p->close);
        P(p->mutex);
	memcpy(m, p->message[p->out], 10*sizeof(int));
        printf("\n\tReceived Message at Port  Number %d\n", p-ports, p->out);
        p->out = ((p->out)+1) % N;
        V(p->mutex);
        V(p->open);
        //sleep(cons_delay);

}

// Initialize the ports
void InitPorts()
{
	int i;
	for(i = 0; i < 100;i++)
	{
		ports[i].mutex = CreateSem(1);
		ports[i].open = CreateSem(N);// length of message is 10
		ports[i].close = CreateSem(0);
		ports->in = 0;
		ports->out = 0;
	}
}

