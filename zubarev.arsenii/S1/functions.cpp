#include "all.hpp"
#include <string>
#include <iostream>

namespace zubarev
{
  List< Data > input(std::istream& in,bool& error)
  {
    List< Data > list;
    LIter< Data > itList = list.before_begin();
    Data value;

    while (in >> value.name) {
      List< int > nums;
      LIter< int > itNum = nums.before_begin();
      int num;

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
          nums.insert_after(itNum, num);
          ++itNum;

        } else {
          error=true;
          std::cerr<<"input: bad input"<<'\n';
          return List<Data>{};
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

  int output_names(List< Data >* list)
  {
    if (!list) {
      std::cerr<<"output_names: list pointer is nullptr"<<'\n';
      return 1;
    }
    LIter< Data > it = list->begin();
    bool is_first = true;
    while (it != list->end()) {
      output< std::string >(std::cout, (*it).name, is_first);
      is_first = false;
      // std::cout<< (*it).name << ' ';
      ++it;
    }
    std::cout << '\n';

    return 0;
  }
  size_t max_sequences(List< Data >* list)
  {
    if (!list) {
      std::cerr<<"max_sequences: list pointer is nullptr";
      return 0;
    }
    size_t maxNum = 0;
    LIter< Data > itList = list->begin();
    while (itList != list->end()) {
      const List< int >& nums = (*itList).numbers;
      LCIter< int > itNums = nums.begin();
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
  int output_sequences(List< Data >* list)
  {
    if (!list) {
      std::cerr<<"output_sequences: list pointer is nullptr";
      return 1;
    }
    for (size_t i = 0; i < max_sequences(list); ++i) {
      bool is_first = true;
      LIter< Data > itList = list->begin();
      while (itList != list->end()) {
        LIter< int > itNums = (*itList).numbers.begin();
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
          output< int >(std::cout, (*itNums), is_first);
          is_first = false;

        }
        ++itList;
      }
      std::cout << '\n';

    }
    return 0;
  }

  int output_sums(List< Data >* list)
  {
    if (!list) {
      std::cerr<<"output_sums: list pointer is nullptr";
      return 1;
    }

    for (size_t i = 0; i < max_sequences(list); ++i) {
      size_t sum = 0;
      LIter< Data > itList = list->begin();

      while (itList != list->end()) {
        LIter< int > itNums = (*itList).numbers.begin();
        for (size_t j = 0; j < i && itNums != (*itList).numbers.end(); ++j) {
          ++itNums;
        }
        if (itNums != (*itList).numbers.end()) {
          sum += (*itNums);

        }
        ++itList;
      }

      output< size_t >(std::cout, sum, i == 0);

    }
    std::cout<<'\n';
    return 0;
  }

}
