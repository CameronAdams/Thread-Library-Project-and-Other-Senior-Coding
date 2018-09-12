#include <iostream>      
#include "thread.h"

#define DEBUG true

unsigned int on = 1;
unsigned int off = 0;
unsigned int lock;
unsigned int sum = 0;

void one (void *arg) 
{
	while (sum < 20) 
	{
		thread_lock(lock);
		if (DEBUG) std::cout << "call thread lock twice" << std::endl;  
		thread_lock(lock) == -1;
		while(sum%2 == 0) 
		{
			thread_wait(lock, on);
		}
		if (DEBUG) std::cout << "1: " << sum << std::endl;
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
		while(sum%2 == 1) 
		{
			thread_wait(lock, off);
		}
		if (DEBUG) std::cout << "2: " << sum << std::endl;
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
		while(sum%2 == 0 || sum<20) 
		{
			thread_wait(lock, on);
		}
		if (DEBUG) std::cout << "3: " << sum << std::endl;
		sum++;
		thread_signal(lock, off);
		thread_unlock(lock);
	}
}

void four (void *arg) 
{
	while (sum < 20) 
	{
		thread_lock(lock);
		if (DEBUG) std::cout << "call thread lock twice" << std::endl;  
		thread_lock(lock) == -1;
		while(sum%2 == 0) 
		{
			thread_wait(lock, on);
		}
		if (DEBUG) std::cout << "1: " << sum << std::endl;
		sum++;
		thread_signal(lock, off);
		thread_unlock(lock);
	}
}

void start (void *arg) 
{
	thread_create (one, arg);
	thread_create (two, arg);
	thread_create (three, arg);
	thread_create (four, arg);
}

int main (int argc, char* argv[]) 
{
	thread_libinit(start, (void *) 100);
}
