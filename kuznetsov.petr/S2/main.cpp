#include <iostream>
#include <fstream>
#include "math_funcs.hpp"

int main(int argc, char* argv[])
{
  namespace kuz = kuznetsov;
  kuz::Stack< kuz::Queue< std::string > > mathExpressions;
  std::ifstream file;
  std::istream* source = &std::cin;
  if (argc > 1) {
    file.open(argv[1]);
    if (!file) {
      std::cerr << "Cant open file\n";
      return 1;
    }
    source = &file;
  }
  try {
    mathExpressions = kuznetsov::getExpressions(*source);
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
    res = kuz::calculateStackOfInfix(mathExpressions);
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

