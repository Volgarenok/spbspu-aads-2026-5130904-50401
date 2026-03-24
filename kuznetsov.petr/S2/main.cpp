#include <iostream>
#include <fstream>
#include "math_funcs.hpp"
#include "../common/includes/stack.hpp"
#include "../common/includes/queue.hpp"

int main(int argc, char* argv[])
{
  namespace kuz = kuznetsov;
  kuz::Stack< kuz::Queue< std::string > > mathExpressions;
  try {
    if (argc > 1) {
      std::ifstream d(argv[1]);
      if (!d) {
        std::cerr << "Cant open file\n";
        return 1;
      }
      kuz::getExpressions(d, mathExpressions);
    } else {
      kuz::getExpressions(std::cin, mathExpressions);
    }
  } catch (...) {
    std::cerr << "Input error\n";
    mathExpressions.clear();
    return 1;
  }

  if (mathExpressions.empty()) {
    std::cout << '\n';
    return 0;
  }

  kuz::Queue< kuz::lli_t > res;

  try {
    kuz::calculate(mathExpressions, res);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }

  std::cout << res.front();
  res.pop();
  while (!res.empty()) {
    std:: cout << ' ' << res.front();
    res.pop();
  }
  std::cout << '\n';

  return 0;
}

