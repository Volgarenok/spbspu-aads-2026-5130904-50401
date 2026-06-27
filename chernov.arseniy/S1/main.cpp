#include <iostream>
#include <limits>

#include <list.hpp>
#include <liter.hpp>
#include <lciter.hpp>

int main()
{
  using namespace chernov;
  using pStrList = std::pair< std::string, List< size_t > >;
  using pLCIterSize = std::pair< LCIter< size_t >, size_t >;
  List< pStrList > sequences;
  LIter< pStrList > seqs_iter = sequences.beforeBegin();
  size_t MAX_SIZE_T = std::numeric_limits< size_t >::max();

  std::string name;
  while (std::cin >> name) {
    List< size_t > sequence;
    LIter< size_t > seq_iter = sequence.beforeBegin();

    size_t num = 0;
    while (std::cin >> num) {
      seq_iter = sequence.insertAfter(seq_iter, num);
    }

    seqs_iter = sequences.insertAfter(seqs_iter, {name, sequence});
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
  LIter< pLCIterSize > iters_iter = iters.beforeBegin();

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
      iters_iter = iters.insertAfter(iters_iter, {seqs_iter->second.cbegin(), seqs_iter->second.size()});
    }
  } while (++seqs_iter != sequences.begin());
  std::cout << "\n";

  List< size_t > sums;
  LIter< size_t > sums_iter = sums.beforeBegin();

  bool exist_nums = true;
  bool exist_lines = false;
  bool is_overflow = false;
  while (exist_nums) {
    iters_iter = iters.begin();
    bool first_num = true;
    size_t sum = 0;
    do {
      pLCIterSize & pair = *iters_iter;

      if (pair.second > 0) {
        if (!first_num) {
          std::cout << " ";
        } else {
          first_num = false;
        }

        if (!is_overflow) {
          if (MAX_SIZE_T - *pair.first < sum) {
            is_overflow = true;
          } else {
            sum += *pair.first;
          }
        }

        std::cout << *pair.first;
        ++pair.first;
        --pair.second;
      }
    } while (++iters_iter != iters.begin());

    if (first_num) {
      exist_nums = false;
    } else {
      exist_lines = true;
      sums_iter = sums.insertAfter(sums_iter, sum);
      std::cout << "\n";
    }
  }

  if (!exist_lines) {
    std::cout << 0 << "\n";
    return 0;
  }

  if (is_overflow) {
    std::cerr << "overflow error\n";
    return 1;
  }

  sums_iter = sums.begin();
  bool first_num = true;
  do {
    if (!first_num) {
      std::cout << " ";
    } else {
      first_num = false;
    }

    std::cout << *sums_iter;
  } while (++sums_iter != sums.begin());
  std::cout << "\n";
}
