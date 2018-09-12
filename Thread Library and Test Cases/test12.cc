#include <cstdlib>
#include <iostream>
#include "thread.h"

#define DEBUG true
#define DEBUGelse false

int num = 1;
int count = 0;
int stop = 11;
unsigned int lock1 = 1;
unsigned int lock2 = 2;
unsigned int lock3 = 3;
unsigned int cond1 = 11;
unsigned int cond2 = 12;

void checkErrors() 
{
    if (thread_lock(lock1) == 0)
    {
        if (DEBUG) std::cout << "correct" << std::endl;
    }   
    else
    {
        if (DEBUGelse) std::cout << "lock while locked: wrong" << std::endl;
    }
    if (thread_unlock(lock2) == 0)
    {
        if (DEBUG) std::cout << "correct" << std::endl;
    }
    else   
    {
        if (DEBUGelse) std::cout << "unlocked lock of lock 4: wrong" << std::endl;
    }
    if (thread_unlock(lock3) == 0)
    {
        if (DEBUG) std::cout << "correct" << std::endl;
    }
    else   
    {
        if (DEBUGelse) std::cout << "unlocked lock of lock 4: wrong" << std::endl;
    }
    if (thread_wait(lock3, cond2) == 0)
    {
        if (DEBUG) std::cout << "correct" << std::endl;
    }
    else   
    {
        if (DEBUGelse) std::cout << "no lock: wrong" << std::endl;
    }
    if (thread_wait(lock2, cond2) == 0)
    {
        if (DEBUG) std::cout << "correct" << std::endl;
    }
    else   
    {
        if (DEBUGelse) std::cout << "no lock: wrong" << std::endl;
    }
    if (thread_wait(lock3, cond1) == 0)
    {
        if (DEBUG) std::cout << "correct" << std::endl;
    }
    else   
    {
        if (DEBUGelse) std::cout << "no lock: wrong" << std::endl;
    }
    if (thread_wait(lock2, cond1) == 0)
    {
        if (DEBUG) std::cout << "correct" << std::endl;
    }
    else   
    {
        if (DEBUGelse) std::cout << "no lock: wrong" << std::endl;
    }
    if (thread_signal(lock1, cond2)!=0)
    {
        if (DEBUG) std::cout << "correct" << std::endl;
    }
    else   
    {
        if (DEBUGelse) std::cout << "no condition: wrong" << std::endl;
    }
    if (thread_broadcast(lock1, cond2)!=0)
    {
        if (DEBUG) std::cout << "correct" << std::endl;
    }
    else   
    {
        if (DEBUGelse) std::cout << "no condition: wrong" << std::endl;
    }
    if (thread_signal(lock3, cond1)!=0)
    {
        if (DEBUG) std::cout << "correct" << std::endl;
    }
    else   
    {
        if (DEBUGelse) std::cout << "no condition: wrong" << std::endl;
    }
    if (thread_broadcast(lock3, cond1)!=0)
    {
        if (DEBUG) std::cout << "correct" << std::endl;
    }
    else   
    {
        if (DEBUGelse) std::cout << "no condition: wrong" << std::endl;
    }
    if (thread_signal(lock2, cond1)!=0)
    {
        if (DEBUG) std::cout << "correct" << std::endl;
    }
    else   
    {
        if (DEBUGelse) std::cout << "no condition: wrong" << std::endl;
    }
    if (thread_broadcast(lock2, cond1)!=0)
    {
        if (DEBUG) std::cout << "correct" << std::endl;
    }
    else   
    {
        if (DEBUGelse) std::cout << "no condition: wrong" << std::endl;
    }
}

void one(void *args) 
{
    std::cout << "started 1" << std::endl;
    while (count < stop) 
    {
        std::cout << "thread lock: " << thread_lock(lock1) << std::endl;
        checkErrors();
        while (num != 1)
        {
            thread_wait(lock1, cond1);
        }
        std::cout << "1 acquires lock1" << lock1 << std::endl;
        count++;
        std::cout << "num: " << num << " count: " << count << std::endl;
        num = 2;
        thread_broadcast(lock1, cond1);
        thread_unlock(lock1);
    }
    std::cout << "finished 1" << std::endl;
}

void two(void *args) 
{
    std::cout << "started 2" << std::endl;
    while (count < stop) 
    {
        std::cout<< "thread lock: " << thread_lock(lock1) << std::endl;
        while (num != 2)
        {
            checkErrors();
            thread_wait(lock1, cond1);
        }
        count++;
    	std::cout << "num: " << num << " count: " << count << std::endl;
        num = 3;
        thread_broadcast(lock1, cond1);
        thread_unlock(lock1);
    }
    std::cout << "finished 2" << std::endl;
}

void three(void *args) 
{
    std::cout << "started 3" << std::endl;
    while (count < stop) 
    {
        std::cout << "thread lock: "<<thread_lock(lock1) << std::endl;
        while (num != 3)
        {
            thread_wait(lock1, cond1);
		}
        std::cout << "three acquires lock" << lock1<< std::endl;
        checkErrors();
        count++;
        std::cout << "num: " << num << " count: " << count << std::endl;
        num = 1;
        thread_broadcast(lock1, cond1); 
        thread_unlock(lock1);
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
    thread_unlock(lock1);
    thread_unlock(lock2);
    std::cout << "four finished" << std::endl;
}

void master(void* args) 
{
    stop = (intptr_t) args;
    thread_create((thread_startfunc_t) one, (void*) "null");
    thread_create((thread_startfunc_t) two, (void*) "null");
    thread_create((thread_startfunc_t) three, (void*) "null");
    thread_create((thread_startfunc_t) four, (void*) "null");
}

int main(int argc, char *argv[]) 
{
    thread_libinit((thread_startfunc_t) master, (void *) 100);
}
