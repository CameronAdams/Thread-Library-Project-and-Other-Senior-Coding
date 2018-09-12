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

	a[0] = 'M';
	a[1] = 'a';
	a[2] = 'h';
	a[3] = 'o';
	a[4] = 'u';

	b[0] = 'S';
	b[1] = 'e';
	b[2] = 'n';
	b[3] = 's';
	b[4] = 'e';
	b[5] = 'i';
	
	c[0] = 'N';
	c[1] = 'e';
	c[2] = 'g';
	c[3] = 'i';
	c[4] = 'm';
	c[5] = 'a';

	vm_syslog(a, 5);
	vm_syslog(b, 6);
	vm_syslog(c, 6);

}
