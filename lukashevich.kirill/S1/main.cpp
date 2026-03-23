#include "list.hpp"
#include "functions.hpp"
#include <iostream>

int main()
{
  lukashevich::List< lukashevich::pair_t > data;

  lukashevich::getData(std::cin, data);

  if (data.empty()) {
    std::cout << 0 << '\n';
    return 0;
  }

  lukashevich::printSeqName(data);

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

  lukashevich::printSum(sums);

  return 0;
}
