#include <iostream>
#include <string>
#include <fstream>
#include "general_functions.hpp"

using ll_t = long long;

int main(int argc, char ** argv)
{
  zharov::Stack< ll_t > results;
  try {
    if (argc > 1) {
      std::ifstream d(argv[1]);
      zharov::getResults(d, results);
    } else {
      zharov::getResults(std::cin, results);
    }
  } catch (const std::overflow_error & e) {
      std::cerr << e.what() << "\n";
      return 1;
    } catch (const std::logic_error & e) {
      std::cerr << e.what() << "\n";
      return 1;
    }
  bool is_first = true;
  while (!results.empty()) {
    if (!is_first) {
      std::cout << " ";
    } else {
      is_first = false;
    }
    std::cout << results.drop();
  }
  std::cout << "\n";
}
