#include "../include/thread_pool.h"
#include <atomic>
#include <iostream>
#include <vector>

int main() {
  constexpr int THREADS = 4;
  constexpr int TASKS = 10000;

  ThreadPool pool(THREADS);

  std::atomic<int> counter{0};
  std::vector<std::future<void>> futures;
  futures.reserve(TASKS);

  for (int i = 0; i < TASKS; ++i) {
    futures.push_back(pool.submit([&counter]() {
      counter.fetch_add(1, std::memory_order_relaxed);
    }));
  }

  for (auto &f : futures) {
    f.get();
  }

  if (counter != TASKS) {
    std::cerr << "Stress test failed: counter = " << counter << ", expected " << TASKS << "\n";
    return 1;
  }

  std::cout << "Stress test passed (" << TASKS << " tasks)\n";
  return 0;
}
