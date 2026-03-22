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

  }

  if (mathExpressions.empty()) {
    return 0;
  }

  kuz::Queue< kuz::lli_t > res;
  kuz::calculate(mathExpressions, res);

  std::cout << res.front();
  res.pop();
  while (!res.empty()) {
    std:: cout << ' ' << res.front();
    res.pop();
  }
  std::cout << '\n';

}

