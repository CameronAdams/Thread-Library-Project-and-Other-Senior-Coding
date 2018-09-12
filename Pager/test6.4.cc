#include <iostream>
#include <unistd.h>
#include "vm_app.h"

#define DEBUG true

int main() {

		char a;
		char *b;
		
		b = (char *) vm_extend();

		a = b[0];

		b = (char *) vm_extend();

		a = b[8192];
		
		char *c;
		
		c = (char *) vm_extend();
		
		a = b[0];
		
		vm_syslog(c, 1);
}
