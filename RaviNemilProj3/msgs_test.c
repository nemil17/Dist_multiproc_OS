/*
Ravi Naik : 1205109590
Nemil Timbadia : 1205120341
*/

#include<stdio.h>
#include "msgs.h"
#define N 10
#define MESSAGE_NUMBER 200
#define CLIENT_PORTS 25
#define SERVER_PORT 80

// Builds the message that needs to be sent
void buildMessage(Message *m)
{
	int i;
	for(i=1;i<N;i++)
	{
		// take any random number between 0 and 200 in the message
		((int *)(m))[i] = rand() % MESSAGE_NUMBER;
	}
}


void client1()
{
	while(1)
	{
		Message send_msg;
		Message rcv_msg;
		// build the message to be sent
		int fromPort = rand()%CLIENT_PORTS;
		((int *)send_msg)[0] = fromPort;
		buildMessage(&send_msg);
		printf("\n\tClient 1 is sending a message from port %d to server port %d \n", fromPort, SERVER_PORT);
		Send(&ports[SERVER_PORT], &send_msg);
		memset(&rcv_msg, 0, sizeof(rcv_msg));
		printf("\n\tClient 1 is receiving a message from the server port %d at its port %d \n",SERVER_PORT, fromPort );
		Receive(&ports[fromPort],&rcv_msg);
		sleep(1);
	}
}

void client2()
{
        while(1)
        {
                Message send_msg;
                Message rcv_msg;
                // build the message to be sent
                int fromPort = rand()%CLIENT_PORTS;
                ((int *)send_msg)[0] = fromPort;
		buildMessage(&send_msg);
                printf("\n\tClient 2 is sending a message from port %d to server port %d \n", fromPort, SERVER_PORT);
                Send(&ports[SERVER_PORT], &send_msg);
                memset(&rcv_msg, 0, sizeof(rcv_msg));
                printf("\n\tClient 2 is receiving a message from the server port %d at its port %d\n",SERVER_PORT, fromPort);
                Receive(&ports[fromPort],&rcv_msg);
                sleep(1);
        }       
}


void client3()
{
        while(1)
        {
                Message send_msg;
                Message rcv_msg;
                // build the message to be sent
                int fromPort = rand()%CLIENT_PORTS;
                ((int *)send_msg)[0] = fromPort;
                buildMessage(&send_msg);
                printf("\n\tClient 3 is sending a message from port %d to server port %d \n", fromPort, SERVER_PORT);
                Send(&ports[SERVER_PORT], &send_msg);
                memset(&rcv_msg, 0, sizeof(rcv_msg));
                printf("\n\tClient 3 is receiving a message from the server port %d at its port %d\n",SERVER_PORT, fromPort);
                Receive(&ports[fromPort],&rcv_msg);
                sleep(1);
        }
}
               



void server()
{
	while(1)
	{	
		//opposite of client
		Message rcvd_msg;
		Message send_msg;
		memset(&rcvd_msg, 0, sizeof(rcvd_msg));
		printf("\n\tServer is receiving messages on Port %d\n", SERVER_PORT);
		Receive(&ports[SERVER_PORT], &rcvd_msg);
		memcpy(&send_msg, &rcvd_msg, sizeof(rcvd_msg));
		int toPort = send_msg[0];
		printf("\n\tServer is sending a message from port %d", SERVER_PORT);
		Send(&ports[toPort],&send_msg);
		sleep(1);
	}
}


int main()
{
	InitPorts();

	start_thread(server);
	start_thread(client1);
	start_thread(client2);
	//start_thread(client3);

	
	run();
	return 0;
}
