#include "stack.hpp"
#include "utils.hpp"
#include <exception>
#include <fstream>
#include <iostream>
int main(int argc, char* argv[])
{
  try {

    std::ifstream file;
    if (argc > 1) {
      file.open(argv[1]);
      if (!file) {
        std::cerr << "bad file\n";
        return 1;
      }
    }
    std::istream& in = (argc > 1) ? file : std::cin;
    auto results = levkin::parse(in);
    while (!results.empty()) {
      std::cout << results.drop() << (results.empty() ? "" : " ");
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
