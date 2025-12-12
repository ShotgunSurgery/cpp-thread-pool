## what is a thread pool anyway ? 
A fix number of pre created threads that are initilized at the start of the program, then they sleep in a loop waiting for task and as a task arrivers it is pushed into a queue and a thread is woken up and assigned to one of the task and this keeps on happening till the pool is instructed to shutdown. Doing so solves a couple of problems - first of all creating and destroying threads as processes arrive and leave causes overhead and increases latency, also this ensures there is a cap to the number of threads in the system as a cpu can only run a specific number of threads at a time (depending upon the no. of cores) so the rest of them need to wait, more threads means more waiting means more switching means more overhead and the CPU spends more time in switching rather than actually doing the work. Also having too many threads destroyes cache locality as the cpu dumps cached data and loads new data every time a new thread gets scheduled.

where are these used in real life ?
- web servers 
- game engines
- database engines etc ...
