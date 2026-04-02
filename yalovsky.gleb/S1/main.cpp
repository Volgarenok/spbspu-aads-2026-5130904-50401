#include "input-output.hpp"

#include <exception>
#include <iostream>

int main()
{
  yalovsky::SequenceList sequences;

  try
  {
    yalovsky::readSequences(std::cin, sequences);

    if (sequences.empty())
    {
      std::cout << "0\n";
      return 0;
    }
    
    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
}