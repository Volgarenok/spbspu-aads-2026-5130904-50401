#include "functions.hpp"
#include <iostream>

int main()
{
  List<Sequence> seqs = read_sequences(std::cin);

  if (!(seqs.cbegin() != seqs.cend()))
  {
    std::cout << "0\n";
    return 0;
  }

  for (LCIter<Sequence> it = seqs.cbegin(); it != seqs.cend(); ++it)
  {
    std::cout << it->name;
    if (++LCIter<Sequence>(it) != seqs.cend())
    {
      std::cout << " ";
    }
  }
  std::cout << "\n";

  List<List<size_t>> transposed = transpose_sequences(seqs);

  print_sequences(transposed);

  List<size_t> sums = calculate_sums(transposed);

  print_sums(sums);

  return 0;
}
