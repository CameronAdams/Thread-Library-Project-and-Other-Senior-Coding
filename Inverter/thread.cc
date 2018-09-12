#include <cstdlib>
#include <deque>
#include <iostream>
#include <iterator>
#include <map>
#include <ucontext.h>
#include "interrupt.h"
#include "thread.h"

#define DEBUG false

struct thread 
{
  bool finished;
  char* stack;
  ucontext_t* ucontext_ptr;
  unsigned int id;
};

struct mutex
{
  std::deque<thread*>* blocked_threads;
  thread* owner;
};

typedef std::map<unsigned int, std::deque<thread*>*>::const_iterator condIterator;
typedef std::map<unsigned int, mutex*>::const_iterator mutexIterator;
static std::map<unsigned int, std::deque<thread*>*> condMap;
static std::map<unsigned int, mutex*> mutexMap;

static std::deque<thread*> readyDeque;

static thread* currentThread;
static bool initialized = false;
static int identification = 0;
static ucontext_t* scheduler;

bool interruptsEnabled = 1;

static void enableInterrupts() 
{
	interrupt_enable();
	if(!interruptsEnabled) 
	{
		if (DEBUG) std::cout << "Good" << std::endl;
		if (DEBUG) std::cout.flush();
		interruptsEnabled = true;
	}
	else 
	{
		if (DEBUG) std::cout << "Re-check" << std::endl;
		if (DEBUG) std::cout.flush();
	}
}

static void disableInterrupts() 
{
	interrupt_disable();
	if(interruptsEnabled) 
	{
		if (DEBUG) std::cout << "Good" << std::endl;
		if (DEBUG) std::cout.flush();
		interruptsEnabled = false;
	}
	else 
	{
		if (DEBUG) std::cout << "Re-check" << std::endl;
		if (DEBUG) std::cout.flush();
	}
}

static int kernelSwap(thread_startfunc_t func, void *arg) 
{
  enableInterrupts();
  func(arg);
  disableInterrupts();

  currentThread->finished = 1;
  swapcontext(currentThread->ucontext_ptr, scheduler);
  return 0;
}

void printdeque(std::deque<thread*> deque, std::string name) 
{
	std::deque<thread*> q = deque;
	if (DEBUG) std::cout << name << " " ;
	while (!q.empty()) 
	{
		if (DEBUG) std::cout << q.front()->id << ", ";	
		q.pop_front();
	}
	if (DEBUG) std::cout << std::endl;
}

int thread_libinit(thread_startfunc_t func, void *arg) 
{
	if (DEBUG) std::cout << "Entered thread_libinit" << std::endl;	
	if (DEBUG) std::cout.flush();
  	if (initialized) return -1;
  	initialized = true;

  	try 
  	{
   		scheduler = new ucontext_t;
  	} 
  	catch (std::bad_alloc b) 
  	{
    	delete scheduler;
    	return -1;
  	}
  	if (DEBUG) std::cout << "thread_libinit Interrupt Disable" << std::endl;	
	if (DEBUG) std::cout.flush();
  	getcontext(scheduler);
  	if (DEBUG) std::cout << "thread_libinit getcontext" << std::endl;	
	if (DEBUG) std::cout.flush();
 	if (thread_create(func, arg) < 0)
 	{
    	return -1;
	}
  	disableInterrupts();
  	currentThread = readyDeque.front();
  	readyDeque.pop_front();
  	swapcontext(scheduler, currentThread->ucontext_ptr);

  	while (!readyDeque.empty()) 
  	{
    	if (currentThread->finished == 1)
    	{
			delete currentThread->stack;
	  		currentThread->ucontext_ptr->uc_stack.ss_sp = NULL;
	  		currentThread->ucontext_ptr->uc_stack.ss_size = 0;
	  		currentThread->ucontext_ptr->uc_stack.ss_flags = 0;
	  		currentThread->ucontext_ptr->uc_link = NULL;
	  		delete currentThread->ucontext_ptr;
	 		delete currentThread;
		  	currentThread = NULL;
		}
		currentThread = readyDeque.front();
		readyDeque.pop_front();
		swapcontext(scheduler, currentThread->ucontext_ptr);
  	}

  	if (currentThread != NULL) 
  	{
    	delete currentThread->stack;
  		currentThread->ucontext_ptr->uc_stack.ss_sp = NULL;
  		currentThread->ucontext_ptr->uc_stack.ss_size = 0;
  		currentThread->ucontext_ptr->uc_stack.ss_flags = 0;
  		currentThread->ucontext_ptr->uc_link = NULL;
  		delete currentThread->ucontext_ptr;
 		delete currentThread;
    	currentThread = NULL;
  	}

  	std::cout << "Thread library exiting." << std::endl;
  	if (DEBUG) std::cout.flush();
  	exit(0);
}

