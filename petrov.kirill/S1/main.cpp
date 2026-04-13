#include <iostream>
#include "list.hpp"
#include <string>

int main()
{
	petrov::List<std::pair<std::string, petrov::List<int>>> list_for_sol;
	std::string fill_str, s = "";
	while (std::getline(std::cin, fill_str) && !std::cin.eof())
	{
		s += " ";
		s += fill_str;
	}
	std::cout <<	s <<	std::endl;
}