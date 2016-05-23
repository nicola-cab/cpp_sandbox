Desing Task System   
===================

All this code has been partially copied and inspired by the talk given by Sean Parent on concurrency.

Implementation of a simple task system + task stealing. The task system is implementend gradually, following different approaches. 
There are 3 different implementations for this task system:
1. only queue 1 producer and multiple consumers (1Q)
2. one queue per thread 1 producer and multiple consumers (NQ)
3. one queue per thread 1 producer and multiple consumers plus task stealing (NQS)

The queue used is not optimized. it is just a simple FIFO implementation using a std::deque.
All the operations that attempt to push/pop from the queue are atomic and protected using a 
lock.
The locking technique adopted has a corse granularity. There are no opmitimization on the queue.

Further steps will entitle optimizations applied to the queue in order to try to lock as less as possible.
Final goal is to replace the current queue with a lock free one in order to meausere the actual gain of using lock free algorithms

Fibonacci has been implemented using two different algorithm. The first one using DP, the second one copy the one presented in the book "From Mathematics to Generic Programming" that uses Egyptian Multiplication (honestly I have no idea on how it accomplishes fibonacci). 
Important to notice that both the algorithms were run with a spin factor of 100000 in order to have a well gauced result

#####OSX operating system 

| task system   | algorithm | N |avg time per computation | All computation (N*SPIN_FACTOR)|
| --- |---| ---| --- |  --- |  ---   | ---      
Single thread   |    dynamic programming   |  500 |  15us  |  ~1.6   sec   
Single thread   |    egyptian mul          |  500 | 418ns  |  0.041  sec 
 1Q             |  dynamic programming     |  500 |  9us   |  0.95   sec
 1Q             |  egyptian mul            |  500 |  16us  |  ~1.7   sec    
 NQ             |  dynamic programming     |  500 |  4us   |  0.35   sec
 NQ             | egyptian mul             |  500 |  1 us  |  ~0.17  sec
 NSQ            | dynamic programming      |  500 | 440 ns |  0.044   sec 
 NSQ            | egyptian mul             |  500 | 3us    |  ~0.31   sec 
 


#####Windows operating system 

| task system   | algorithm | N |avg time per computation | All computation (N*SPIN_FACTOR)|
| ---           |   ---    | ---|    ---                  | ---   
Single thread   |    dynamic programming   |  500 |       |   
Single thread   |    egyptian mul          |  500 |       |         
1Q             |  dynamic programming     |  500 |        |  
1Q             |  egyptian mul            |  500 |        |
NQ             |  dynamic programming     |  500 |        |  
NQ             | egyptian mul             |  500 |        | 
NSQ            | dynamic programming      |  500 |        |     
NSQ            | egyptian mul             |  500 |        |   


Conclusion
-------------

On windows the situation is quite bizarre and the less effective task pool looks to outperform that one that has the work stealing implementation.
The egyptian multiplication algorithm to compute fibonacci looks like the best algorithm to run on single thread that outperforms all the computation done using a task system (with and without task stealing).
At the same time the dynamic programming fibonacci is better to run on a task system that is implementing the work stealing approach rather than as single threaded implementation, that it looks like dp solution is more adpated to be run on multicore machines than the egyptina multiplication.
Even still the egyptian multiplication outperforms the task system plus work stealing. Odd but in most of the cases it happened that the egyptian multiplication run on a task system with work stealing run slower than the same algorithm run on a task system without work stealing, essentially only for egyptian multiplication NQ outperformed NSQ.




----------


