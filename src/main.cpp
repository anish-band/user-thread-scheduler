#include <iostream>
#include <ucontext.h>
#include <vector>

ucontext_t context_main;

struct Thread {
  ucontext_t context;
  char *stack;
  bool status;
};

std::vector<Thread> threads;
int current_thread = 0;

void yield() {
  int prev = current_thread;
  current_thread = (current_thread + 1) % threads.size();
  swapcontext(&threads[prev].context, &threads[current_thread].context);
}

void funcA() {
  std::cout << "Pre swap\n";
  yield();
  std::cout << "Post swap\n";
}

void funcB() {
  std::cout << "B: start\n";
  yield();
}

int main() {
  std::cout << "Before Context Switch\n";
  Thread t1;
  t1.stack = new char[64 * 1024];
  getcontext(&t1.context);
  t1.context.uc_stack.ss_sp = t1.stack;
  t1.context.uc_stack.ss_size = 64 * 1024;
  t1.context.uc_link = &context_main;
  makecontext(&t1.context, funcA, 0);
  threads.push_back(t1);

  Thread t2;
  t2.stack = new char[64 * 1024];
  getcontext(&t2.context);
  t2.context.uc_stack.ss_sp = t2.stack;
  t2.context.uc_stack.ss_size = 64 * 1024;
  t2.context.uc_link = &t1.context;
  makecontext(&t2.context, funcB, 0);
  threads.push_back(t2);

  swapcontext(&context_main, &t1.context);
  std::cout << "After Context Switch\n";
}
