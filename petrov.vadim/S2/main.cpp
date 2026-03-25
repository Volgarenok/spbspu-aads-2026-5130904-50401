#include <iostream>
#include <fstream>
#include "stack.hpp"
#include "foo.hpp"
#include "math.hpp"

int main(int argc, char* argv[])
{
  using namespace petrov;

  Stack< Queue<std::string> > infix;
  Stack< Queue<std::string> > postfix;
  Stack<ll> results;

  try
  {
    // Чтение входа
    if (argc > 1)
    {
      std::ifstream file(argv[1]);
      if (!file)
      {
        std::cerr << "Error: cannot open file\n";
        return 1;
      }
      getInfix(file, infix);
    }
    else
    {
      getInfix(std::cin, infix);
    }

    // Перевод в постфикс
    infixToPostfix(infix, postfix);

    // Вычисление
    while (!postfix.empty())
    {
      Queue<std::string> expr = postfix.drop();
      ll value = calculatePostfix(expr);
      results.push(value);
    }

    // Вывод
    printResults(std::cout, results);
  }
  catch (const std::overflow_error& e)
  {
    std::cerr << "Overflow error: " << e.what() << "\n";
    return 1;
  }
  catch (const std::invalid_argument& e)
  {
    std::cerr << "Math error: " << e.what() << "\n";
    return 1;
  }
  catch (const std::logic_error& e)
  {
    std::cerr << "Logic error: " << e.what() << "\n";
    return 1;
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}