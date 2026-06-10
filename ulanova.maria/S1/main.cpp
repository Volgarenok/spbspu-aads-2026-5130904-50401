#include <iostream>
#include <stdexcept>
#include "functions.hpp"

int main()
{
  ulanova::List<ulanova::Sequence> seqs = ulanova::read_sequences(std::cin);

  if (seqs.cbegin() == seqs.cend())
  {
    std::cout << "0\n";
    return 0;
  }

  ulanova::LCIter< ulanova::Sequence > name_it = seqs.cbegin();

  if (name_it != seqs.cend())
  {
    std::cout << name_it->name;
    ++name_it;
  }

  for (; name_it != seqs.cend(); ++name_it)
  {
    std::cout << " " << name_it->name;
  }
  std::cout << "\n";

  ulanova::List< ulanova::List< size_t > > transposed = transpose_sequences(seqs);
  if (transposed.cbegin() == transposed.cend())
  {
    std::cout << "0\n";
    return 0;
  }

  print_sequences(std::cout, transposed);
  std::cout << "\n";

  try
  {
    ulanova::List< size_t > sums = ulanova::calculate_sums(transposed);
    ulanova::print_sums(std::cout, sums);
    std::cout << "\n";
  }
  catch (const std::overflow_error& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }


  return 0;
}
