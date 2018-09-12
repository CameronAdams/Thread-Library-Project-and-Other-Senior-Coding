#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <queue>
#include <stack>
#include <string>
#include <utility>
#include <vector>
#include <assert.h>
#include "vm_pager.h"

#define DEBUG false

typedef struct page 
{
    bool dirtyBit;
    bool referenceBit;
    bool residentBit;
    bool validBit;
    bool newPage;
    pid_t owner; 
    unsigned int blockNum;
    unsigned int entryNum;
} virtualPage;

typedef struct process 
{
		page_table_t pageTable;
		std::vector<virtualPage*> virtualPageVector;
		int virtualPageNum;
} processTable;

static pid_t currentPid;
static processTable *currentProcess;
static bool *diskBlockIsFree;

static std::map<pid_t, processTable*> processTableMap;
static std::vector<unsigned int> freePPages;
static std::deque<std::pair<unsigned int, virtualPage*> > clockDeque;

static int numFreeBlocks = -1;
static int numTotalBlocks = -1;

void clockDequeCheck()
{
	if (DEBUG) std::cout << "Printing Deque" << std::endl;
	if (DEBUG) std::cout.flush();
	std::deque<std::pair<unsigned int, virtualPage*> >::iterator it = clockDeque.begin();

	while (it != clockDeque.end())
	{
		unsigned int pPage = it->first;
		virtualPage* vPage = it->second;
		std::cout << "\tPhysical Page= " << pPage << std::endl;
		std::cout << "\tVirtual Page= " << vPage << std::endl;
		std::cout << "\tProcessID= " << vPage->owner << std::endl;
		it++;
	}
	if (DEBUG) std::cout << "Finished Printing Deque" << std::endl;
}

void vm_init(unsigned int memory_pages, unsigned int disk_blocks) 
{
	if (DEBUG) std::cout << "Entering vm_init" << std::endl;
	if (DEBUG) std::cout.flush();
	if (DEBUG) clockDequeCheck();
	
	int i = 0; 
	int j = 0;
	if (DEBUG) std::cout << "i= " << i << "j = " << j << std::endl;
	diskBlockIsFree = new bool [disk_blocks];
	page_table_base_register = NULL;
	
	while(i < memory_pages) 
	{
		freePPages.push_back(i);
		i++;
	}	

	while(j < disk_blocks) 
	{
		diskBlockIsFree[j] = false;
		j++;
	}

	numFreeBlocks = disk_blocks;
	numTotalBlocks = disk_blocks;
	
	if (DEBUG) std::cout << "Exiting vm_init" << std::endl;
	if (DEBUG) std::cout.flush();
}

void vm_create(pid_t pid) 
{
	if (DEBUG) std::cout << "Entering vm_create" << std::endl;		
	if (DEBUG) std::cout.flush();
	
	processTable *newProcessTable = new processTable();
	
	if (DEBUG) std::cout << "Created Process Table" << std::endl;
	if (DEBUG) std::cout.flush();		
	
	processTableMap.insert(std::pair<pid_t, processTable*> (pid, newProcessTable));
	
	if (DEBUG) std::cout << "Exiting vm_create" << std::endl;
	if (DEBUG) std::cout.flush();
}

void vm_switch(pid_t pid)
{
	if (DEBUG) clockDequeCheck();
	if (DEBUG) std::cout << "Entering vm_switch" << std::endl;
	if (DEBUG) std::cout.flush();
	
	if(processTableMap.find(pid)!=processTableMap.end())
	{
		currentPid = pid;
		currentProcess = processTableMap[currentPid];
	
		page_table_base_register = &currentProcess ->pageTable;	
	}
	
	if (DEBUG) std::cout << "Exiting vm_switch" << std::endl;
	if (DEBUG) std::cout.flush();
}

