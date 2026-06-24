#include "commands.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>

int main(int argc, char * argv[])
{
  if (argc > 2) {
    std::cerr << "invalid arguments\n";
    return 1;
  }

  try {
    lukashevich::MatrixTable table;
    std::istream * input = &std::cin;
    std::ifstream file;

    if (argc == 2) {
      file.open(argv[1]);
      if (!file.is_open()) {
        std::cerr << "file open error\n";
        return 1;
      }
      input = &file;
    }

    std::string line;
    while (std::getline(*input, line)) {
      lukashevich::executeCommand(line, table, std::cout);
    }

    if (input->bad()) {
      std::cerr << "input error\n";
      return 1;
    }
  } catch (const std::exception & e) {
    std::cerr << e.what() << '\n';
    return 2;
  }

  return 0;
}
