

#include "vm_app.h"

#include <iostream>
#include <cstdio>

#define DEBUG true

int main()
{ 
	char *a;
	
	for(int j = 0 ; j < 100; j++) {
		a = (char *) vm_extend();
	
		for(int i=0; i < 8192; i++)
		{
			if (a[i] != 0)
			{
				std::cout << "Memory not zero'd" << std::endl;
				std::cerr << "error: Memeory not zero'd" << std::endl; 
				return -1;  	
			}
		
			a[i] = 'g';
		}
		
		//std::cout << "Press any key to continue";
		//getchar();
	}
	
	std::cout << "Memory Initialized to zero correctly" << std::endl;
}
