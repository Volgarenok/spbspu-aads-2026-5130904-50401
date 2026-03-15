#include "functions.hpp"
#include <iostream>

int main()
{
  ulanova::List<ulanova::Sequence> seqs = ulanova::read_sequences(std::cin);

  if (seqs.cbegin() == seqs.cend())
  {
    std::cout << "0\n";
    return 0;
  }
  bool first = true;
  for (ulanova::LCIter<ulanova::Sequence> it = seqs.cbegin(); it != seqs.cend(); ++it)
  {
    if (!first)
    {
      std:: cout << " " ;
    }
    std::cout << it->name;
    first = false;
  }
  std::cout << "\n";

  ulanova::List<ulanova::List<size_t>> transposed = transpose_sequences(seqs);
  if (transposed.cbegin() == transposed.cend())
  {
    std::cout << "0\n";
    return 0;
  }

  print_sequences(transposed);

  ulanova::List<size_t> sums = calculate_sums(transposed);

  print_sums(sums);

  return 0;
}
