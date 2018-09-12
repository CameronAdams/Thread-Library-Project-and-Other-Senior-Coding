#include <algorithm>
#include <cstdlib>
#include <climits> 
#include <fstream>
#include <iostream>
#include <vector>
#include "thread.h"

#define DEBUG true

using namespace std;

// globals
int max_queue_size;

// the mutex for the disk scheduler
const unsigned int DISK_LOCK = 0;

// list of conditional variables
const unsigned int IS_NOT_EMPTY_COND = 0;
const unsigned int IS_NOT_FULL_COND  = 1;

// current number of registered recievers
int active_requesters = 0;
int total_requesters;

// struct
struct requester_info
{
	int id; 
	int condition; 
	string filename;
};

struct entry
{
	int requester_id;
	int condition;
	int track;
};

vector<entry> queue;

void requester(void *a);
void server(void *a);
void create(void *a[]);

void create(void *a[])
{	
	start_preemptions(true, true, 10);
	
	active_requesters = total_requesters;
	
	// create server thread
	thread_create((thread_startfunc_t) server, NULL);
	
	// create requester threads
	for (int count=0; count < total_requesters; count++)
	{
		
		requester_info* info = new requester_info();
		
		info->id = count;
		info->condition = count + 2;  // a unique int to wait for
		info->filename = (char *) a[count];
		thread_create((thread_startfunc_t) requester,  info);
	}
}		
		
void server(void *a)
{
	int last_track = 0;	
	
	if (DEBUG) cout << "Active_requesters: " << active_requesters << endl;
	if (DEBUG) cout << "queue.size(): " << queue.size() << endl;
	std::cout.flush();
	
	// main server loop
	while(active_requesters > 0 || queue.size() > 0)
	{			
		if (DEBUG) cout << "serverloop" << endl;
		if (DEBUG) cout << "Active_requesters: " << active_requesters << endl;
		if (DEBUG) cout << "queue.size(): " << queue.size() << endl;	
		std::cout.flush();
			
		// first aquire the mutex
		thread_lock(DISK_LOCK);		
		
		if (DEBUG) cout << "min(max_queue_size, active_requesters): " << min(max_queue_size, active_requesters) << endl;
		std::cout.flush();
		
		// check to see if the queue is as full as it can be
		while (queue.size() < (min(max_queue_size, active_requesters))) 
		{
			// queue is maxed so wait to allow other thread to work
			if (DEBUG) cout << "wait started" << endl;
			std::cout.flush();
			thread_signal(DISK_LOCK, IS_NOT_FULL_COND);
			thread_wait(DISK_LOCK, IS_NOT_EMPTY_COND);
			if (DEBUG) cout << "wait ended" << endl;
			std::cout.flush();
		}
		
		/* 
		 * the queue now should not be emtpy, 
		 * but we don't processes until the max possible number of requests in queue.
		 * we consider the max requests as being equal to:
		 * the minimum of total number of recievers and the queue size
		*/
		
		if (queue.size() >  0 && (queue.size() >= (min(max_queue_size, active_requesters))))
		//if (queue.size() >  0 )	
		{	
		
			// time to procees next entry
			int min_distance = INT_MAX; 
			int min_i; 
			for(unsigned int i = 0; i < queue.size(); i++)	
			{
				int distance =  abs(queue[i].track - last_track);
				if(distance < min_distance) 
				{
					min_distance = distance;
					min_i = i;
				}
			}	
			last_track = queue[min_i].track;
			int last_requester = queue[min_i].requester_id;
			int condition = queue[min_i].condition;
			queue.erase(queue.begin()+min_i);
			
			cout << "service requester " << (last_requester) << " track " << last_track << endl; 
			cout.flush();


			thread_signal(DISK_LOCK, condition);
			
			// Now signal requestors the queue is not full
			thread_signal(DISK_LOCK, IS_NOT_FULL_COND);	
		}

		thread_unlock(DISK_LOCK);
	} 
}		
		
void requester(void *info)
{

	requester_info* req_info = (requester_info *) info;
	
	ifstream stream(req_info->filename.c_str());
	
	if(stream.good()) 
	{

	
		// loop while we have new tracks 
		int track_ID;
		while(stream >> track_ID)
		{	
		
			// acquire mutex
			thread_lock(DISK_LOCK);
			
			// test if the queue is full
			while (queue.size() >= max_queue_size) 
			{
				thread_signal(DISK_LOCK, IS_NOT_EMPTY_COND);
				// wait till the queue is not full
				thread_wait(DISK_LOCK, IS_NOT_FULL_COND);

			}
		
			// create new entry	
			entry myentry;
			myentry.track = track_ID;
			myentry.requester_id = req_info->id;
			myentry.condition = req_info->condition;
		
			// push entry to server
			queue.push_back(myentry);
			cout << "requester " << (myentry.requester_id) << " track " << track_ID << endl; 
			cout.flush();		
						
			// signal the release mutex		
			thread_signal(DISK_LOCK, IS_NOT_EMPTY_COND); 				
				
			// wait (mutex, condition)
			thread_wait(DISK_LOCK, myentry.condition);	
			
			// check queue to make sure you weren't interrupted
				
			thread_unlock(DISK_LOCK);
			
		} // end while
	
		// deregister this requestor
		{
			// first acquire mutex
			thread_lock(DISK_LOCK);
		
			// next decrement active requestor counter
			active_requesters--;
			
			// signal the release mutex		
			thread_signal(DISK_LOCK, IS_NOT_EMPTY_COND); 	
		
			// release mutex
			thread_unlock(DISK_LOCK);
		}	   
	
		stream.close();
	
	}
	
	// error check to prevent missing files messing up order
	else
	{
		active_requesters--;
	}
}	

int main(int argc, char* argv[])
{
	if (argc < 3) 
	{
		exit(EXIT_FAILURE);
	}
	
	// Initialize global variables	
	total_requesters = argc - 2;
	max_queue_size = atoi(argv[1]);
	
	// Create Master Thread
	if (DEBUG) cout << "Pre-Lib:" << endl;
	std::cout.flush();
	thread_libinit((thread_startfunc_t) create, &argv[2]);
}

