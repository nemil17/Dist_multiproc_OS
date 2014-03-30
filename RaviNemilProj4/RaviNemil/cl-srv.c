/***
RAVI NAIK: 1205109590
NEMIL TIMBADIA: 1205120341
**/

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "msgs.h"

int server_port = 80;
int string_max_length = 128;


char * client_1_pre_coded_string = "Ubuntu"; // short string
char * client_2_pre_coded_string = "Windows Operating System"; // long string

void str2msg(Msg * m, char * str)
{
    int i=3;
    char *p = str;
    while (*p!='\0' && i<10)
    {
        ((int*)(m))[i]  = *p;
        p++;
        i++;
    }
}
void str2msg2(Msg * m, char * str)
{
    int i=3;
    char *p = str+7;
    while (*p!='\0' && i<10)
    {
        ((int*)(m))[i++]  = (int) (*(p++));
    }
}

void msg2str(char * str, Msg * m)
{
    int i;
    char * p = str;
    for (i=3; i<10; i++, p++)
    {
        *p = (char) ( ((int*)(m))[i] );
    }
}

void msg2str2(char * str, Msg * m)
{
    int i;
    char * p = str;
    while(*p!='\0')
    {
        p++;
    }
    for (i=3; i<10; i++, p++)
    {
        *p = (char) ( ((int*)(m))[i] );
    }
}

/****
3 clients and 1 server
****/
void client1(void)
{
    int port = rand()%(server_port/2);
    while(1)
    {
        Msg msg, rmsg;
        memset(&rmsg, 0, sizeof(msg));
        ((int *)(msg))[0] = port;
        ((int *)(msg))[1] = rand()%4;
        if (3 == ((int *)(msg))[1])
        {
            ((int *)(msg))[1] = 0;
        }
        ((int *)(msg))[2] = 0; // string
        str2msg(&msg, client_1_pre_coded_string);

        if (0 == ((int *)(msg))[1])
        {
            printf("client 1: adding\n");
        }
        else if (1 == ((int *)(msg))[1])
        {
            printf("client 1: deleting\n");
        }
        else if (2 == ((int *)(msg))[1])
        {
            printf("client 1: modifying\n");
        }
        Send(ports+server_port, &msg);
        //
        memset(&rmsg, 0, sizeof(rmsg));
//        printf("client 1: receiving  <--- server\n");
        Receive(ports+port, &rmsg);
        //
        sleep(1);
    }
}

void client2(void)
{
    int port = rand()%(server_port/2) + server_port/2;
    while(1)
    {
        Msg msg1, msg2, rmsg;
        //
        memset(&rmsg, 0, sizeof(msg1));
        memset(&rmsg, 0, sizeof(msg2));
        ((int *)(msg1))[0] = port;
        ((int *)(msg2))[0] = port;
        ((int *)(msg1))[1] = rand()%3;
        if (3 == ((int *)(msg1))[1])
        {
            ((int *)(msg1))[1] = 0;
        }
        ((int *)(msg2))[1] = ((int *)(msg1))[1];
        ((int *)(msg1))[2] = 1; // long string
        ((int *)(msg2))[2] = 2; // long string end
        str2msg(&msg1, client_2_pre_coded_string);
        str2msg2(&msg2, client_2_pre_coded_string);
//        printf("client 2: sending    ---> server\n");
        if (0 == ((int *)(msg1))[1])
        {
            printf("client 2: adding\n");
        }
        else if (1 == ((int *)(msg1))[1])
        {
            printf("client 2: deleting\n");
        }
        else if (2 == ((int *)(msg1))[1])
        {
            printf("client 2: modifying\n");
        }
        Send(ports+server_port, &msg1);
        if (!(1 == ((int *)(msg2))[1]))
        {
            Send(ports+server_port, &msg2);
        }
        //
        memset(&rmsg, 0, sizeof(rmsg));
//        printf("client 2: receiving  <--- server\n");
        Receive(ports+port, &rmsg);
        //
        sleep(1);
    }
}

void client3(void)
{
    char table_cache[10*string_max_length];
    int port = rand()%(100-server_port-1) + server_port + 1;
    while(1)
    {
        Msg msg, rmsg;
        //
        ((int *)(msg))[0] = port;
        ((int *)(msg))[1] = 3;
//        printf("client 3: sending    ---> server\n");
        Send(ports+server_port, &msg);
        //
        memset(&table_cache, 0, sizeof(table_cache));
        do
        {
            memset(&rmsg, 0, sizeof(rmsg));
//            printf("client 3: receiving  <--- server\n");
            Receive(ports+port, &rmsg);
            msg2str2(table_cache, &rmsg);
        }
        while(1==((int *)(rmsg))[2]);
        if (rand()%3==0)
        {
            printf("client 3: printing\n");
            printf("---------------------\n");
            printf("%s", table_cache);
            printf("---------------------\n");
        }
        else
        {
            yield();
        }
        //
        sleep(1);
    }
}

