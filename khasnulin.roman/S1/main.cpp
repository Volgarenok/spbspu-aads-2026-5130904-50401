#include <iostream>

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

  BiList< BiList< int > > transposed = getTransosedNumsSequences(allPairs);

  if (transposed.empty())
  {
    std::cout << "0\n";
    return 0;
  }

  printSequencesNumsByPlace(std::cout, transposed);

  printSumsOfSequences(std::cout, transposed);
}
