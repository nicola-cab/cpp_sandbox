#include <cstddef>
#include <tuple>
#include <utility>
#include <type_traits>
#include <iostream>

template<typename F, typename T, size_t... S>
inline
auto apply(F&& fn, T&& t, std::index_sequence<S...>)
{
	return std::forward<F>(fn)(std::get<S>(std::forward<T>(t))...);
}

template<typename F, typename T>
inline
auto run_f(F&&fn, T&& t)
{
	constexpr std::size_t tSize = 
		std::tuple_size<typename std::remove_reference<T>::type>::value;

	return apply(std::forward<F>(fn), 
			std::forward<T>(t),
			std::make_index_sequence<tSize>());
}

int main(int argc, char** argv)
{

	auto f = [](int a, int b){return a+b;};
	auto t = std::make_tuple(10,20);
	auto sum = run_f(f,t);
	std::cout << sum << std::endl;
   
   return 0;
}
