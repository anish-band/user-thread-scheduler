#include <iostream>
#include <ucontext.h>

ucontext_t context_main;

struct Thread {
  ucontext_t context;
  char *stack;
  bool status;
};

Thread *g_t1, *g_t2;

void funcA() {
  std::cout << "A: pre swap\n";
  swapcontext(&g_t1->context, &g_t2->context);
  std::cout << "A: post swap\n";
}

void funcB() {
  std::cout << "B: start\n";
  swapcontext(&g_t2->context, &g_t1->context);
}

int main() {
  std::cout << "Before Context Switch\n";
  Thread t1;
  g_t1 = &t1;
  t1.stack = new char[64 * 1024];
  getcontext(&t1.context);
  t1.context.uc_stack.ss_sp = t1.stack;
  t1.context.uc_stack.ss_size = 64 * 1024;
  t1.context.uc_link = &context_main;
  makecontext(&t1.context, funcA, 0);

  Thread t2;
  g_t2 = &t2;
  t2.stack = new char[64 * 1024];
  getcontext(&t2.context);
  t2.context.uc_stack.ss_sp = t2.stack;
  t2.context.uc_stack.ss_size = 64 * 1024;
  t2.context.uc_link = &t1.context;
  makecontext(&t2.context, funcB, 0);

  swapcontext(&context_main, &t1.context);
  std::cout << "After Context Switch\n";
}
