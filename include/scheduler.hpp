#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "thread.hpp"
#include <sys/ucontext.h>
#include <ucontext.h>
#include <vector>

class Scheduler {
private:
  std::vector<Thread *> threads;
  int current_thread = 0;
  ucontext_t *main_context;

public:
  void create_thread(void (*func)()) {
    Thread *t = new Thread();
    t->stack = new char[64 * 1024];
    t->status = false;
    getcontext(&t->context);
    t->context.uc_stack.ss_sp = t->stack;
    t->context.uc_stack.ss_size = 64 * 1024;
    t->context.uc_link = main_context;
    makecontext(&t->context, func, 0);
    threads.push_back(t);
  }

  void yield() {
    int prev = current_thread;
    int next = prev;
    bool found = false;

    for (size_t i = 0; i < threads.size(); i++) {
      next = (next + 1) % threads.size();
      if (threads[next]->status == false) {
        found = true;
        break;
      }
    }

    if (!found) {
      swapcontext(&threads[prev]->context, main_context);
      return;
    }

    current_thread = next;
    swapcontext(&threads[prev]->context, &threads[next]->context);
  }

  void complete() { threads[current_thread]->status = true; }

  void start(ucontext_t *call_context) {
    main_context = call_context;
    if (threads.empty())
      return;
    current_thread = 0;
    swapcontext(main_context, &threads[0]->context);
  }
};

#endif
