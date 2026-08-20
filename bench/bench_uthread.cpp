#include "../include/scheduler.hpp"
#include <chrono>
#include <iostream>
#include <ucontext.h>

ucontext_t context_main;
Scheduler *g_scheduler;

void worker() {
  std::cout << "Pre swap\n";
  g_scheduler->yield();
  std::cout << "Post Swap\n";
  g_scheduler->complete();
  g_scheduler->yield();
}

long long switch_count = 0;
const long long TARGET_SWITCHES = 1000000;

void bench_worker() {
  while (switch_count < TARGET_SWITCHES) {
    switch_count++;
    g_scheduler->yield();
  }

  g_scheduler->complete();
  g_scheduler->yield();
}

int main() {
  std::cout << "Before Context Switch\n";

  Scheduler scheduler;
  g_scheduler = &scheduler;

  int num_threads = 5;
  for (int i = 0; i < num_threads; i++) {
    scheduler.create_thread(bench_worker);
  }

  auto t_start = std::chrono::high_resolution_clock::now();
  scheduler.start(&context_main);
  auto t_end = std::chrono::high_resolution_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::nanoseconds>(t_end - t_start);
  long long avg_ns = duration.count() / switch_count;

  std::cout << "After Context Switch\n";

  std::cout << "Total switches: " << switch_count << "\n";
  std::cout << "Total time: " << duration.count() << " ns\n";
  std::cout << "Average per switch: " << avg_ns << " ns\n";
}
