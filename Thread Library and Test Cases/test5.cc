#include <cstdlib>
#include <iostream>
#include "thread.h"

#define DEBUG true

int num = 0;
int count = 0;
unsigned int lock = 1;
unsigned int cond = 2;

void loop(void *a) 
{
    char *id;
    int i;
    id = (char *) a;
    if (DEBUG) std::cout << "loop called with id " << (char *) id << std::endl;
    if (DEBUG) std::cout.flush();
    for (i = 0; i < 4; i++, num++) 
    {
        if (DEBUG) std::cout << id << ":\t" << i << "\t" << num << std::endl;
        if (DEBUG) std::cout.flush();
        if (thread_yield()) 
        {
            if (DEBUG) std::cout << "thread_yield failed" << std::endl;
            if (DEBUG) std::cout.flush();
            exit(1);
        }
    }
}

void
loop2(void *a) 
{
   thread_lock(lock);
    count++;
    if (DEBUG) std::cout << " blocked count " << count << std::endl;
    if (DEBUG) std::cout.flush();
    thread_wait(lock, cond);

    if (DEBUG) std::cout << " blocked count " << count << std::endl;
    if (DEBUG) std::cout.flush();
    count--;
   thread_signal(lock,cond);
   thread_unlock(lock);
}

void wake_signal(void*a)
{
    thread_signal(lock,cond);
}

void wake_broadcast(void*a)
{
    thread_broadcast(lock,cond);
}

void
parent(void *a) 
{
    int arg;
    arg = (intptr_t) a;
    if (DEBUG) std::cout << "parent called with arg " << arg << std::endl;
    if (DEBUG) std::cout.flush();
    for (int i = 0; i < 4; i++) {
        if (thread_create((thread_startfunc_t) loop, (void *) "child thread")) 
        {
            if (DEBUG) std::cout << "thread_create failed" << std::endl;
            if (DEBUG) std::cout.flush();
            exit(1);
        }
    }

    loop((void *) "parent thread");

    if (DEBUG) std::cout<<"creating" << std::endl;
    if (DEBUG) std::cout.flush();
    for (int i = 0; i < 4; i++) 
    {
        if (thread_create((thread_startfunc_t) loop2, (void *) "signal thread")) 
        {
            if (DEBUG) std::cout << "thread_create failed" << std::endl;
            if (DEBUG) std::cout.flush();
            exit(1);
        }
    }

    if (DEBUG) std::cout<<"creating threads and broadcast" << std::endl;
    if (DEBUG) std::cout.flush();
    thread_create((thread_startfunc_t) wake_signal, (void *) "wake thread");

    for (int i = 0; i < 4; i++) 
    {
        if (thread_create((thread_startfunc_t) loop2, (void *) "broadcast thread")) 
        {
            if (DEBUG) std::cout << "thread_create failed" << std::endl;
            if (DEBUG) std::cout.flush();
            exit(1);
        }
    }
    thread_create((thread_startfunc_t) wake_broadcast, (void *) "wake thread");
}


int main() 
{
    if (thread_libinit((thread_startfunc_t) parent, (void *) 100)) 
    {
        if (DEBUG) std::cout << "thread_libinit failed" << std::endl;
        if (DEBUG) std::cout.flush();
        exit(1);
    }
}
