#include <fstream>
#include "stack.hpp"
#include "queue.hpp"

namespace donkeev
{
  void readFromFile(donkeev::Queue< donkeev::Queue< char > >& expressionsQueue, const char* filename)
  {
    std::ifstream input(filename);
    if (!input)
    {
      throw std::runtime_error("Can't open the file");
    }

    char test;
    while (input >> test)
    {
      if (test == ' ' || test == '\n')
      {
        continue;
      }
      if (input.eof())
      {
        break;
      }

      donkeev::Queue< char > symbolsQueue;
      symbolsQueue.push(test);
      input >> test;
      while (test != '\n')
      {
        if (input.eof())
        {
          break;
        }
        if (test != ' ')
        {
          symbolsQueue.push(test);
        }
        input >> test;
      }

      expressionsQueue.push(symbolsQueue);
    }
  }
  void readFromTerminal(donkeev::Queue< donkeev::Queue< char > >& expressionsQueue, std::istream& input)
  {
    char test;
    while (input >> test)
    {
      if (test == ' ' || test == '\n')
      {
        continue;
      }
      if (input.eof())
      {
        break;
      }

      donkeev::Queue< char > symbolsQueue;
      symbolsQueue.push(test);
      input >> test;
      while (test != '\n')
      {
        if (input.eof())
        {
          break;
        }
        if (test != ' ')
        {
          symbolsQueue.push(test);
        }
        input >> test;
      }

      expressionsQueue.push(symbolsQueue);
    }
  }
  void calculate()
  {
    
  }
}
