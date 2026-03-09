#include <utility>
#include <string>
#include <iostream>
#include "list.hpp"
#include "functions.hpp"
#include "iterator.hpp"
int main()
{
  using namespace malashenko;
  using pair_t = std::pair< std::string, List< int > >;

  List< pair_t > data;
  try
  {
    getData(std::cin, data);
    if (data.empty())
    {
      std::cout << "0\n";
      return 0;
    }
  } catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  printSeqNames(std::cout, data);

  List< int > sums;
  try
  {
    printSeqsAndGetSums(std::cout, data, sums);
    if (sums.empty())
    {
      std::cout << "0\n";
      return 0;
    }
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }

  printSums(std::cout, sums);
}
