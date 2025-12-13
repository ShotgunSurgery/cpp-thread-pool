#include "thread_pool.h"
#include <algorithm>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
using namespace std;

// we must lock the shared data i.e. the queue and the shutdown_flag while a
// thread checks both to avoid race conditions. order of things -
// 0. the thread enters the while loop
// 1. unique_lock is init which by default locks the mutex
// 2. lambda is safely evaluated
// 3. if it's evaluated to true then the threads runs
// 3. if it's evaluated to false the thread is put to sleep and the wait()
// unlocks the mutex until
// 4. it recieves a notification and it wakes up then the control returns back
// to the block and it continues in the while loop and then again keeps looping in the while loop
void ThreadPool::infiniteLoop() {
  while (true) {
    unique_lock<mutex> lock(ThreadPool::queue_mutex);
    ThreadPool::cv.wait(lock, [this]() {
      return shutdown_flag || !ThreadPool::task_queue.empty();
    });
    // when control returns then cv again lock it and only then the control returns to the next line
    if (shutdown_flag && task_queue.empty())
      break;
    function<void()> task = ThreadPool::task_queue.front();
    ThreadPool::task_queue.pop();
    lock.unlock();
    task();
  }
}

ThreadPool::ThreadPool(size_t no_of_threads) : no_threads(no_of_threads) {
  for (size_t i = 0; i < no_of_threads; i++) {
    workers.emplace_back([this]() {
      this->infiniteLoop();
    });
  }
}

// variadic templates are moved not inspected
// here F is the type of callable i.e. is it a function or lambda or functor ... and args are the types of argument to the callable f is the
// actuall object of type F and args... is the actual value of arguments passed

ThreadPool::~ThreadPool() {
  shutdown_flag = 1;
  cv.notify_all();
  for (size_t i = 0; i < no_threads; i++) {
    workers[i].join();
  }
}
