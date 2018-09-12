#include <iostream> 
#include "vm_app.h"

#define DEBUG true

int main() {

	char *a;
	char *b;
	
	a = (char *) vm_extend();
	b = (char *) vm_extend();

	for (int i = 0; i < 8192; i++)
    {
		a [i] = 'a';
    }

	a[4] = 'a';
	a[8191] = 'b';

	vm_syslog(a-1,2);
	vm_syslog(a+3, 3);
	vm_syslog(a+8190, 4);
	vm_syslog(a, 4);
	vm_syslog(a, 8191);
	vm_syslog(a, 8192);
}