int thread_create(thread_startfunc_t func, void *arg) 
{
  	if (DEBUG) std::cout << "Entered thread_create" << std::endl;
	if (DEBUG) std::cout.flush();	
  	if (!initialized) return -1;
  	disableInterrupts();
  
  	thread* newThread;
 	try 
 	{
   		newThread = new thread;
    
		newThread->ucontext_ptr = new ucontext_t;
		getcontext(newThread->ucontext_ptr);
		
		newThread->stack = new char [STACK_SIZE];
		newThread->ucontext_ptr->uc_stack.ss_sp = newThread->stack;
		newThread->ucontext_ptr->uc_stack.ss_size = STACK_SIZE;
		newThread->ucontext_ptr->uc_stack.ss_flags = 0;
		newThread->ucontext_ptr->uc_link = NULL;
		makecontext(newThread->ucontext_ptr, (void (*)())kernelSwap, 2, func, arg);
		
		newThread->id = identification;
		newThread->finished = 0;
		readyDeque.push_back(newThread);
		identification++;
  	}
  	catch (std::bad_alloc b) 
  	{
		delete newThread->ucontext_ptr;
		delete newThread->stack;
		delete newThread;
		enableInterrupts();
		return -1;
  	}
 	enableInterrupts();
 	
 	// possible yield location (gives child thread a chance to run, before parent exits) 
 	//thread_yield();
 	
	return 0;
}

int thread_yield(void) 
{
	if (DEBUG) std::cout << "Entered thread_yield" << std::endl;	
	if (DEBUG) std::cout.flush();
 	if (!initialized) return -1;
  	disableInterrupts();
	
	// check if current thread and scheduler are not null (we are properly initialized)
	if(scheduler!= NULL && currentThread != NULL)
	{
		// push back current thread
  		readyDeque.push_back(currentThread);
		// then swap context
  		swapcontext(currentThread->ucontext_ptr, scheduler);
	}
	else
	{
		if(scheduler== NULL) if (DEBUG) std::cout << "scheduler: NULL" << std::endl;	
		if(currentThread == NULL) if (DEBUG) std::cout << "curentThread: NULL" << std::endl;		
		if (DEBUG) std::cout.flush();
	}
	
  	if (DEBUG) std::cout << "After swapcontext" << std::endl;	
	if (DEBUG) std::cout.flush();
  	enableInterrupts();
  	if (DEBUG) std::cout << "After Enable in Yield" << std::endl;	
	if (DEBUG) std::cout.flush();
  	return 0;
}

int thread_lock(unsigned int lock) {
  	if (DEBUG) std::cout << "Entered thread_lock" << std::endl;	
	if (DEBUG) std::cout.flush();
  	if (!initialized) return -1;

  	disableInterrupts();
  	mutex* newmutex;
  	if (mutexMap.find(lock) == mutexMap.end())  
  	{ 
   		try 
    	{
      		newmutex= new mutex;
      		newmutex->owner = currentThread;
      		newmutex->blocked_threads = new std::deque<thread*>;
    	}    
    	catch (std::bad_alloc b) 
    	{
		  	delete newmutex;
		  	enableInterrupts();
		  	return -1;
    	}
    	mutexMap.insert(std::make_pair(lock, newmutex));
  	} 
  
  	else  
  	{
  		mutexIterator newmutexIterator = mutexMap.find(lock);
    	newmutex= (*newmutexIterator).second;
    	if (newmutex->owner != NULL) 
    	{
      		if (newmutex->owner->id == currentThread->id) 
      		{
        		enableInterrupts();
        		return -1;
      		}      
      		else  
      		{ 
        		newmutex->blocked_threads->push_back(currentThread);
        		swapcontext(currentThread->ucontext_ptr, scheduler);
      		}
    	}
    	else 
    	{ 
      		newmutex->owner = currentThread;
    	}	
	}
  	enableInterrupts();
  	return 0;
}

