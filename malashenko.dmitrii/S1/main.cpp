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

  List< List< int > > res;
  try
  {
    getTransedSeq(res, data);
    printNewSeqs(std::cout, res);
    List< int > sums;
    countSums(res, sums);
    if (sums.empty())
    {
      std::cout << "0\n";
      return 0;
    }
    printSums(std::cout, sums);
  } catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  return 0;
}
