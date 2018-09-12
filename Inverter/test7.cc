#include <iostream>       
#include "thread.h"

#define DEBUG true

unsigned int lock;
unsigned int on = 1;
unsigned int off = 0;
unsigned int sum = 0;

void one (void *arg) 
{
	while (sum < 20) 
	{
		thread_lock(lock);
		while(sum == 15) 
		{
			thread_wait(lock, on);
		}
		if (DEBUG) std::cout << "1: " << sum << std::endl;
		if (DEBUG) std::cout.flush();
		sum++;
		thread_signal(lock, off);
		thread_unlock(lock);
	}
	

}

void two (void *arg) 
{
	while (sum < 40) 
	{
		thread_lock(lock);
		while(sum == 31) 
		{
			thread_wait(lock, off);
		}
		if (DEBUG) std::cout << "2: " << sum << std::endl;
		if (DEBUG) std::cout.flush();
		sum++;
		thread_broadcast(lock, on);
		thread_unlock(lock);
	}
}

void three (void *arg) 
{
	while (sum < 40) 
	{
		thread_lock(0);
		while(sum<20) 
		{
			thread_wait(lock, on);
		}
		if (DEBUG) std::cout << "3: " << sum << std::endl;
		if (DEBUG) std::cout.flush();
		sum++;
		thread_signal(lock, off);
		thread_unlock(lock);
	}
}

void start (void *arg) 
{
	if (DEBUG) std::cout << "libinit done" << std::endl;
	if (DEBUG) std::cout.flush();
	thread_create (one, arg);
	if (DEBUG) std::cout << "thread 1 created" << std::endl;
	if (DEBUG) std::cout.flush();
	thread_create (two, arg);
	if (DEBUG) std::cout << "thread 2 created" << std::endl;
	if (DEBUG) std::cout.flush();
	thread_create (three, arg);
	if (DEBUG) std::cout << "thread 3 created" << std::endl;
	if (DEBUG) std::cout.flush();
}

int main (int argc, char* argv[]) 
{
	if (DEBUG) std::cout << "memory" << std::endl;
	if (DEBUG) std::cout.flush();
	thread_libinit(start, (void *) 100);
}
