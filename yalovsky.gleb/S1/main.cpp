#include <exception>
#include <iostream>

#include "input-output.hpp"

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

    yalovsky::printNames(std::cout, sequences);
    std::cout << '\n';

    yalovsky::Matrix matrix;
    yalovsky::transposeSequences(sequences, matrix);
    yalovsky::printMatrix(std::cout, matrix);

    yalovsky::NumberList sums;
    yalovsky::calculateSums(matrix, sums);

    if (sums.empty())
    {
      std::cout << "0\n";
    }
    else
    {
      yalovsky::printNumberList(std::cout, sums);
      std::cout << '\n';
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }

  return 0;
}
