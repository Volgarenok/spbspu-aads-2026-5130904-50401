#include <fstream>
#include <iostream>

#include "func-stack-queue.hpp"
#include "stack.hpp"
#include "queue.hpp"
#include "func-math.hpp"

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

    zubarev::Stack< long long > results;
    std::string expression = "";
    while (std::getline(*input, expression)) {
      zubarev::Queue< std::string > infixQ = zubarev::detail::fromStrToQueue(expression);
      if (infixQ.empty()) {
        continue;
      }

      zubarev::Queue< std::string > postfixQ = zubarev::detail::fromInfixToPostfix(infixQ);
      results.push(zubarev::eval(postfixQ));
    }

    if (results.empty()) {
      std::cout << '\n';
      return 0;
    }
    std::cout << results.top();
    results.drop();
    while (!results.empty()) {
      std::cout << ' ' << results.top();
      results.drop();
    }

    std::cout << '\n';
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
