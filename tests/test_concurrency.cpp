#include "../include/thread_pool.h"
#include <chrono>
#include <iostream>
#include <vector>

int main() {
  ThreadPool pool(4);

  auto start = std::chrono::steady_clock::now();

  std::vector<std::future<void>> futures;
  for (int i = 0; i < 4; ++i) {
    futures.push_back(pool.submit([] {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }));
  }

  for (auto &f : futures) {
    f.get();
  }

  auto end = std::chrono::steady_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

  std::cout << "Elapsed time: " << elapsed << " ms\n";

  if (elapsed > 2000) {
    std::cerr << "Concurrency test failed\n";
    return 1;
  }

  std::cout << "Concurrency test passed\n";
  return 0;
}
