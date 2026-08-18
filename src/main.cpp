#include <iostream>
#include <ucontext.h>

ucontext_t context_main;

struct Thread {
  ucontext_t context;
  char *stack;
  bool status;
};

void funcA() {
  std::cout << "A: pre swap\n";
  // swapcontext(&context_a, &context_b);
  // structtd::cout << "A: post swap\n";
}

/*
voidid funcB() {
  std::cout << "B: start\n";
  swapcontext(&context_b, &context_a);
}
*/

int main() {
  std::cout << "Before Context Switch\n";
  Thread t1;
  t1.stack = new char[64 * 1024];
  getcontext(&t1.context);
  t1.context.uc_stack.ss_sp = t1.stack;
  t1.context.uc_stack.ss_size = sizeof(t1.stack);
  t1.context.uc_link = &context_main;
  makecontext(&t1.context, funcA, 0);

  swapcontext(&context_main, &t1.context);
  std::cout << "After Context Switch\n";
}
