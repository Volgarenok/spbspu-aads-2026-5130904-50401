#include "functions.hpp"

int main()
{
  List<Sequence> sequences = read_sequences(std::cin);

  List<List<size_t>> transposed = transpose_sequences(sequences);

  print_sequences(transposed);

  List<size_t> sums = calculate_sums(transposed);

  print_sums(sums);

  return 0;
}
