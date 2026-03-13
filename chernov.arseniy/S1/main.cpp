#include <iostream>

#include "list.hpp"
#include "liter.hpp"
#include "lciter.hpp"

int main()
{
  using namespace chernov;
  using pStrList = std::pair< std::string, List< size_t > >;
  using pLCIterSize = std::pair< LCIter< size_t >, size_t >;
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

  List < pLCIterSize > iters;
  LIter< pLCIterSize > iters_iter = iters.before_begin();

  bool first_name = true;
  seqs_iter = sequences.begin();
  do {
    if (!first_name) {
      std::cout << " ";
    } else {
      first_name = false;
    }

    std::cout << seqs_iter->first;
    if (!seqs_iter->second.empty()) {
      iters_iter = iters.insert_after(iters_iter, {seqs_iter->second.cbegin(), seqs_iter->second.size()});
    }
  } while (++seqs_iter != sequences.begin());
  std::cout << "\n";

  bool exist_nums = true;
  while (exist_nums) {
    iters_iter = iters.begin();
    bool first_num = true;
    do {
      pLCIterSize & pair = *iters_iter;

      if (pair.second > 0) {
        if (!first_num) {
          std::cout << " ";
        } else {
          first_num = false;
        }

        std::cout << *pair.first;
        ++pair.first;
        --pair.second;
      }
    } while (++iters_iter != iters.begin());

    if (first_num) {
      exist_nums = false;
    } else {
      std::cout << "\n";
    }
  }
}
