#include <iostream>
#include <ucontext.h>
#include <vector>

ucontext_t context_main;

struct Thread {
  ucontext_t context;
  char *stack;
  bool status;
};

std::vector<Thread *> threads;
int current_thread = 0;

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
    swapcontext(&threads[prev]->context, &context_main);
    return;
  }

  current_thread = next;
  swapcontext(&threads[prev]->context, &threads[next]->context);
}

void worker() {
  int start_id = current_thread;
  std::cout << "Pre swap ID: " << current_thread << std::endl;
  yield();
  std::cout << "Post Swap\n";
  threads[start_id]->status = true;
  yield();
}

int main() {
  std::cout << "Before Context Switch\n";
  int num_threads = 5;
  for (int i = 0; i < num_threads; i++) {
    Thread *t = new Thread();
    t->stack = new char[64 * 1024];
    t->status = false;

    getcontext(&t->context);
    t->context.uc_stack.ss_sp = t->stack;
    t->context.uc_stack.ss_size = 64 * 1024;
    t->context.uc_link = &context_main;
    makecontext(&t->context, worker, 0);
    threads.push_back(t);
  }

  swapcontext(&context_main, &threads[0]->context);
  std::cout << "After Context Switch\n";
}
