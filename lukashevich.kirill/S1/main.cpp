#include <iostream>
#include <stdexcept>

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
    while (lukashevich::hasNum(copy)) {
      size_t sum = lukashevich::printOneRow(std::cout, copy);
      sums.pushBack(sum);
      std::cout << '\n';
    }
  } catch (const std::overflow_error&)
  {
    std::cout << '\n';
    std::cerr << "error\n";
    return 1;
  }

  lukashevich::printSum(std::cout, sums);
  std::cout << '\n';

  return 0;
}
