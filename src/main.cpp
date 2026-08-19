#include "../include/scheduler.hpp"
#include <iostream>
#include <ucontext.h>
#include <vector>

ucontext_t context_main;
Scheduler *g_scheduler;

void worker() {
  std::cout << "Pre swap\n";
  g_scheduler->yield();
  std::cout << "Post Swap\n";
  g_scheduler->complete();
  g_scheduler->yield();
}

int main() {
  std::cout << "Before Context Switch\n";

  Scheduler scheduler;
  g_scheduler = &scheduler;

  int num_threads = 5;
  for (int i = 0; i < num_threads; i++) {
    scheduler.create_thread(worker);
  }

  scheduler.start(&context_main);
  std::cout << "After Context Switch\n";
}
