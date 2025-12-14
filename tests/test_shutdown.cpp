#include "../include/thread_pool.h"
#include <chrono>
#include <iostream>

int main() {
  {
    ThreadPool pool(4);

    // Submit tasks that take time
    for (int i = 0; i < 4; ++i) {
      pool.submit([] {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Task finished\n";
      });
    }

    // IMPORTANT:
    // We do NOT call get() on the futures.
    // We let the pool go out of scope while tasks are running.
  } // destructor called here

  std::cout << "ThreadPool destroyed cleanly\n";
  return 0;
}
