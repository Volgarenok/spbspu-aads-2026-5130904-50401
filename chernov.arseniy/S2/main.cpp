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
  std::ifstream file_input;
  if (args == 2) {
    file_input.open(argv[1]);
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