int vm_fault(void *addr, bool write_flag)
{
	if (DEBUG) std::cout << "Entering vm_fault" << std::endl;
	if (DEBUG) std::cout.flush();
	
	if ((intptr_t) addr < (intptr_t) VM_ARENA_BASEADDR || ((intptr_t)addr - (intptr_t)VM_ARENA_BASEADDR) >= (VM_PAGESIZE*currentProcess->virtualPageNum))
	{
        return -1;
	}

	int pageNum = ((intptr_t)addr - (intptr_t)VM_ARENA_BASEADDR) /VM_PAGESIZE;
	
	unsigned int numPPage;
	unsigned int evictedPage;
	bool searching = true;
	std::pair<unsigned int, virtualPage *> nextVirtualPage;

	page_table_base_register = &currentProcess->pageTable;	
	if (currentProcess->virtualPageVector[pageNum]->residentBit == false) 
	{
		if(freePPages.empty()) 
		{
			while (searching == true) 
			{
				if (DEBUG) std::cout << "about to pop from clock queue size=" << clockDeque.size() << std::endl;
				if (DEBUG) std::cout.flush();
				nextVirtualPage = clockDeque.front();
				clockDeque.pop_front();

				if(nextVirtualPage.second->referenceBit == true) 
				{
					nextVirtualPage.second->referenceBit = false;
					processTableMap[nextVirtualPage.second->owner]->pageTable.ptes[nextVirtualPage.second->entryNum].read_enable = 0;
					processTableMap[nextVirtualPage.second->owner]->pageTable.ptes[nextVirtualPage.second->entryNum].write_enable = 0;
					clockDeque.push_back(nextVirtualPage);
				}

				else 
				{
					evictedPage = nextVirtualPage.first;
					nextVirtualPage.second->residentBit = false;

					if (nextVirtualPage.second->dirtyBit == true) {

						nextVirtualPage.second->dirtyBit = false;
						nextVirtualPage.second->newPage = false;
						disk_write(nextVirtualPage.second->blockNum, nextVirtualPage.first);
						if (DEBUG) std::cout << "**************************vm_fault: disk write" << std::endl;
						if (DEBUG) std::cout.flush();
					}
					searching = false;
				}

			}	

		 	processTableMap[nextVirtualPage.second->owner]->pageTable.ptes[nextVirtualPage.second->entryNum].read_enable = 0;
			processTableMap[nextVirtualPage.second->owner]->pageTable.ptes[nextVirtualPage.second->entryNum].write_enable = 0;
			numPPage = evictedPage;		
		}

		else 
		{
			numPPage = freePPages[0];
			freePPages.erase(freePPages.begin());
		}
	
		if (currentProcess->virtualPageVector[pageNum]->newPage == true) 
		{
			if (DEBUG) std::cout << "**************************new page: Setting to Zero" << std::endl;
			if (DEBUG) std::cout.flush();
			memset((char *)pm_physmem + (VM_PAGESIZE * numPPage), 0, VM_PAGESIZE);
		}

		else 
		{
			if (DEBUG) std::cout << "**************************vm_fault: disk read" << std::endl;
			if (DEBUG) std::cout.flush();
			disk_read(currentProcess->virtualPageVector[pageNum]->blockNum, numPPage);	
			currentProcess->virtualPageVector[pageNum]->dirtyBit = false;
		}
            
		page_table_base_register->ptes[pageNum].ppage = numPPage;	
		currentProcess->virtualPageVector[pageNum]->residentBit = true;
		clockDeque.push_back(std::pair<unsigned int, virtualPage *> (numPPage, currentProcess->virtualPageVector[pageNum])); 

	}
	
	if(write_flag == true || currentProcess->virtualPageVector[pageNum]->dirtyBit == 1) 
	{
		page_table_base_register->ptes[pageNum].write_enable = true;
		currentProcess->virtualPageVector[pageNum]->dirtyBit = true;
		currentProcess->virtualPageVector[pageNum]->newPage = false;
	}
	page_table_base_register->ptes[pageNum].read_enable = 1;
	currentProcess->virtualPageVector[pageNum]->referenceBit = true;
	
	if (DEBUG) std::cout << "Exiting vm_fault" << std::endl;
	if (DEBUG) std::cout.flush();
	
	return 0;
}

void vm_destroy()
{
	if (DEBUG) std::cout << "Entering vm_destroy" << std::endl;
	if (DEBUG) std::cout.flush();
	
	std::pair<unsigned int, virtualPage*> deletedVirtualPage;
	
	int numclockDeque = clockDeque.size();
	int i = 0;
	int j = 0;
	
	if (DEBUG) std::cout << "about to destroy clock deque size=" << numclockDeque << ", i=" << i <<std::endl;
	if (DEBUG) std::cout.flush();
	if (DEBUG) clockDequeCheck();
	while (i < numclockDeque) 
	{
		if (DEBUG) std::cout << "inside of loop" << numclockDeque << std::endl;
		if (DEBUG) std::cout.flush();
		deletedVirtualPage = clockDeque.front();
		clockDeque.pop_front();
		
		if (DEBUG) std::cout << "pid before=" << currentPid << std::endl;
		if (DEBUG) std::cout.flush();
		if (deletedVirtualPage.second->owner != currentPid)
		{
			if (DEBUG) std::cout << "pid in=" << currentPid << std::endl;
			if (DEBUG) std::cout.flush();
			clockDeque.push_back(deletedVirtualPage);
		}

		else 
		{
			freePPages.push_back(deletedVirtualPage.first);
		}
		i++;
	}
	if (DEBUG) std::cout << "destroyed clock deque size=" << clockDeque.size() << std::endl;
	if (DEBUG) std::cout.flush();
	
	while (j < currentProcess->virtualPageNum) 
	{
		diskBlockIsFree[currentProcess->virtualPageVector[j]->blockNum] = false;
		//currentProcess->virtualPageVector[j]->validBit = false;
		delete currentProcess->virtualPageVector[j];
		j++;
		numFreeBlocks++;
	}
	//processTableMap[currentPid] = NULL;
	page_table_base_register = NULL;
	delete processTableMap[currentPid];
	processTableMap.erase(currentPid);
	
	if (DEBUG) std::cout << "Exiting vm_destroy" << std::endl;
	if (DEBUG) std::cout.flush();
	if (DEBUG) clockDequeCheck();
	return;
}

