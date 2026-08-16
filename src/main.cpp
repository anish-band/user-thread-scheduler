#include <iostream>
#include <ucontext.h>

ucontext_t context_main, context_a;

void funcA() { std::cout << "A: start\n"; }

int main() {
  std::cout << "Before Context Switch\n";
  char stackA[64 * 1024];
  getcontext(&context_a);
  context_a.uc_stack.ss_sp = stackA;
  context_a.uc_stack.ss_size = sizeof(stackA);
  context_a.uc_link = &context_main;
  makecontext(&context_a, funcA, 0);
  swapcontext(&context_main, &context_a);
  std::cout << "After Context Switch\n";
}
