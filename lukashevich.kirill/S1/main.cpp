#include <iostream>

#include "list.hpp"
#include "functions.hpp"

int main()
{
  lukashevich::List< lukashevich::pair_t > data;

  lukashevich::getData(std::cin, data);

  if (data.empty()) {
    std::cout << 0 << '\n';
    return 0;
  }

  lukashevich::printSeqName(std::cout, data);
  std::cout << '\n';

  lukashevich::List< lukashevich::pair_t > copy(data);
  lukashevich::List< size_t > sums;

  try
  {
    lukashevich::printAll(copy, sums);
  }
  catch (const std::overflow_error&)
  {
    std::cerr << "erorr\n";
    return 1;
  }

  lukashevich::printSum(std::cout, sums);
  std::cout << '\n';

  return 0;
}
