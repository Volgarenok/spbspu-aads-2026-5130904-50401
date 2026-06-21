#include "math_funcs.hpp"

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
      if (!(line.empty())) {
        results.push(evaluate_expression(line));
      }
    }

    if (!results.empty()) {
      std::cout << results.top();
      results.pop();
    }

    while (!results.empty()) {
      std::cout << " " << results.top();
      results.pop();
    }

    std::cout << "\n";
  }
  catch (const std::exception& e) {
    std::cerr << "error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
