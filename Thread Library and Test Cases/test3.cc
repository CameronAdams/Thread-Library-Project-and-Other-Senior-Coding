#include <iostream>
#include "thread.h"

#define DEBUG true

unsigned int lock1 = 1;
unsigned int cond = 1;

void parent_thread(void* args);
void child_thread1(void* args);
void child_thread2(void* args);
void child_thread3(void* args);
void child_thread4(void* args);

void test_init(int (*func)(unsigned int)) {
    if (func(lock1))
        std::cout << "not init: correct" << std::endl;
    else
        std::cout << "uninitialized: wrong" << std::endl;
}

void test_init2(int (*func)(unsigned int, unsigned int)) {
    if (func(lock1, cond))
        std::cout << "not init: correct" << std::endl;
    else
        std::cout << "uninitialized: wrong" << std::endl;
}

void test_init3(int (*func)(void)) {
    if (func())
        std::cout << "not init: correct" << std::endl;
    else
        std::cout << "uninitialized: wrong" << std::endl;
}

void test_method_initialization() {
    test_init(&thread_lock);
    test_init(&thread_unlock);
    test_init2(&thread_wait);
    test_init2(&thread_signal);
    test_init2(&thread_broadcast);
    test_init3(&thread_yield);
}


void parent_thread(void * args)
{
	if (DEBUG) std::cout << "parent thread create" << std::endl;
	if (DEBUG) std::cout.flush();
	thread_create((thread_startfunc_t) child_thread1, (void *) 999);
	thread_create((thread_startfunc_t) child_thread2, (void *) 999);
	thread_create((thread_startfunc_t) child_thread3, (void *) 999);
	thread_create((thread_startfunc_t) child_thread4, (void *) 999);
	if (DEBUG) std::cout << "parent thread yield" << std::endl;
	if (DEBUG) std::cout.flush();
	thread_yield();
	if (DEBUG) std::cout << "parent thread resume" << std::endl;
	if (DEBUG) std::cout.flush();
}

void child_thread1(void * args)
{
	if (DEBUG) std::cout << "child thread 1 yield" << std::endl;
	if (DEBUG) std::cout.flush();
	thread_yield();
	if (DEBUG) std::cout << "child thread 1 resume" << std::endl;
	if (DEBUG) std::cout.flush();
}

void child_thread2(void * args)
{
	if (DEBUG) std::cout << "child thread 2 yield" << std::endl;
	if (DEBUG) std::cout.flush();
	thread_yield();
	if (DEBUG) std::cout << "child thread 2 resume" << std::endl;
	if (DEBUG) std::cout.flush();
}

void child_thread3(void * args)
{
	if (DEBUG) std::cout << "child thread 3 yield" << std::endl;
	if (DEBUG) std::cout.flush();
	thread_yield();
	if (DEBUG) std::cout << "child thread 3 resume" << std::endl;
	if (DEBUG) std::cout.flush();
}

void child_thread4(void * args)
{
	if (DEBUG) std::cout << "child thread 4 yield" << std::endl;
	if (DEBUG) std::cout.flush();
	thread_yield();
	if (DEBUG) std::cout << "child thread 4 resume" << std::endl;
	if (DEBUG) std::cout.flush();
}

void reinit_thread(void * args) {
    std::cout << "ERROR: Initialized threadlib twice!!" << std::endl;
}

int main(int argc, char *argv[])
{
	test_method_initialization();
	thread_libinit((thread_startfunc_t) parent_thread, (void *) 999);
	thread_libinit((thread_startfunc_t) reinit_thread, (void *) 100);
}



