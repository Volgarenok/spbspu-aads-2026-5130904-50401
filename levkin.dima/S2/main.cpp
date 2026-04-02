#include "stack.hpp"
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

    levkin::Stack< int > results;
    std::string line;

    using Block = int;
    levkin::Stack< Block > a;
    while (std::getline(in, line)) {
      if (line.empty())
        continue;
      std::cout << line;
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}

