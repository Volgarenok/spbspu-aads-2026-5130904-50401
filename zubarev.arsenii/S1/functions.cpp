#include "all.hpp"
#include <iostream>
#include <limits>
#include <string>

namespace zubarev
{
  List< Data > input(std::istream& in, bool& error)
  {
    List< Data > list;
    LIter< Data > itList = list.before_begin();
    Data value;

    while (in >> value.name) {
      List< size_t > nums;
      LIter< size_t > itNum = nums.before_begin();
      std::string num;

      while (true) {

        while (in.peek() == ' ' || in.peek() == '\t') {
          in.get();
        }

        if (in.peek() == '\n' || in.eof()) {
          if (in.peek() == '\n') {
            in.get();
          }
          break;
        }

        if (in >> num) {
          if (isdigit(num)) {
            std::string maxStr = std::to_string(std::numeric_limits< size_t >::max());

            if (num.size() < maxStr.size()) {

              nums.insert_after(itNum, to_size_t(num));
              ++itNum;
            } else if (num.size() == maxStr.size()) {

              if (num <= maxStr) {
                nums.insert_after(itNum, to_size_t(num));
                ++itNum;
              } else {
                error = true;
                std::cerr << "input: number overflow" << '\n';
                return List< Data >{};
              }
            } else {

              error = true;
              std::cerr << "input: number overflow" << '\n';
              return List< Data >{};
            }
          } else {
            error = true;
            std::cerr << "input: incorrect input" << '\n';
            return List< Data >{};
          }
        } else {
          error = true;
          std::cerr << "input: bad input" << '\n';
          return List< Data >{};
        }
      }
      value.numbers = nums;
      list.insert_after(itList, value);
      ++itList;
    }
    return list;
  }
  template < class T >
  void output(std::ostream& out, T el, bool is_first)
  {
    if (is_first) {
      out << el;
    } else {
      out << " " << el;
    }
  }

  size_t output_names(List< Data >* list)
  {
    if (!list) {
      std::cerr << "output_names: list pointer is nullptr" << '\n';
      return 1;
    }
    LIter< Data > it = list->begin();
    bool is_first = true;
    while (it != list->end()) {
      output< std::string >(std::cout, (*it).name, is_first);
      is_first = false;
      ++it;
    }
    if (!list->empty()) {
      std::cout << '\n';
    }

    return 0;
  }
  size_t max_sequences(List< Data >* list)
  {
    if (!list) {
      std::cerr << "max_sequences: list pointer is nullptr";
      return 0;
    }
    size_t maxNum = 0;
    LIter< Data > itList = list->begin();
    while (itList != list->end()) {
      const List< size_t >& nums = (*itList).numbers;
      LCIter< size_t > itNums = nums.begin();
      size_t count = 0;
      while (itNums != nums.end()) {
        count++;
        ++itNums;
      }
      maxNum = std::max(maxNum, count);
      ++itList;
    }
    return maxNum;
  }
  size_t output_sequences(List< Data >* list)
  {
    if (!list) {
      std::cerr << "output_sequences: list pointer is nullptr";
      return 1;
    }
    for (size_t i = 0; i < max_sequences(list); ++i) {
      bool is_first = true;
      LIter< Data > itList = list->begin();
      while (itList != list->end()) {
        LIter< size_t > itNums = (*itList).numbers.begin();
        bool flag = true;
        for (size_t j = 0; j < i; ++j) {
          if (itNums != (*itList).numbers.end()) {
            ++itNums;
          } else {
            flag = false;
            break;
          }
        }
        if (flag && itNums != (*itList).numbers.end()) {
          output< size_t >(std::cout, (*itNums), is_first);
          is_first = false;
        }
        ++itList;
      }
      std::cout << '\n';
    }
    return 0;
  }

  size_t output_sums(List< Data >* list)
  {
    if (!list) {
      std::cerr << "output_sums: list pointer is nullptr" << '\n';
      return 1;
    }

    const size_t maxSeq = max_sequences(list);

    if (maxSeq == 0) {
      std::cout << "0\n";
      return 0;
    }

    List< size_t > sums;
    LIter< size_t > itSum = sums.before_begin();
    bool overflow = false;

    for (size_t i = 0; i < maxSeq; ++i) {
      size_t sum = 0;
      LIter< Data > itList = list->begin();

      while (itList != list->end()) {
        LIter< size_t > itNums = (*itList).numbers.begin();
        for (size_t j = 0; j < i && itNums != (*itList).numbers.end(); ++j) {
          ++itNums;
        }
        if (itNums != (*itList).numbers.end()) {
          size_t value = (*itNums);
          if (sum > std::numeric_limits< size_t >::max() - value) {
            overflow = true;
            break;
          }
          sum += value;
        }
        ++itList;
      }

      if (overflow) {
        break;
      }

      sums.insert_after(itSum, sum);
      ++itSum;
    }

    if (overflow) {
      std::cerr << "output_sums: sum overflow" << '\n';
      return 1;
    }

    LIter< size_t > it = sums.begin();
    bool is_first = true;
    while (it != sums.end()) {
      output< size_t >(std::cout, (*it), is_first);
      is_first = false;
      ++it;
    }
    std::cout << '\n';

    return 0;
  }
  size_t to_size_t(const std::string& num)
  {
    size_t res = 0;
    for (size_t i = 0; i < num.size(); ++i) {

      res = res * 10 + static_cast< size_t >(num[i] - '0');
    }
    return res;
  }
  bool isdigit(const std::string& str)
  {
    if (str.empty()) {
      return false;
    }

    for (size_t i = 0; i < str.size(); ++i) {
      if (!std::isdigit(str[i])) {
        return false;
      }
    }

    return true;
  }
}
