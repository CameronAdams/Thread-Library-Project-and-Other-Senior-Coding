#include <iostream> 
#include "vm_app.h"

#define DEBUG true
   
int main() {

	char *a;
	char *b;
	char *c;

	int i = 0; 

	a = (char *) vm_extend();
	c = (char *) vm_extend();
	
	vm_syslog(a, 5);

	while (i < 8192) {
		a[i] = 'a';
		i++;
	}

	vm_syslog(a + 8193, 1);

	for (int j = 0; j < 10; j++) {
		b = (char *) vm_extend();
		b [0] = 'b';
	}	

	vm_syslog(a, 1);

	vm_extend();
	
	a[i] = 'c';

	vm_syslog(a+i, 1);

	vm_extend();

	vm_syslog(a, 10);
	vm_syslog(b, 11);
}
