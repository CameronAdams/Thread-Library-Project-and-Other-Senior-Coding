#include <cstdlib>
#include <iostream>
#include "thread.h"

#define DEBUG false

int num = 1;
int count = 0;
int stop = 2;
int sleepcount = 1;
unsigned int lock1 = 1;
unsigned int lock2 = 2;
unsigned int lock3 = 3;
unsigned int lock4 = 4;
unsigned int cond1 = 11;
unsigned int cond2 = 12;

void checkErrors() 
{
	if (thread_signal(lock1, cond2)!=0)
    {
        if (DEBUG) std::cout << "handled non-existent condition: wrong" << std::endl;
    }   
    else
    {
        if (DEBUG) std::cout << "signal lock1, cond2: correct" << std::endl;
    }   
    if (thread_broadcast(lock1, cond2)!=0)
    {
        if (DEBUG) std::cout << "handled non-existent condition: wrong" << std::endl;
    }   
    else
    {
        if (DEBUG) std::cout << "broadcast lock1, cond2: correct" << std::endl;
    }
    if (thread_signal(lock4, cond1)!=0)
    {
        if (DEBUG) std::cout << "handled non-existent condition: wrong" << std::endl;
    }    
    else
    {
        if (DEBUG) std::cout << "signal lock4, cond1: correct" << std::endl;
    }    
    if (thread_broadcast(lock4, cond1)!=0)
    {
        if (DEBUG) std::cout << "handled non-existent condition: wrong" << std::endl;
    }    
    else
    {
        if (DEBUG) std::cout << "broadcast lock4, cond1: correct" << std::endl;
    }    
    if (thread_signal(lock2, cond1)!=0)
    {
        if (DEBUG) std::cout << "handled non-existent condition: wrong" << std::endl;
    }    
    else
    {
        if (DEBUG) std::cout << "signal lock2, cond1: correct" << std::endl;
    }    
    if (thread_broadcast(lock2, cond1)!=0)
    {
        if (DEBUG) std::cout << "handled non-existent condition: wrong" << std::endl;
    }    
    else
    {
        if (DEBUG) std::cout << "broadcast lock2, cond1: correct" << std::endl;
	}
}

void one(void *args) 
{
    std::cout << "started 1" << std::endl;
    while (count < stop) 
    {
        thread_lock(lock1);
        sleepcount--;
        checkErrors();
        while (num != 1)
        {
            thread_wait(lock1, cond1);
        }
        sleepcount++;
        std::cout << "1 acquires lock1" << std::endl;
        count++;
        if (DEBUG) std::cout << "num: " << num << " count: " << count << std::endl;
        num = 2;
        thread_broadcast(lock1, cond1);
		std::cout << "1 unlocks lock1 "<< thread_unlock(lock1) << std::endl;
    }
    std::cout << "finished 1" << std::endl;
}

void two(void *args) 
{
    std::cout << "started 2" << std::endl;
    while (count < stop) 
    {
        thread_lock(lock1);
        sleepcount--;
        while (num != 2)
        {
            checkErrors();
            thread_wait(lock1, cond1);
        }
        std::cout << "2 acquires lock1" << std::endl;
        sleepcount++;
        count++;
        if (DEBUG) std::cout << "num: " << num << " count: " << count << std::endl;
        num = 3;
        thread_broadcast(lock1, cond1);
        std::cout << "2 unlocks lock1 "<< thread_unlock(lock1) << std::endl;
    }
    std::cout << "finished 2" << std::endl;
}

void three(void *args) 
{
    std::cout << "started 3" << std::endl;
    while (count < stop) 
    {
        sleepcount--;
        thread_lock(lock1);
        while (num != 3)
        {
            thread_wait(lock1, cond1);
		}
        std::cout << "3 acquires lock1" << std::endl;
        sleepcount++;
        count++;
        if (DEBUG) std::cout << "num: " << num << " count: " << count << std::endl;
        num = 1;
        checkErrors();
        thread_broadcast(lock1, cond1);
		std::cout << "3 unlocks lock1 "<< thread_unlock(lock1) << std::endl;
		checkErrors();
    }
    std::cout << "finished 3" << std::endl;
}

void four(void *args) 
{
    std::cout << "started 4" << std::endl;
    thread_lock(lock2);
    std::cout << "4 acquires lock2" << std::endl;
    thread_lock(lock1);
    std::cout << "4 acquires lock1" << std::endl;
    while (count < stop - 1) 
    {
        thread_wait(lock1, cond1);
    }
    thread_broadcast(lock1, cond1);
    std::cout << "4 unlocks lock1 "<< thread_unlock(lock1) << std::endl;
    std::cout << "4 unlocks lock2 "<< thread_unlock(lock2) << std::endl;
    std::cout << "finished 4" << std::endl;
}

void master(void *args) 
{
	stop = (intptr_t) args;
    thread_create((thread_startfunc_t) four, (void*) "null");
    thread_create((thread_startfunc_t) one, (void*) "null");
    thread_create((thread_startfunc_t) two, (void*) "null");
    thread_create((thread_startfunc_t) three, (void*) "null");
}

int main(int argc, char *argv[]) 
{
    thread_libinit((thread_startfunc_t) master, (void *) 100);
}
