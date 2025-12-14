#include "../include/thread_pool.h"
#include <chrono>
#include <iostream>

int main() {
  {
    ThreadPool pool(4);

    // the main thread inits the threadpool and the workers go to sleep the main thread then allocates the tasks and it worker threads
    // start taking up the tasks then the main thread reaches the end of the block where the pool object will go out of scope so the destructor
    // is called and the main calls join() after which it has to wait for the workers for completion
    for (int i = 0; i < 4; ++i) {
      pool.submit([] {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Task finished\n";
      });
    }

    // def of a proper shutdown -
    // 1. no worker thread is left running after destruction
    // 2. no worker thread is stuck sleeping forever
    // 3. the destructor waits for the workers to finish
    // 3.5 the submitted tasks are not abandoned i.e. if tasks were running or waiting then then threads don't abruptly abandone them
    // 4. the program exits without hand or crash

    // We do NOT call get() on the futures.
    // We let the pool go out of scope while tasks are running.
  } // destructor called here

  std::cout << "ThreadPool destroyed cleanly\n";
  return 0;
}
