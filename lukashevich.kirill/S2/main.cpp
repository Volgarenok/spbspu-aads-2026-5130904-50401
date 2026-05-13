#include "math_funcs.hpp"
#include "../common/list.hpp"
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[])
{
  using namespace lukashevich;

  if (argc > 2) {
    std::cerr << "invalid arguments\n";
    return 1;
  }
  Stack< ll > results;
  try {
    std::istream* input = &std::cin;
    std::ifstream file;
    if (argc == 2) {
      file.open(argv[1]);
      if (!file.is_open()) {
        std::cerr << "error in open file\n";
        return 1;
      }
      input = &file;
    }

    std::string line;
    while (std::getline(*input, line)) {
      if (line.empty()) {
        continue;
      }
      results.push(evaluate_expression(line));
    }

    bool first = true;
    while (!results.empty()) {
      if (!first) {
        std::cout << " ";
      }
      std::cout << results.drop();
      first = false;
    }
    std::cout << "\n";
    return 0;
  }
  catch (const std::exception& e) {
    std::cerr << "error: " << e.what() << "\n";
    return 1;
  }
}