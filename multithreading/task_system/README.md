Desing Task System   
===================

All this code has been partially copied and inspired by the talk given by Sean Parent on concurrency.

All this code is about implementing of a simple task system + task stealing. The task system has been implementend gradually, following 3 different incremental approaches. 
These are 3 different implementations:
* only queue 1 producer and multiple consumers (1Q)
* one queue per thread 1 producer and multiple consumers (NQ)
* one queue per thread 1 producer and multiple consumers plus task stealing (NQS)

In first instance the queue used is not optimized. it is just a simple FIFO implementation using a std::deque.
All the operations that attempt to push/pop from the queue are atomic and protected using a 
lock.
The locking technique adopted has a corse granularity. There are no opmitimization on the queue.

Further steps entitled adding optimizations changing the underneath queue used in order to try to lock as less as possible.
Final goal is to replace the current queue with a lock free one in order to meausere the actual gain of using lock free algorithms

All the test are conducted on 2 different machines:
* MacBook Air (core I5 + 8GB ram)
* Windows7 (core i7 + 16GB ram)

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

####Using a crafted queue + fine grain locking
-------------
Rather then using a STL container I tried out with a crafted lock based queue, using as a base the one described in the book "concurrency in action" by  Anthony Williams. I replace the queue in all the task systems implemented with this new one.

The results that I got are slighly different that what I got before, the interesting part is that the tests have showed off that using a fine grain locking approach helped more those task systems that were not using the task stealing approach.

For simplicity the result on Windows are not reported here, but what I observed is that, again on windows the overall computation resulted slower than on OSX, moreover the fine grain locking queue had less impact on the task system implementing task stealing approach and made the other 2 task systems (without task stealing) closing the gap with the former. On both OSs but particularly on Windows, sometimes the task stealing system that was using the fine locking queue resulted slower than the one using a simple STL container with coarse locking.

Remarkable is the impact of using the fine locked queue that is implemented underneath like a single linked list. In order to avoid to allocate often to make room for new slots in the queue, I tried to run the same experiment on a slighly different fine grain locking queue that allowed me to reserve in advance a bunch of slots per each queue per thread. 

Reusing these slots rather than allocate memory every time. The overall performance achieved using this queue is however neglectable for the task stealing system, but all the other task pools got a good boost, sometimes they were as twice as faster as they were before or more. 
The intersting part is that accordingly with the threading schedulation the 1Q task system with grain locking outperformed by a factor 2 the task stealing one.
Here the results that I got.

#####OSX operating system (fine grain locking) 

| task system   | algorithm | N   |avg time per computation  | All computation (N*SPIN_FACTOR)|
| --- |---| --- | ---       | --- |  ---   | ---  |  ---     |  ---    
Single thread   | dynamic programming      |  500 |  ~19 us  |  ~1.9   sec   
Single thread   | egyptian mul             |  500 |  595 ns  |  ~0.06 sec 
1Q             | dynamic programming      |  500 |   ~5 us   |  ~0.54 sec
1Q             | egyptian mul             |  500 |   ~5 us   |  ~0.45  sec     *1    
NQ             | dynamic programming      |  500 |    ~5 us  |   0.49  sec   
NQ             | egyptian mul             |  500 |    ~2 us  |  ~0.25  sec
NSQ            | dynamic programming      |  500 |    ~4 us  |  ~0.39  sec  
NSQ            | egyptian mul             |  500 |    ~2 us   | ~0.25  sec 


#####OSX operating system (fine grain locking + reserve slots)

| task system   | algorithm | N   |avg time per computation  | All computation (N*SPIN_FACTOR)|
| --- |---| --- | ---       | --- |  ---   | ---  |  ---     |  ---    
Single thread   | dynamic programming      |  500 |  16 us   |  ~1.6   sec   
Single thread   | egyptian mul             |  500 |  616 ns  |  ~0.062 sec 
1Q             | dynamic programming      |  500 |   ~903 ns |  ~0.1 sec      *1 
1Q             | egyptian mul             |  500 |   ~1 us   |  ~0.35  sec    
NQ             | dynamic programming      |  500 |    ~3 us  |  ~0.37  sec   
NQ             | egyptian mul             |  500 |    2 us   |  ~0.18  sec
NSQ            | dynamic programming      |  500 |    ~4 us  |  ~0.4  sec      *2
NSQ            | egyptian mul             |  500 |    ~3 us  |  ~0.32 sec      *2


* 1 stricly dependant of order of threading schedulation (not always reproducible)
* 2 increased granurality of the lock in order to reduce number of allocations


Conclusion
-------------

On windows the situation is quite bizarre and the less efficient task pool looks to outperform that one that has the work stealing implementation.

The egyptian multiplication algorithm to compute fibonacci looks to be the best algorithm to run on single thread that outperforms all the computation done using a task system (with and without task stealing).

DP algorithm is less efficient than the egyptian multiplication, but DP runs better than Egyptian multiplication on multicore machines. Even still the egyptian multiplication outperforms the DP one that runs on top of any task system implemented.

The bests result gotten are:
* Single thread, fib(500) computed 100000 times, total time 0.041 (Egyptian multiplication)
* Task stealing, fib(500) computed 100000 ties, total time 0.044  (Dynamic programming)

Important to notice that Dynamic programming algorithm run on a single thread application is roughly 25 times slower than Egyption multiplication one. This means that the task stealing approach brought an algorithm that was quite slow to be computed in more or less the same time of a very efficient algorithm, just exploiting the power of a multicore cpu (this is valid on OSX only though).

On Windows essentially it is possible to say that for egyptian multiplication NQ task pool outperformed NSQ one. Weird but 1Q task system outperforms the other 2 task systems and then it could be considered is the best pick.

On Mac things are quite different though, basic implementation of a task stealing thread pool revealed that it outperforms all the other task systems. But as soon I changed the stl std::queue with a crafted queue with fine locking implementation the performances of the task stealing system decreased and the other task systems caught up the task stealing one. This is most probably due to mine crafted queue that is not as efficient as the stl one and how much is the crafted queue cache friendly compared to the stl one.




----------


