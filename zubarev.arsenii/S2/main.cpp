#include "func-stack-queue.hpp"
#include <fstream>
#include <iostream>

int main(int argc, char const* argv[])
{

  try {
    std::ifstream file;
    std::istream* input = &std::cin;
    if (argc > 1) {
      file.open(argv[1]);
      if (!file.is_open()) {
        std::cerr << "Ошибка: Не удалось открыть файл " << argv[1] << '\n';
        return 1;
      }
      input = &file;
    }
    zubarev::run(*input, std::cout);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
