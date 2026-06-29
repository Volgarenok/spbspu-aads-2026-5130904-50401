#include <fstream>
#include <iostream>
#include <string>
#include "math.hpp"
#include "postfix.hpp"
#include "queue.hpp"
#include "stack.hpp"

int main(int argc, char * argv[])
{
  namespace mad = madieva;
  std::ifstream file;
  std::istream * in = &std::cin;
  if (argc == 2) {
    file.open(argv[1]);
    if (!file.is_open()) {
      std::cerr << "file not open\n";
      return 1;
    }
    in = &file;
  } else if (argc > 2) {
    std::cerr  << "Not right size arguments\n";
    return 2;
  }
  mad::Stack< long long > res;
  std::string line;
  while (std::getline(*in, line)) {
    if (!line.empty()) {
      try {
        madieva::Queue< std::string > post = madieva::postfix(line);
        madieva::evaluateExpression(post, res);
      } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
      }
    }
  }
  if (!res.empty()) {
    std::cout << res.top();
    res.pop();
  }
  while (!res.empty()) {
    std::cout << " " << res.top();
    res.pop();
  }
  std::cout << "\n";
}
