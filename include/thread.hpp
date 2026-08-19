#ifndef THREAD_HPP
#define THREAD_HPP

#include <sys/ucontext.h>
#include <ucontext.h>

struct Thread {
  ucontext_t context;
  char *stack;
  bool status;
};

#endif
