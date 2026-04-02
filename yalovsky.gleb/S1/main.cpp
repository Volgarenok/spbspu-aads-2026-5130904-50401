#include "input-output.hpp"

#include <exception>
#include <iostream>

int main()
{
  yalovsky::SequenceList sequences;

  try
  {
    yalovsky::readSequences(std::cin, sequences);
    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
}