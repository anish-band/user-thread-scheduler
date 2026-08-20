#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

const long long TARGET_SWITCHES = 1000000;
long long switch_count = 0;
int turn = 0;
std::mutex mtx;
std::condition_variable cv;

void funcA() {
  while (switch_count < TARGET_SWITCHES) {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [] { return turn == 0; });
    switch_count++;
    turn = 1;
    cv.notify_one();
  }
}

void funcB() {
  while (switch_count < TARGET_SWITCHES) {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [] { return turn == 1; });
    switch_count++;
    turn = 0;
    cv.notify_one();
  }
}

int main() {
  auto t_start = std::chrono::high_resolution_clock::now();

  std::thread t1(funcA);
  std::thread t2(funcB);

  t1.join();
  t2.join();

  auto t_end = std::chrono::high_resolution_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::nanoseconds>(t_end - t_start);
  long long avg_ns = duration.count() / switch_count;

  std::cout << "Total switches: " << switch_count << "\n";
  std::cout << "Total time: " << duration.count() << " ns\n";
  std::cout << "Average per switch: " << avg_ns << " ns\n";
}
