#include <iostream> 
#include "vm_app.h"
   
#define DEBUG true   
   
int main() 
{

	char *a;
	char *b;

	a = (char *) vm_extend();
	
	vm_syslog(a, 5);

	int i;
	for (i = 0; i < 8192; i++)
    {
		a [i] = 'a';
    }

	vm_syslog(a + 8191, 1);

	for (int j = 0; j < 10; j++) {
		b = (char *) vm_extend();
		b [0] = 'b';
	}	

	vm_syslog(a, 1);

	a[8191] = 'c';

	vm_syslog(a+i, 1);

	vm_extend();

	vm_syslog(a, 1);
}
