#include <iostream>
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

    a[0] = 'H';
    a[1] = 'e';
    a[2] = 'l';
    a[3] = 'l';
    a[4] = 'o';
    a[5] = '!';

    b[0] = 'H';
    b[1] = 'e';
    b[2] = 'r';
    b[3] = 'o';
    b[4] = 'e';
    b[5] = 's';

    c[0] = 'S';
    c[1] = 'q';
    c[2] = 'u';
    c[3] = 'a';
    c[4] = 'd';
    c[5] = 's';

    for (int i = 0; i < 6; i++)
    {
    	d[i] = b[i];
    }
    
    for (int j = 0; j < 6; j++)
    {
        c[j] = a[j];
    }
    
    for (int k = 0; k < 6; k++)
    {
        b[k] = a[k];
    }

    vm_syslog(a, 6);
    vm_syslog(b, 6);
    vm_syslog(c, 6);
    vm_syslog(d, 6);
}
