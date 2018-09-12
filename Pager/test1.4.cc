#include <iostream> 
#include "vm_app.h"

#define DEBUG true

int main() 
{
	char *a;
	char *b;
	char *c;
	
	a = (char *) vm_extend();
	b = (char *) vm_extend();
	c = (char *) vm_extend();

	a [0] = 'a';
	b [0] = 'b';
	c [0] = 'c';
		
	vm_syslog(b, 1);
	vm_syslog(c, 1);
}
