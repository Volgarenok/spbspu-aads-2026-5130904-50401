#include <fstream>
#include <iostream>

#include <stack.hpp>
#include <queue.hpp>
#include "general_funcs.hpp"
#include "math_funcs.hpp"

int main(int args, char ** argv)
{
  using namespace chernov;

  Stack< long long > results;

  std::istream * input = &std::cin;
  if (args > 1) {
    static std::ifstream file_input(argv[1]);
    input = &file_input;
  }

  std::string line;
  while (*input) {
    std::getline(*input, line);
    Queue< std::string > math_expression = processLine(line);
    if (math_expression.size() == 0) {
      continue;
    }

    long long result = 0;
    try {
      result = calculateMathExpression(math_expression);
    } catch (const std::exception & e) {
      std::cerr << e.what() << "\n";
      return 1;
    }

    results.push(result);
  }

  bool first_result = true;
  while (!results.empty()) {
    if (first_result) {
      first_result = false;
    } else {
      std::cout << " ";
    }

    std::cout << results.top();
    results.pop();
  }
  std::cout << "\n";
}
