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
}
