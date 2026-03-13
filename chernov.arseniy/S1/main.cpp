#include <iostream>

#include "list.hpp"
#include "liter.hpp"
#include "lciter.hpp"

int main()
{
  using namespace chernov;
  using pStrList = std::pair< std::string, List< size_t > >;
  List< pStrList > sequences;
  LIter< pStrList > seqs_iter = sequences.before_begin();

  std::string name;
  while (std::cin >> name) {
    List< size_t > sequence;
    LIter< size_t > seq_iter = sequence.before_begin();

    size_t num = 0;
    while (std::cin >> num) {
      seq_iter = sequence.insert_after(seq_iter, num);
    }

    seqs_iter = sequences.insert_after(seqs_iter, {name, sequence});
    if (std::cin.bad()) {
      std::cerr << "bad input\n";
      return 1;
    }
    std::cin.clear();
  }

  if (!std::cin.eof()) {
    std::cerr << "bad input\n";
    return 1;
  }

  if (sequences.empty()) {
    std::cout << 0 << "\n";
    return 0;
  }

  seqs_iter = sequences.begin();
  std::cout << seqs_iter->first;
  ++seqs_iter;
  while (seqs_iter != sequences.begin()) {
    std::cout << " " << seqs_iter->first;
    ++seqs_iter;
  }
  std::cout << "\n";
}
