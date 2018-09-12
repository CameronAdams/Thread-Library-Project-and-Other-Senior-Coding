#include <iostream>
#include <stdlib.h>
#include "thread.h"

#define DEBUG true

int num=0;

void loop(void *a) {
  char *id;
  int i;

  id = (char *) a;
  std::cout <<"loop called with id " << (char *) id << std::endl;
  std::cout.flush();

  for (int i=0; i<10; i++, num++) {
    std::cout << id << ":\t" << i << "\t" << num << std::endl;
    std::cout.flush();
    if (thread_yield()) {
      std::cout << "thread_yield failed" << std::endl;
      std::cout.flush();
      exit(1);
    }
  }

  if (thread_libinit( (thread_startfunc_t) loop, (void *) 100)) {
    std::cout << "thread_init dummy failed" << std::endl;
    std::cout.flush();
  }

}

void parent(void *a) 
{
  int arg;
  arg = (intptr_t) a;
  std::cout << "parent called with arg " << arg << std::endl;
  std::cout.flush();
  if (thread_create((thread_startfunc_t) loop, (void *) "child 1 thread")) {
    std::cout << "thread_create failed" << std::endl;
    std::cout.flush();
    exit(1);
  }

  loop( (void *) "parent thread");

  if (thread_create((thread_startfunc_t) loop, (void *) "child 2 thread")) {
    std::cout << "thread_create failed" << std::endl;
    std::cout.flush();
    exit(1);
  }

}

int main(int argc, char *argv[]) {

  if (thread_create((thread_startfunc_t) loop, (void *) "dummy thread")) {
    std::cout << "thread_create dummy thread failed" << std::endl;
    std::cout.flush();
  }

  if (thread_libinit( (thread_startfunc_t) parent, (void *) 100)) {
    std::cout << "thread_init failed" << std::endl;
    std::cout.flush();
    exit(1);
  }
}
