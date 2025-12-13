#include "../include/thread_pool.h"
#include <iostream>
#include <vector>

int main() {
  ThreadPool pool(2);

  auto f1 = pool.submit([]() {
    return 1;
  });

  auto f2 = pool.submit([]() {
    return 2;
  });

  int r1 = f1.get();
  int r2 = f2.get();

  std::cout << "Result: " << (r1 + r2) << std::endl;

  return 0;
}