int thread_unlock(unsigned int lock) 
{
	if (DEBUG) std::cout << "Entered thread_unlock" << std::endl;	
	if (DEBUG) std::cout.flush();
  	if (!initialized) return -1;
  	
  	mutex* newmutex;
  	if (mutexMap.find(lock) == mutexMap.end()) 
  	{ 
    	return -1;
  	}	
  	else 
  	{ 
    	mutexIterator newmutexIterator = mutexMap.find(lock);
    	newmutex= (*newmutexIterator).second;
    
    	if (newmutex->owner != NULL) 
    	{ 
      
      		if (newmutex->owner->id == currentThread->id) 
      		{
        		if (newmutex->blocked_threads->size() > 0) 
        		{
          			newmutex->owner = newmutex->blocked_threads->front();
          			newmutex->blocked_threads->pop_front();
          			readyDeque.push_back(newmutex->owner);
          			
          			// another potential location for yields
          			//enableInterrupts();
          			//thread_yield();
        		} 
        		else
        		{
          			newmutex->owner = NULL;
        		}
      		}
      
      		else 
      		{
        		return -1;
      		}
    	}
    	else  
    	{
     		return -1;
    	}
	}
  	return 0;
}

int thread_wait(unsigned int lock, unsigned int cond) 
{
  	if (DEBUG) std::cout << "Entered thread_wait" << std::endl;	
	if (DEBUG) std::cout.flush();
  	if (!initialized) return -1;

  	disableInterrupts();
  	if (thread_unlock(lock) == 0) 
  	{
    	if (condMap.find(cond) == condMap.end()) 	
    	{  
      		std::deque<thread*>* waiting_threads;
      		try 
      		{
        		waiting_threads = new std::deque<thread*>;
      		} 
      		catch (std::bad_alloc b) 
      		{
       			delete waiting_threads;
        		enableInterrupts();
        		return -1;
      		}
      		waiting_threads->push_back(currentThread);
      		condMap.insert(make_pair(cond, waiting_threads));
    	} 
    	else
    	{
    		condIterator newCondIterator = condMap.find(cond);
    		(*newCondIterator).second->push_back(currentThread);
		}
    	swapcontext(currentThread->ucontext_ptr, scheduler);
    	enableInterrupts();
    	return thread_lock(lock);
  	}
  	enableInterrupts();
  	return -1;
}

int thread_signal(unsigned int lock, unsigned int cond) 
{
  	if (DEBUG) std::cout << "Entered thread_signal" << std::endl;
	if (DEBUG) std::cout.flush();	
  	if (!initialized) return -1;

  	disableInterrupts();
  	condIterator newCondIterator = condMap.find(cond);
  	
  	if (condMap.find(cond) != condMap.end())  
  	{ 
    	std::deque<thread*>* c = (*newCondIterator).second;
    	
    	if (!c->empty())  
    	{ 
      		readyDeque.push_back(c->front());
      		c->pop_front();
    	}
  	}
	enableInterrupts();
	
	// another possible yield location
	//thread_yield();
 	return 0;
}

int thread_broadcast(unsigned int lock, unsigned int cond) 
{
  	if (DEBUG) std::cout << "Entered thread_broadcast" << std::endl;	
	if (DEBUG) std::cout.flush();
  	if (!initialized) return -1;

  	disableInterrupts();
  	condIterator newCondIterator = condMap.find(cond);

  	if (condMap.find(cond) != condMap.end()) 
  	{ 
    	std::deque<thread*>* c = (*newCondIterator).second;
    	while (c->size() > 0) 
    	{  
      		readyDeque.push_back(c->front());
      		c->pop_front();
    	}	
  	}
  	enableInterrupts();
  	
  	// another possible yield location
  	//thread_yield();
  	
  	return 0;
}
