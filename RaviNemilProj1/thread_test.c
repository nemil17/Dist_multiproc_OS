/*
Ravi Naik : 1205109590
Nemil Timbadia : 1205120341
*/ 
#include<stdio.h>
#include "threads.h"
int global = 0;
void f1()
{
	int local = 0;
	//infinite loop
	while(1)
	{
		// print statements
		printf("XXXXPrinting from Function 1 global = %d local = %d\n", global, local);
		sleep(1);
		global ++;
		local++;
		printf("Function 1 incremented .... global = %d local = %d\n", global, local);
		sleep(1);
		//yield()
		yield();
	}
}
void f2()
{
	int local = 0;
	//infinite loop
	while(1)
	{
		//print statements
		printf("XXXXPrinting from Function 2 global = %d local = %d\n", global, local);
		sleep(1);
		global++;
		local++;
		printf("Function 2 incremented .... global = %d local = %d\n", global, local);
		sleep(1);
		//yield()
		yield();
		
	}
}
void f3()
{
	int local = 0;
	//infinite loop
	while(1)
	{
		//print statements
		printf("XXXXPrinting from Function 3 global = %d local = %d\n", global, local);
		sleep(1);
		global++;
		local++;
		printf("Function 3 incremented .... global = %d local = %d\n", global, local);
		// yield()
		yield();
	}
}

int main()
{
	InitQ();
	//infinite loop
	while(1)
	{
		// start the threads
		start_thread(f1);
		start_thread(f2);
		start_thread(f3);
		run();
	}
	return 0;
}

