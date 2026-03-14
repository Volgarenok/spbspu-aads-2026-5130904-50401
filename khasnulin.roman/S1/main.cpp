#include <cstddef>
#include <exception>
#include <iostream>
#include <stdexcept>

#include "Sequences.hpp"

int main()
{
  using namespace khasnulin;
  bilist_row_pairs allPairs = readAll(std::cin);

  if (allPairs.empty())
  {
    std::cout << "0\n";
    return 0;
  }

  printSequenceNames(std::cout, allPairs);

  BiList< BiList< size_t > > transposed = getTransosedNumsSequences(allPairs);

  if (transposed.empty())
  {
    std::cout << "0\n";
    return 0;
  }

  printSequencesNumsByPlace(std::cout, transposed);

  try
  {
    printSumsOfSequences(std::cout, transposed);
  }
  catch (const std::overflow_error &e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
  catch (const std::exception &e)
  {
    std::cerr << "Unknown error: " << e.what() << "\n";
    return 1;
  }
}
