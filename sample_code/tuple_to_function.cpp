#include <tuple>
#include <memory>
#include <iostream>
#include <string>

template<typename F>
class FunctorObj 
{
protected:
	F _f;
public:
	FunctorObj(F f) : _f(std::move(f))
	{}	

   virtual ~FunctorObj() = default;
   
   virtual void toArgs() = 0;
   
   template<typename... Args>
   auto getArgs(Args&&... args )
   {
      return std::make_tuple(std::forward<Args>(args)...);
   }
};

template<typename F>
class ImplFunctorObj : public FunctorObj<F>{
   std::string _name;
   int _num;
public:

   ImplFunctorObj(F f, std::string name, int num) : FunctorObj<F>(f),
																	 _name(std::move(name)), 
																	 _num(num)
   {}
   
   virtual void toArgs()
   {
      auto t = getArgs(_name, _num);
      
      std::cout << std::get<0>(t) << " - " << std::get<1>(t) << std::endl;
   }
};


///
/// @brief: Takes a function and a list of params and executes f(args...)
/// @param F : function to exec
/// @param Args: arguments to pass to f
///

/*
class Functor {
   template<typename F, typename... Args  >
   Functor( F f, Args&&... args ) {
      auto argtuple = std::make_tuple(std::forward<Args>(args)...);
      func(insert_magic_here(argtuple));
   }
};
*/



int main(int argc, char** argv)
{
   std::unique_ptr<FunctorObj> ptr = std::make_unique<ImplFunctorObj>("hello", 10);
   ptr->toArgs();

   
   return 0;
}
