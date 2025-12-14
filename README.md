# C++ Thread Pool (C++17)

A minimal thread pool implementation with safe shutdown, exception propagation, and verified concurrency.

---

## Motivation

### What is a thread pool anyway?

A thread pool is a fixed number of pre-created threads that are initialized at the start of the program. These threads sleep in a loop while waiting for tasks. When a task arrives, it is pushed into a shared queue, and one of the sleeping threads is woken up and assigned the task. This process continues until the pool is instructed to shut down.

This design solves several problems. Creating and destroying threads as requests arrive and leave introduces overhead and increases latency. A thread pool ensures there is a cap on the number of threads in the system, which is important because a CPU can only execute a limited number of threads at a time (depending on the number of cores). Having too many threads leads to excessive waiting, more context switching, and higher overhead, causing the CPU to spend more time switching between threads rather than doing useful work. Additionally, having too many threads destroys cache locality, as the CPU frequently evicts cached data and loads new data every time a different thread gets scheduled.

### Where are these used in real life?

- Web servers  
- Game engines  
- Database engines  

---

### Design Goals

- Bounded number of worker threads  
- Task-based execution model  
- No busy-waiting  
- Safe shutdown via RAII  
- Exception-safe task execution  

---

### Components of a thread pool

1. **submit()** — Wraps an incoming task using the future mechanism and wakes a sleeping worker thread.  
2. **Queue** — Holds tasks until a worker becomes available.  
3. **Worker threads** — Execute tasks pulled from the queue.  
4. **Condition variable** — Allows worker threads to sleep without wasting CPU resources and wake up immediately when new tasks arrive. It acts as a communication channel between `submit()` and the worker threads.  
5. **Mutex** — Protects the task queue, ensuring that multiple threads do not push or pop tasks simultaneously and that submitters and workers do not modify the queue concurrently.  
6. **Shutdown flag** — Signals worker threads to stop waiting for new tasks and exit their execution loop during destruction.  

---

### Testing Strategy

| Test               | Purpose                            |
| ------------------ | ---------------------------------- |
| `test_basic`       | Correct task execution & futures   |
| `test_concurrency` | Parallel execution proof           |
| `test_shutdown`    | Safe destruction with active tasks |
| `test_exceptions`  | Exception propagation              |
| `stress_test`      | High-load correctness              |

---

### Build and Run Instructions

**Requirements**
- C++17-compatible compiler (tested with `g++`)
- POSIX-compatible system (Linux)
- `make`

**Build**
All builds are handled via the provided `Makefile`.

To see available targets:
```bash
make

---

### Limitations and Future Work

**Current limitations**

- Fixed-size thread pool (no dynamic resizing of worker threads)
- Single global task queue (no work-stealing or per-thread queues)
- FIFO scheduling only (no task priorities)
- No task cancellation support
- No explicit `wait_for_all()` or barrier API
- No CPU affinity or NUMA-aware scheduling

**Possible future extensions**

- Dynamic scaling of worker threads based on load
- Work-stealing queues to reduce contention
- Task prioritization
- Graceful task cancellation
- Barrier / synchronization primitives (e.g., `wait_for_all()`)
- Performance benchmarking and profiling

