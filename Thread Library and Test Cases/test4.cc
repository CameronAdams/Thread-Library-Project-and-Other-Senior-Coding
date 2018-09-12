#include <iostream>
#include "thread.h"

#define DEBUG true

void master_thread(void*args);
void child_thread(void*args);
void child2_thread(void*args);
void child3_thread(void*args);
void child4_thread(void*args);

int main(int argc, char *argv[])
{
	int number = 999;
	thread_libinit((thread_startfunc_t) master_thread, &number);
}

void master_thread(void * args)
{
	if (DEBUG) std::cout << "master thread created" << std::endl;
	if (DEBUG) std::cout.flush();
	if (DEBUG) std::cout << "number is " << * (int *) args << std::endl;
	if (DEBUG) std::cout.flush();
	thread_create((thread_startfunc_t) child_thread, args);
	thread_yield();
	thread_create((thread_startfunc_t) child2_thread, args);
	thread_yield();
	thread_create((thread_startfunc_t) child3_thread, args);
	thread_yield();
	thread_create((thread_startfunc_t) child4_thread, args);
	if (DEBUG) std::cout << "master thread ended" << std::endl;
	if (DEBUG) std::cout.flush();
}

void child_thread(void * args)
{
	if (DEBUG) std::cout << "child thread created" << std::endl;
	if (DEBUG) std::cout.flush();
}

void child2_thread(void * args)
{
	if (DEBUG) std::cout << "child 2 thread created" << std::endl;
	if (DEBUG) std::cout.flush();
}

void child3_thread(void * args)
{
	if (DEBUG) std::cout << "child 3 thread created" << std::endl;
	if (DEBUG) std::cout.flush();
}

void child4_thread(void * args)
{
	if (DEBUG) std::cout << "child 4 thread created" << std::endl;
	if (DEBUG) std::cout.flush();
}
