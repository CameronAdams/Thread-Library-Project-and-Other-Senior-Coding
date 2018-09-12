#include <iostream>
#include <stdlib.h> 
#include "vm_app.h"

#define DEBUG true

int main()
{
	char *a;
	char *b;
	char *c;
	char *d;
    
    a = (char *) vm_extend();
    b = (char *) vm_extend();
    c = (char *) vm_extend();
    d = (char *) vm_extend();

	b[0] = 'Y';
	b[1] = 'e';
	b[2] = 's';

	d[0] = 'S';
	d[1] = 'i';
	d[2] = 'r';

	vm_syslog(a, 3);
	vm_syslog(b, 3);
	vm_syslog(c, 5);
	vm_syslog(d, 5);
}
