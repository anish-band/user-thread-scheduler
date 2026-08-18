#include <iostream>
#include <ucontext.h>

ucontext_t context_main, context_a, context_b;

void funcA() {
  std::cout << "A: pre swap\n";
  swapcontext(&context_a, &context_b);
  std::cout << "A: post swap\n";
}

void funcB() {
  std::cout << "B: start\n";
  swapcontext(&context_b, &context_a);
}

int main() {
  std::cout << "Before Context Switch\n";
  char stackA[64 * 1024];
  char stackB[64 * 1024];
  getcontext(&context_a);
  getcontext(&context_b);
  context_a.uc_stack.ss_sp = stackA;
  context_b.uc_stack.ss_sp = stackB;
  context_a.uc_stack.ss_size = sizeof(stackA);
  context_b.uc_stack.ss_size = sizeof(stackB);
  context_a.uc_link = &context_main;
  context_b.uc_link = &context_a;
  makecontext(&context_a, funcA, 0);
  makecontext(&context_b, funcB, 0);
  swapcontext(&context_main, &context_a);
  std::cout << "After Context Switch\n";
}
