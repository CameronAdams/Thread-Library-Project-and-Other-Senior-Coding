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

    a[0] = 'h';
    a[1] = 'i';
    a[2] = 'd';
    a[3] = 'e';

    b[0] = 'w';
    b[1] = 'a';
    b[2] = 'i';
    b[3] = 't';

    c[0] = 'f';
    c[1] = 'i';
    c[2] = 'n';
    c[3] = 'd';

    for (int i = 0; i < 4; i++)
    {
    	e[i] = a[i];
    }
    
    for (int j = 0; j < 4; j++)
    {
        d[j] = c[j];
    }

    for (int l = 0; l < 4; l++)
    {
        f[l] = b[l];
    }

    vm_syslog(a, 4);
    vm_syslog(b, 4);
    vm_syslog(c, 4);    
    vm_syslog(d, 4);
    vm_syslog(e, 4);
    vm_syslog(f, 4);
}
