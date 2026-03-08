#include <utility>
#include <string>
#include <iostream>
#include "list.hpp"
#include "functions.hpp"
#include "iterator.hpp"
int main()
{
  
  namespace mal = malashenko;
  using pair_t = std::pair<std::string, mal::List< int > >;

  mal::List< pair_t > data;
  
  try
  {
    mal::getData(std::cin, data);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }

  try
  {
    mal::printSeqNames(std::cout, data);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  
  mal::List< int > sums;

  try
  {
    printSeqsAndGetSums(std::cout, data, sums);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  
  printSums(std::cout, sums);
  return 0;

}
