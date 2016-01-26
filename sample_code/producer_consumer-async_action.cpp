#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <future>
#include <mutex>

struct Functor {
	int operator()(int i) const 
	{
		return i;
	}
};

std::mutex m;
std::queue<Functor> q;


void runAction(Functor task, int i)
{
  auto f = std::async(task, i); //deposit the result wherever it is needed
  std::cout << f.get() << "\n";
}


void producer(Functor task)
{
   int i = 10;
	//very ineficient loop.. this melts the cpu (in production use a proper message queue with condictional variables)
   while (i-- > 0)
   {
      std::lock_guard<std::mutex> lk{ m }; 
      q.push(std::move(task));
      std::cout << "Pushed -->" << i << "\n";
   }
  
}

void consumer()
{
   Functor task;
   int i = 10;

	//very ineficient loop.. this melts the cpu (in production use a proper message queue with condictional variables)
   while (i-- > 0)
   {
      std::lock_guard<std::mutex> lk{ m }; 

      {
         if (q.empty() == false)
         {
            task = std::move(q.front());
            std::cout << "Popped -->" << i << "\n";
            q.pop();
         }
         
      }
      
      runAction(std::move(task), i);

   }
      
}

int main(int argc, char* argv[])
{
	Functor f;
   std::vector<std::thread> tasks;
   tasks.emplace_back(producer, f);
   tasks.emplace_back(consumer);

   //to run in the dtor
   for (auto& task : tasks)
      task.join();
  

	return 0;
}

