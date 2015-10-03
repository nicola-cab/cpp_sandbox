#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <future>

std::string flip(std::string s)
{
	std::reverse(s.begin(), s.end());
	return s;
}

int main(int argc, char** argv)
{
	std::vector<std::future<std::string>> v;
	auto f = [](std::string s)
	{
		return flip(s);
	};

	v.push_back(std::async(f," olleH"));
	v.push_back(std::async(f,"!!!dlroW"));
	v.push_back(std::async(f,"zyxwutsrqponmlkjihgfedcba"));

	for(auto&f : v)
		std::cout << f.get() << std::endl;

	return 0;
}
