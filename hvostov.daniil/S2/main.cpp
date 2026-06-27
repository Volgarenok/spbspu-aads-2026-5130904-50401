#include <fstream>
#include <iostream>
#include "arifmetic.hpp"
#include <stack.hpp>

int main(int argc, char** argv)
{
  hvostov::Stack< long long int > result;
  std::istream* input_stream = nullptr;
  std::ifstream file_stream;

  if (argc > 1) {
    file_stream.open(argv[1]);
    if (!file_stream.is_open()) {
      std::cerr << "Не удалось открыть файл: " << argv[1] << "\n";
      return 1;
    }
    input_stream = &file_stream;
  } else {
    input_stream = &std::cin;
  }

  try {
    std::string line;
    while (std::getline(*input_stream, line)) {
      if (line.empty()) {
        continue;
      }
      long long int answer = hvostov::calculateMathExpression(line);
      result.push(answer);
    }
  } catch (...) {
    std::cerr << "Uncorrect math expression!\n";
    return 1;
  }

  if (!result.empty()) {
    std::cout << result.top();
    result.pop();
  }
  while (!result.empty()) {
    std::cout << " " << result.top();
    result.pop();
  }
  std::cout << "\n";
  return 0;
}
