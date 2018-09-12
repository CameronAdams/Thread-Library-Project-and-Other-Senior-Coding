#include <assert.h>
#include <cstdlib>
#include <iostream>
#include "thread.h"

#define DEBUG true 

int g=0;

void
loop(void *a)
{
    char *id;
    int i;

    id = (char *) a;
    std::cout <<"loop called with id " << (char *) id << std::endl;

    for (i=0; i<5; i++, g++) 
    {
		std::cout << id << ":\t" << i << "\t" << g << std::endl;
		if (thread_yield()) 
		{
	   		std::cout << "thread_yield failed" << std::endl;
	    	exit(1);
		}
    }
}

void
parent(void *a)
{
    int arg;
    arg = (intptr_t) a;
    std::cout << "parent called with arg " << arg << std::endl;
    if (thread_create((thread_startfunc_t) loop, (void *) "child thread")) 
    {
		std::cout << "thread_create failed" << std::endl;
        exit(1);
    }
    loop( (void *) "parent thread");
}

int
main()
{
    if (thread_libinit( (thread_startfunc_t) parent, (void *) 100)) 
    {
	std::cout << "thread_libinit failed" << std::endl;
        exit(1);
    }
}
