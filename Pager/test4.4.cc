#include <iostream>
#include "vm_app.h"

#define DEBUG true

int main() {
   
    char *a;
    char *b;
	
 	a = (char*) vm_extend();
 	b = (char*) vm_extend();

	a =  a + 8190;
	a [0] = 'A';
	a [1] = 'd';
	a [2] = 'v';
	a [3] = 'e';
	a [4] = 'n';
	a [5] = 't';
	a [6] = 'u';
	a [7] = 'r';
	a [8] = 'e';

	vm_syslog(a, 9);
	vm_syslog(b, 8);

	b[0] = 'B';
	b[1] = 'o';
	b[2] = 'a';
	b[3] = 'r';
	b[4] = 'd';
	
	vm_syslog(b, 5);

	a = b + 8190;

	a[0] = 'G';
	a[1] = 'o';
	a[2] = 'n';
	a[3] = 'e';

	vm_syslog(a, 4);
	vm_syslog(a, 9);
}
