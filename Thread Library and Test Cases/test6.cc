#include <iostream>       
#include "thread.h"

#define DEBUG true

void one (void *arg) 
{
	if (DEBUG) std::cout << "1: thread 1: thread_lock" << std::endl;
	if (DEBUG) std::cout.flush();
	thread_lock (0);      
   
	if (DEBUG) std::cout << "2: thread 1: thread_wait" << std::endl;
	if (DEBUG) std::cout.flush();
	thread_wait (0,0);    
  
	if (DEBUG) std::cout << "11: thread 1: thread_unlock" << std::endl;
	if (DEBUG) std::cout.flush();
	thread_unlock (0);    

	if (DEBUG) std::cout << "12: thread 1: done" << std::endl;     
	if (DEBUG) std::cout.flush();
}



void two (void *arg) 
{
	if (DEBUG) std::cout << "3: thread 2: thread_lock" << std::endl;
	if (DEBUG) std::cout.flush();
	thread_lock (0);      
 
	if (DEBUG) std::cout << "4: thread 2: thread_yield" << std::endl;
	if (DEBUG) std::cout.flush();
	thread_yield ();   
 
	if (DEBUG) std::cout << "6: thread 2: thread_unlock" << std::endl;
	if (DEBUG) std::cout.flush();
	thread_unlock (0);    

	if (DEBUG) std::cout << "7: thread 2: done" << std::endl;     
	if (DEBUG) std::cout.flush();
}



void three (void *arg) 
{
	if (DEBUG) std::cout << "5: thread 3: thread_lock" << std::endl;
	if (DEBUG) std::cout.flush();
	thread_lock (0);         
	
	if (DEBUG) std::cout << "8: thread 3: thread_signal" << std::endl;
	if (DEBUG) std::cout.flush();
	//thread_signal(0,0);      
	thread_broadcast(0,0); 

	if (DEBUG) std::cout << "9: thread 3: thread_unlock" << std::endl;
	if (DEBUG) std::cout.flush();
	thread_unlock (0);     

	if (DEBUG) std::cout << "10: thread 3: done" << std::endl;      
	if (DEBUG) std::cout.flush();
}





void start (void *arg) 
{
	thread_create (one, arg);
	thread_create (two, arg);
	thread_create (three, arg);
}

int main (int argc, char* argv[]) 
{
	thread_libinit(start, (void *) 100);
}
