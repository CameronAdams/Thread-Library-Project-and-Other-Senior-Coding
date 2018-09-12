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
    char *f;
    
    a = (char *) vm_extend(); 
    b = (char *) vm_extend();
    c = (char *) vm_extend();  
    d = (char *) vm_extend();
    e = (char *) vm_extend();
    f = (char *) vm_extend();

    a[0] = 'W';
    a[1] = 'e';
    a[2] = 'l';
    a[3] = 'c';
    a[4] = 'o';
    a[5] = 'm';
    a[6] = 'e';

    b[0] = 'A';
    b[1] = 'd';
    b[2] = 'v';
    b[3] = 'e';
    b[4] = 'n';
    b[5] = 't';
    b[6] = 'u';
    b[7] = 'r';
    b[8] = 'e';
    b[9] = 'r';

	c[0] = 'T';
    c[1] = 'h';
    c[2] = 'i';
    c[3] = 's'; 

    d[0] = 'I';
    d[1] = 's'; 

    e[0] = 'D';
    e[1] = 'a';
    e[2] = 'n';
    e[3] = 'd'; 
    e[4] = 'd'; 

    for (int i = 0; i < 8; i++)
    {
    	b[i] = f[i];
    }

    c[0] = 'B';
    c[1] = 'e';

    f[0] = 'S';
    f[1] = 'c';
    f[2] = 'a';
	f[3] = 'r';

    for (int j = 0; j < 2; j++)
    {
    	d[j] = f[j];
    }

    vm_syslog(a, 5);
    vm_syslog(b, 10);
    vm_syslog(c, 3);
    vm_syslog(d, 2);
    vm_syslog(e, 8);
    vm_syslog(e, 12);
}