void * vm_extend() 
{
	if (DEBUG) std::cout << "Entering vm_extend" << std::endl;
	if (DEBUG) std::cout.flush();
	
	int numTotalVirtualPages = currentProcess->virtualPageVector.size();
	int i = 0;
	int newFreeBlock = -1;

	if (numFreeBlocks == 0)
	{
		return NULL;
	} 
	
	if(numTotalVirtualPages >= VM_ARENA_SIZE/VM_PAGESIZE || numTotalVirtualPages < 0) 
	{
		return NULL;
	}

	while(i < numTotalBlocks)
	{
		
		if (diskBlockIsFree[i] != true) 
		{
			newFreeBlock = i;
			numFreeBlocks--;
			diskBlockIsFree[i] = true;
			break;
		}
		i++;
	}

	if (newFreeBlock != -1) 
	{
		virtualPage *createVirtualPage  = new virtualPage;
		createVirtualPage ->dirtyBit = false;
		createVirtualPage ->referenceBit = false;
		createVirtualPage ->residentBit = false;
		createVirtualPage ->newPage = true;
		createVirtualPage ->validBit = true;
		createVirtualPage ->owner = currentPid;
		createVirtualPage ->blockNum = newFreeBlock;
		createVirtualPage ->entryNum = numTotalVirtualPages;
		
		currentProcess->virtualPageVector.push_back(createVirtualPage );
		currentProcess->virtualPageNum = currentProcess->virtualPageVector.size();
		
		if (DEBUG) std::cout << "Exiting vm_extend" << std::endl;
		if (DEBUG) std::cout.flush();
		
		return (void *) ((intptr_t) VM_ARENA_BASEADDR + VM_PAGESIZE * numTotalVirtualPages);
	}
	
	else
	{
		
		if (DEBUG) std::cout << "Exiting vm_extend" << std::endl;
		if (DEBUG) std::cout.flush();
		
		return NULL;
	}
}

int vm_syslog(void *message, unsigned int len)
{
	if (DEBUG) std::cout << "Entering vm_syslog" << std::endl;
	if (DEBUG) std::cout.flush();
	
	if ((intptr_t) message <  (intptr_t) VM_ARENA_BASEADDR || (intptr_t) VM_ARENA_BASEADDR + (int)  (VM_PAGESIZE * currentProcess->virtualPageNum) < (intptr_t) message + (int) len|| (int) len <= 0) 
	{
		return -1;
	}
	
	// a variable to hold the message bytes
	std::string s;
	
	// variables to keep track of where we are reading from, and the remaing number of bytes to read
	int remainingBytes = len;
	char* readAddress = (char *) message; 
	
	// while there are remaining bytes to read
	while(remainingBytes > 0)
	{
		// read all the bytes from the current page into  s
		
		// first calculate the current page page, from the read address, and the offset
		int virtualPageNumber = ((intptr_t) readAddress - (intptr_t) VM_ARENA_BASEADDR)/VM_PAGESIZE;
		int offset = ((intptr_t) readAddress - (intptr_t) VM_ARENA_BASEADDR)%VM_PAGESIZE;
		
		// next calculate how many bytes to read (in the current page)
		size_t bytesToRead = std::min(remainingBytes, VM_PAGESIZE-offset);

		// test if the current page is resident and readable
		bool isread = page_table_base_register->ptes[virtualPageNumber].read_enable;
		
		if (DEBUG) std::cout << "vm_syslog: isread = "<< ((isread)?"true":"false") << std::endl;
		if (DEBUG) std::cout.flush(); 
		
		if (isread == 0)
		{
			// virtual page not resident so try to simulate a read fault
			if(vm_fault(readAddress, false) == -1)	
			{
				// fault failed to load virtal page
				if (DEBUG) std::cout << "vm_syslog: fault failed, ppage not resident" << std::endl;
				if (DEBUG) std::cout.flush(); 
				return -1;
			}
		}
		bool isreadAfter = page_table_base_register->ptes[virtualPageNumber].read_enable;
		
		if (DEBUG) std::cout << "vm_syslog: isreadAfter = "<< ((isreadAfter)?"true":"false") << std::endl;
		if (DEBUG) std::cout.flush(); 
		
		// ppage address
		int ppageNum = page_table_base_register->ptes[virtualPageNumber].ppage;
		void* ppageAddress = ((char*)pm_physmem + ppageNum * VM_PAGESIZE + offset);
		
		// now read bytesToRead bytes into a buffer as a c-string
		char buff[VM_PAGESIZE+1];
		memcpy(buff, ppageAddress, bytesToRead);
		buff[bytesToRead] = '\0';
		
		// app the buff c-string to our our results s
		s += buff;
		
		// update our loop variables
		readAddress += bytesToRead;
		remainingBytes -= bytesToRead;
	}
	
	std::cout << "syslog \t\t\t" << s << std::endl;
	
	if (DEBUG) std::cout << "Exiting vm_syslog" << std::endl;
	if (DEBUG) std::cout.flush();

	return 0;
} 
