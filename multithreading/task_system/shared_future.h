#pragma once

#if 0 
#include <memory>
#include <utility>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <functional>

#include "task_system_sched_stealing.h"

static task_system_t g_system;

//return the result type of some function
template<typename>
struct result_of_;

template<typename R, typename... Args>
struct result_of_ < R(Args...) > { using type = R; };

template<typename F>
using result_of_t_ = typename result_of_<F>::type;


//forward declaration for classes
template<typename, typename...> class package_task_t;
template<typename> class future_t;
template<typename> struct shared_base_t;
template<typename, typename...> struct shared_t;

//definition of some function that is able to deceive which is the type of the function to execute and which is the return type
template<typename S, typename F>
auto package(F&& f)->std::pair < package_task_t<S>, future_t<result_of_t_<S>> >
{
   auto p = std::make_shared<shared_t<S>>(std::forward<F>(f));
   return std::make_pair(package_task_t<S>(p), future_t<result_of_t_<S>>(p));
}

template<typename F, typename... Args>
auto async(F&& f, Args&& ...args) {
   using result_type = result_of_t_ < F(Args...) >;
   //using package_type = package_task_t < result_type() > ;
   auto pack = package<result_type()>(bind(std::forward<F>(f), std::forward<Args>(args)...));
   g_system.async(std::move(std::get<0>(pack)));
   return std::get<1>(pack);
}

/*
* Implementation of my shared future
*/
template<typename R>
class future_t {
   std::shared_ptr<shared_base_t<R>> _p;

   template<typename S, typename F>
   friend auto package(F&& f)->std::pair < package_task_t<S>, future_t<result_of_t_<S>> >;

public:
   explicit future_t(std::shared_ptr<shared_base_t<R>> p) : _p(std::move(p))
   {}

   template<typename F>
   auto then(F&& f) {
      auto l = [p = _p, f = std::forward<F>(f)](){ return f(p->_r.back()); };
      auto pack = package<result_of_t_<F(R)>>(l);
   }

   const R& get() const { return _p->get(); };
};


/**
* Implementation for package task
*/
template<typename F, typename ...Args>
class package_task_t{
   std::weak_ptr<shared_t<F(Args...)>> _p;

   template<typename S, typename U>
   friend auto package(U&& u)->std::pair < package_task_t<S>, future_t<result_of_t_<S>> >;

public:
   explicit package_task_t(std::weak_ptr<shared_t<F(Args...)>> p) : _p(std::move(p))
   {}

   template<typename ...A>
   void operator()(Args&&... args) const
   {
      auto p = _p.lock();
      if (p) (*p)(std::forward<Args>(args)...);
   }
};

//Implementation of my shared object
template<typename R>
struct shared_base_t {
   std::vector<R> _r;
   std::mutex _mutex;
   std::condition_variable _ready;
   std::vector<std::function<void()>> _then;
protected:
   virtual ~shared_base_t(){}
   void set(R&& r) {
      std::vector<std::function<void()>> then;
      {
         lock_t lock{ _mutex };
         _r.push_back(std::move(r));
         _then.swap(then);
      }
      _ready.notify_all();
      for (const auto& f : then) //spawn a task
         ;
   }
   template <typename F>
   void then(F&& f) {
      bool resolved{ false };
      {
         lock_t lock{ _mutex };
         if (_r.empty()) _then.push_back(std::forward<F>(f));
         else resolved = true;
      }
      if (resolved) async(move(f));
   }
   const R& get() {
      lock_t lock{ _mutex };
      while (_r.empty()) _ready.wait(lock);
      return _r.back();
   }
};

template <typename R, typename... Args>
struct shared_t : public shared_base_t < R > {
   std::function<R(Args...)> _f;
public:
   template<typename F>
   explicit shared_t(F&&f) : _f(std::forward<F>(f))
   {}
   template<typename... A>
   void operator()(A&&...args) const {
      this->set(_f(std::forward<Args>(args)...));
      _f = nullptr;
   }
};

#endif