void server(void)
{
    // Server contains a table, 10 entries. Each entry is a string.
    char table[10][string_max_length];
    char table_cache[10*string_max_length];
    int start_pos = 0; // [
    int end_pos = 0; // )
    int count = 0;
    char long_string_cache[2*10];
    //
    while(1)
    {
        Msg msg;
        int i, j;
        memset(&msg, 0, sizeof(msg));
//        printf("                     ---> server: receiving\n");
        Receive(ports+server_port, &msg);
        int client_port = ((int *)(msg))[0];
        // processing
        switch (((int *)(msg))[1])
        {
        case 0: // add
            if (2 == ((int *)(msg))[2]) // long string end
            {
                if (!(end_pos==start_pos && count>0))  // not full
                {
                    msg2str2(long_string_cache+7, &msg);
                    strcpy(table[end_pos], long_string_cache);
                    end_pos = (end_pos+1)%10;
                    count ++;
                    //
                    ((int *)(msg))[1] = 4;
//                    printf("                     <--- server: sending\n");
                    Send(ports+client_port, &msg); // ACK
                }
                else
                {
                    ((int *)(msg))[1] = 5;
//                    printf("                     <--- server: sending\n");
                    Send(ports+client_port, &msg); // NCK
                }
            }
            else if (1 == ((int *)(msg))[2]) // long string
            {
                memset(&long_string_cache, 0, sizeof(long_string_cache));
                msg2str2(long_string_cache, &msg);
            }
            else // short string
            {
                if (!(end_pos==start_pos && count>0))  // not full
                {
                    msg2str(table[end_pos], &msg);
                    end_pos = (end_pos+1)%10;
                    count ++;
                    //
                    ((int *)(msg))[1] = 4;
//                    printf("                     <--- server: sending\n");
                    Send(ports+client_port, &msg); // ACK
                }
                else
                {
                    ((int *)(msg))[1] = 5;
//                    printf("                     <--- server: sending\n");
                    Send(ports+client_port, &msg); // NCK
                }
            }
            break;
        case 1: // delete
            if (count > 0)  // not empty
            {
                start_pos = (start_pos+1)%10;
                count --;
                //
                ((int *)(msg))[1] = 4;
//                printf("                     <--- server: sending\n");
                Send(ports+client_port, &msg); // ACK
            }
            else
            {
                ((int *)(msg))[1] = 5;
//                printf("                     <--- server: sending\n");
                Send(ports+client_port, &msg); // NCK
            }
            break;
        case 2: // modify
            if (2 == ((int *)(msg))[2]) // long string end
            {
                if (count > 0)  // not empty
                {
                    msg2str2(long_string_cache+7, &msg);
                    strcpy(table[end_pos], long_string_cache);
                    //
                    ((int *)(msg))[1] = 4;
//                    printf("                     <--- server: sending\n");
                    Send(ports+client_port, &msg); // ACK
                }
                else
                {
                    ((int *)(msg))[1] = 5;
//                    printf("                     <--- server: sending\n");
                    Send(ports+client_port, &msg); // NCK
                }
            }
            else if (1 == ((int *)(msg))[2]) // long string
            {
                memset(&long_string_cache, 0, sizeof(long_string_cache));
                msg2str2(long_string_cache, &msg);
            }
            else // short string
            {
                if (count > 0)  // not empty
                {
                    msg2str(table[end_pos], &msg);
                    //
                    ((int *)(msg))[1] = 4;
//                    printf("                     <--- server: sending\n");
                    Send(ports+client_port, &msg); // ACK
                }
                else
                {
                    ((int *)(msg))[1] = 5;
//                    printf("                     <--- server: sending\n");
                    Send(ports+client_port, &msg); // NCK
                }
            }
            break;
        default: // reply whole table
            memset(&table_cache, 0, sizeof(table_cache));
            if (count > 0)
            {
                strcpy(table_cache, table[start_pos]);
                table_cache[strlen(table_cache)] = '\n';
                //
                for (i=(start_pos+1)%10, j=strlen(table_cache); i!=end_pos; j=strlen(table_cache), i=(i+1)%10)
                {
                    strcpy(table_cache+j, table[i]);
                    table_cache[strlen(table_cache)] = '\n';
                }
            }

            i = 0;
            j = 0; // long string flag
            do
            {
                memset(&msg, 0, sizeof(msg));
                if (table_cache[i+7]=='\0')
                {
                    ((int *)(msg))[2] = j;
                }
                else
                {
                    ((int *)(msg))[2] = 1;
                    j = 2;
                }
                str2msg(&msg, table_cache+i);
                Send(ports+client_port, &msg); // table
                i = i+7;
            }
            while(!(0==((int *)(msg))[2] || 2==((int *)(msg))[2]));
        }
    }
}

/****
main
****/
int main()
{
        sleep(2);
        printf("\n\n Client 1 either adds a string  to first empty position in the table, updates/deletes the string wit where it first matches. \n");
        sleep(2);

	printf("\n\n Client 1 either adds a string  to first empty position in the table, updates/deletes the string wit where it first matches. \n");

        printf("\n\n Client 3 either prints table after getting all 10 strings from the server\n");

        sleep(1);

	printf("\n\nOur strings are Ubuntu and  Windows Operating System watch out for them!!!!\n");


   



    InitPort();

    start_thread(server);

    start_thread(client1);
    start_thread(client2);
    start_thread(client3);

    run();

    return 0;
}








