#include <iostream>
#include "vm_app.h"

#define DEBUG true

int main()
{

    char *a;
    char *b;
    char *c;
    char *d;
    char *e;
	
    a = (char *) vm_extend();
    b = (char *) vm_extend();	
    c = (char *) vm_extend();
    d = (char *) vm_extend();
    e = (char *) vm_extend();
    
    a[0] = 'A';
    b[0] = 'B';
    c[0] = 'C';
    d[0] = 'D';
    e[0] = 'E';

    vm_syslog(e, 0);
    vm_syslog(a, 1);
    vm_syslog(b, 2);
    vm_syslog(c, 3);
    vm_syslog(d, 4);
    vm_syslog(e, 5);
    vm_syslog(e, 6);
}
