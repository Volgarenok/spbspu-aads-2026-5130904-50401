#include <fstream>
#include <iostream>
#include "mathFunctions.hpp"

int main(int argc, char **argv)
{
  burukov::Stack< burukov::Queue< std::string > > infix;
  std::istream *inputStream = nullptr;
  std::ifstream fileStream;

  if (argc < 2)
  {
    inputStream = &std::cin;
  }
  else if (argc == 2)
  {
    fileStream.open(argv[1]);
    if (!fileStream)
    {
      std::cerr << "Cannot open file\n";
      return 1;
    }
    inputStream = &fileStream;
  }
  else
  {
    std::cerr << "Too many arguments\n";
    return 1;
  }

  try
  {
    burukov::getInfix(*inputStream, infix);
  }
  catch (const std::exception &e)
  {
    std::cerr << "Read error: " << e.what() << "\n";
    return 1;
  }

  if (infix.empty())
  {
    std::cout << "\n";
    return 0;
  }

  burukov::Queue< std::string > results;
  while (!infix.empty())
  {
    const burukov::Queue< std::string > inf = infix.top();
    infix.pop();
    try
    {
      std::string res = burukov::evaluateExpression(inf);
      results.push(res);
    }
    catch (const std::exception &exc)
    {
      std::cerr << exc.what() << "\n";
      return 1;
    }
  }

  std::cout << results.front();
  results.pop();
  while (!results.empty())
  {
    std::cout << " " << results.front();
    results.pop();
  }
  std::cout << "\n";

  return 0;
}
