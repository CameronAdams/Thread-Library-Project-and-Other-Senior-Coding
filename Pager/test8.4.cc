#include <iostream> 
#include "vm_app.h"
   
#define DEBUG true 
   
int main() 
{
	char *a;
	char *b;
	char *c;
	char *d;

	a = (char *)vm_extend();
	b = (char *)vm_extend();
	c = (char *)vm_extend();
	d = (char *)vm_extend();

	for (int i = 0; i < 8192; i++)
    {
		a [i] = 'a';
    }

	a [8192] = 'b';
	b [0] = 'c';
	c [0] = 'd';
	a [8191] = 'e';
	
	vm_syslog(a + 8191, 3);
	vm_syslog(b, 1);
	vm_syslog(a, 8191);
}
