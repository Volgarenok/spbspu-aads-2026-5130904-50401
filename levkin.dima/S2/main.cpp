#include <exception>
#include <fstream>
#include <iostream>
#include "utils.hpp"

int main(int argc, char* argv[])
{
  std::ifstream file;
  if (argc > 1) {
    file.open(argv[1]);
    if (!file) {
      std::cerr << "bad file\n";
      return 1;
    }
  }
  std::istream& in = (argc > 1) ? file : std::cin;

  try {
    levkin::Stack< long long > results = levkin::parse(in);

    if (!results.empty()) {
      std::cout << results.top();
      results.pop();
    }
    while (!results.empty()) {
      std::cout << " " << results.top();
      results.pop();
    }
    std::cout << "\n";
  } catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}