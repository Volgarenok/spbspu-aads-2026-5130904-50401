#include "functions.hpp"
#include <iostream>

int main()
{
  List<Sequence> seqs = read_sequences(std::cin);

  if (seqs.cbegin() == seqs.cend())
  {
    std::cout << "0\n";
    return 0;
  }
  bool first = true;
  for (LCIter<Sequence> it = seqs.cbegin(); it != seqs.cend(); ++it)
  {
    if (!first)
    {
      std:: cout << " " ;
    }
    std::cout << it->name;
    first = false;
  }
  std::cout << "\n";

  List<List<size_t>> transposed = transpose_sequences(seqs);
  if (transposed.cbegin() == transposed.cend())
  {
    std::cout << "0\n";
    return 0;
  }

  print_sequences(transposed);

  List<size_t> sums = calculate_sums(transposed);

  print_sums(sums);

  return 0;
}
