#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#ifndef THREADPOOL_H
#define THREADPOOL_H

class ThreadPool {
public:
  ThreadPool(size_t no_of_threads);
  // this template is defined here because - Templates are compiled only when they are used, and the compiler must see the full
  // definition at that point. this is because a template is unlike a function which compiler calls knowing the signature, assumes it
  // is defined somewhere and the linker later connects call to def
  // invoke_result_t
  // template <typename F, typename... Args> auto submit(F &&f, Args &&...args) { // std::packaged_task<R(Args...)> callable_object_name(function) here R is the return type of the function and Args... is
  //   // the parameter type of the arguments
  //   using R = std::invoke_result_t<F, Args...>;
  //   auto bound = [func = std::forward<F>(f), ... params = std::forward<Args>(args)]() mutable {
  //     return func(params...);
  //   };
  //   std::packaged_task<R()> task(std::move(bound));
  //   std::future<R> fut = task.get_future();
  //   {
  //     std::unique_lock<std::mutex> lock(queue_mutex);
  //     task_queue.push([t = std::move(task)]() mutable {
  //       t();
  //     });
  //   }
  //   cv.notify_one();
  //   return fut;
  // }

  template <typename F, typename... Args> auto submit(F &&f, Args &&...args) {
    using R = std::invoke_result_t<F, Args...>;

    auto task_ptr = std::make_shared<std::packaged_task<R()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<R> fut = task_ptr->get_future();

    {
      std::unique_lock<std::mutex> lock(queue_mutex);
      task_queue.push([task_ptr]() {
        (*task_ptr)();
      });
    }

    cv.notify_one();
    return fut;
  }
  ~ThreadPool();

  // every shared structure must be private cause if some other code tries to
  // access it then it will break the whole thing
private:
  std::queue<std::function<void()>> task_queue;
  std::mutex queue_mutex;
  std::condition_variable cv;
  bool shutdown_flag = 0;
  std::vector<std::thread> workers;
  void infiniteLoop();
  size_t no_threads;
};

#endif
