Desing Task System   
===================

All this code has been partially copied and inspired by the talk given by Sean Parent on concurrency.

All this code is about implementing of a simple task system + task stealing. The task system has been implementend gradually, following 3 different increment approaches. 
These are 3 different implementations:
* only queue 1 producer and multiple consumers (1Q)
* one queue per thread 1 producer and multiple consumers (NQ)
* one queue per thread 1 producer and multiple consumers plus task stealing (NQS)

The queue used is not optimized. it is just a simple FIFO implementation using a std::deque.
All the operations that attempt to push/pop from the queue are atomic and protected using a 
lock.
The locking technique adopted has a corse granularity. There are no opmitimization on the queue.

Further steps will entitle optimizations applied to the queue in order to try to lock as less as possible.
Final goal is to replace the current queue with a lock free one in order to meausere the actual gain of using lock free algorithms

Fibonacci has been implemented using two different algorithm. The first one using DP, the second one copy the one presented in the book "From Mathematics to Generic Programming" that uses Egyptian Multiplication (honestly I have no idea on how it accomplishes fibonacci). 
Important to notice that both the algorithms were run with a spin factor of 100000 in order to have a well gauced result

#####OSX operating system 

| task system   | algorithm | N   |avg time per computation | All computation (N*SPIN_FACTOR)|
| --- |---| --- | ---       | --- |  ---   | ---  |  ---    |  ---    
Single thread   | dynamic programming      |  500 |  15 us  |  ~1.6   sec   
Single thread   | egyptian mul             |  500 |  418 ns |  0.041  sec 
 1Q             | dynamic programming      |  500 |  9 us   |  0.95   sec
 1Q             | egyptian mul             |  500 |  16 us  |  ~1.7   sec    
 NQ             | dynamic programming      |  500 |  4 us   |  0.35   sec
 NQ             | egyptian mul             |  500 |  1 us   |  ~0.17  sec
 NSQ            | dynamic programming      |  500 |  440 ns |  0.044   sec 
 NSQ            | egyptian mul             |  500 |  3 us   |  ~0.31   sec 
 


#####Windows operating system 

| task system   | algorithm | N  |avg time per computation | All computation (N*SPIN_FACTOR)|
| ---           |   ---     | ---|    --- |  ---  | ---    |  ---           
Single thread   | dynamic programming     |  500  |  76 us |  7.6  sec   
Single thread   | egyptian mul            |  500  |  5 us  |  0.54 sec        
1Q              | dynamic programming     |  500  |  8 us  |  0.86 sec
1Q              | egyptian mul            |  500  |  3 us  |  ~0.34 sec
NQ              | dynamic programming     |  500  |  17 us |  1.7 sec  
NQ              | egyptian mul            |  500  |  3 us  |  ~0.39 sec
NSQ             | dynamic programming     |  500  |  10 us |  ~1 sec     
NSQ             | egyptian mul            |  500  |  4 us  |  0.47 sec 


Conclusion
-------------

On windows the situation is quite bizarre and the less efficient task pool looks to outperform that one that has the work stealing implementation.
The egyptian multiplication algorithm to compute fibonacci looks to be the best algorithm to run on single thread that outperforms all the computation done using a task system (with and without task stealing).
At the same time the dynamic programming fibonacci is better to run on a task system that follows the work stealing approach rather than to be run as single threaded application. Looking into the number DP algorithm is less efficient than the egyptian multiplication, but DP runs better than Egyptian multiplication one on multiple corea.Even still the egyptian multiplication outperforms the task system plus work stealing. 
Odd but in most of the cases it happened that the egyptian multiplication algorithm executed on Windows, runnning on a task system with work stealing, ran slower than using a task system without work stealing. 
Essentially only for egyptian multiplication NQ outperformed NSQ and 1Q is the best pick.... this is true only on windows.




----------


