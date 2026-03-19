#include "all.hpp"
#include <fstream>
#include <iostream>

int main(int argc, char const* argv[])
{
  std::string filename = "in.txt";

  if (argc > 1) {
    filename = argv[1];
  }

  std::ifstream file(filename);

  if (!file.is_open()) {
    std::cerr << "Ошибка: Не удалось открыть файл " << filename << '\n';
    std::cerr << "Текущая директория: ";

    return 1;
  }

  std::cout << "Обработка файла: " << filename << '\n';

  while (file.good() && !file.eof()) {
    bool error = false;
    std::string expression = zubarev::input(file, error);

    if (!expression.empty()) {
      std::cout << "Выражение: " << expression << '\n';
      zubarev::Queue< std::string > infixQ = zubarev::fromStrToQueue(expression);
      std::cout << "Infix: ";
      infixQ.print();

      zubarev::Queue< std::string > postfixQ = zubarev::fromInfixToPostfix(infixQ);
      std::cout << "Postfix: ";
      postfixQ.print();

      std::cout << "Result: ";
      std::cout << zubarev::evil(postfixQ) << '\n';
      std::cout << '\n';
    }
  }
}
