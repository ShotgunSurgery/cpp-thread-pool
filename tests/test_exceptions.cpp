#include "../include/thread_pool.h"
#include <iostream>
#include <stdexcept>

int main() {
  ThreadPool pool(2);

  // the following is a lambda function only and this - "-> int" is a explicit return type
  auto fut = pool.submit([]() -> int {
    throw std::runtime_error("boom");
    return 42;
  });

  try {
    // the get() function does something like below internally and because it's the main thread executing it this time hence it get's caught in the
    //     main thread only below
    // if (state contains exception)
    //     rethrow it;
    // else
    //     return value;
    fut.get();
    std::cerr << "Exception test failed (no exception)\n";
    return 1;
  } catch (const std::runtime_error &e) {
    std::cout << "Caught exception: " << e.what() << "\n";
    std::cout << "Exception test passed\n";
  } catch (...) {
    std::cerr << "Wrong exception type\n";
    return 1;
  }

  return 0;
}
