#include <iostream>
#include <string>
#include <utility>
#include "list.hpp"
#include "sequence_functions.hpp"
#include "iterator.hpp"
int main()
{
  using namespace malashenko;
  List< pair_t > data;
  try
  {
    getData(std::cin, data);
    if (data.empty())
    {
      std::cout << "0\n";
      return 0;
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  printSeqNames(std::cout, data);
  std::cout << '\n';


  List< List< size_t > > res;
  try
  {
    getTransedSeq(res, data);
    printNewSeqs(std::cout, res);
    List< size_t > sums;
    countSums(res, sums);
    if (sums.empty())
    {
      std::cout << "0\n";
      return 0;
    }
    printSums(std::cout, sums);
    std::cout << '\n';
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  return 0;
}
