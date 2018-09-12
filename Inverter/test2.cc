#include <iostream>
#include "thread.h"

#define DEBUG true 

int const mutexID = 1; 
int count = 0;

void child(void *args);
void tester(void *args);
int delay(int delay);

int delay(int delay)
{
	int sum = 0;
	for(int i=0; i<delay*1000; i++)
	{
		sum += i;
	} 
	return sum;
}

void child(void *a)
{
	//int threadID = *((int*) a); 
	int threadID = count++;
	if (DEBUG) std::cout << "threadID: " << threadID << std::endl; 
	for(int i=0; i < 3; i++)
	{
		thread_lock(mutexID);
		if (DEBUG) std::cout << "Thread: " << threadID << " lock: " << i << std::endl;
		if (DEBUG) std::cout.flush();
		delay(10000); 
		if (DEBUG) std::cout << "Thread: " << threadID << " unlock: " << i << std::endl;
		if (DEBUG) std::cout.flush();
		thread_unlock(mutexID);
	} 
}

void tester(void *a)
{
	int numThreads = *((int*) a);
	if (DEBUG) std::cout << "numThreads: " << numThreads << std::endl; 
	thread_lock(mutexID);
	for(int i=0; i < numThreads; i++)
	{
		if (DEBUG) std::cout << "i: " << i << std::endl; 
		int r = i;
		if (DEBUG) std::cout << "r: " << r << std::endl; 
		thread_create((thread_startfunc_t) child, (void *) 0);
		thread_yield();
	}
	delay(10000);
	thread_unlock(mutexID); 
}

int main(int argc, char *argv[])
{
	int children = 10;
	thread_libinit((thread_startfunc_t) tester, (void *) &children);
}
