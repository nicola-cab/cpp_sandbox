#include <iostream>
#include <regex>
#include <string>


//regexes
const std::regex r_pixel {"(\\d{3,5})x(\\d{3,5})"}; //find dimension formatted strings (aka 100x100)
const std::regex r_name_surname{"(\\w+)\\s(\\w+)"}; //find all groups of name / surname
const std::regex r_find_comment{"#\\s*(\\S+)"}; //find all comments (aka lines begin with #)
const std::regex r_days{"\\w*day"}; //find all *day

//format strings
const std::string fmt{"$2,$1"}; //format string swap first group with second

//
// functional object
//

template<typename S, typename R>
struct Verify : std::binary_function<S,R,void>
{
	void operator()( const S& s, const R& r) const 
	{
		std::cout << std::boolalpha << std::regex_match(s,r) << std::endl;
	}
};

template<typename S, typename R>
struct Search : std::binary_function<S,R,void>
{
	void operator()(const S&s, const R&r) const 
	{
		std::smatch m;
		if( std::regex_match(s, m, r) ) 
			std::cout << m[0] << std::endl; 
	}
};

template<typename S, typename R>
struct Replace : std::binary_function<S,R,void>
{
	std::string _fmt;
	Replace(std::string fmt) : _fmt(fmt)
	{}

	void operator()(const S& s, const R& r) const 
	{
		std::cout << std::regex_replace(s,r,fmt) << std::endl;
	}
};

template <typename S, typename R>
struct Iterate : std::binary_function<S,R,void>
{
	void operator()( const S& s, const R& r) const 
	{
		const std::sregex_token_iterator end;
		for(std::sregex_token_iterator i{s.begin(), s.end(), r}; i!= end; ++i)
			std::cout << *i << std::endl;
	}
};


//
// utility 
//
template< typename S, //model a string
			 typename R, //model a regex
			 typename Op> //model a function object 
void exec(const S& s, const R&r, Op op) 
{
	op(s,r);
}

//
// main
//
int main(int argc, char** argv)
{
	for(std::string s; getline(std::cin, s);)
	{
		std::cout << " --------- " << std::endl;
		//exec(s,r_pixel, Verify<std::string, std::regex>{});
		//exec(s,r_find_comment, Search<std::string, std::regex>{});
		//exec(s,r_name_surname, Replace<std::string, std::regex>{fmt});
		exec(s,r_days, Iterate<std::string, std::regex>{});
	}

	return 0;
}
