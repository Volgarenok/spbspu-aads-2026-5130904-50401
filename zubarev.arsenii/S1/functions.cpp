#include "all.hpp"
#include <string>
#include <iostream>

namespace zubarev
{
  List< Data > input(std::istream& in)
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
          throw std::runtime_error("input: invalid number format");
        }
      }

      value.numbers = nums;
      list.insert_after(itList, value);
      ++itList;
    }

    return list;
  }
  template <class T>
  void output(std::ostream &out, T el, bool is_first)
  {
    if (is_first)
    {
      out << el;
    }
    else
    {
      out << " " << el;
    }
  }

  void output_names(List< Data >* list)
  {
    if (!list) {
      throw std::invalid_argument("outout_names: list pointer is nullptr");
      ;
    }
    LIter< Data > it = list->begin();
    bool is_first=true;
    while (it != list->end()) {
      output<std::string>(std::cout, (*it).name,is_first);
      //std::cout<< (*it).name << ' ';
      ++it;
    }
    std::cout << '\n';
  }
  size_t max_sequences(List< Data >* list)
  {
    if (!list) {
      throw std::invalid_argument("max_sequences: list pointer is nullptr");
    }
    size_t maxNum = 0;
    LIter< Data > itList = list->begin();
    while (itList != list->end()) {
      List< int > nums = (*itList).numbers;
      LIter< int > itNums = nums.begin();
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
  void output_sequences(List< Data >* list)
  {
    if (!list) {
      throw std::invalid_argument("output_sequences: list pointer is nullptr");
    }
    for (size_t i = 0; i < max_sequences(list); ++i) {
      bool is_first=true;
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
          output<int>(std::cout,(*itNums),is_first);
          is_first=false;
          // std::cout << (*itNums) << " ";
        }
        ++itList;
      }
      std::cout << '\n';
    }
  }

  void output_sums(List< Data >* list)
  {
    bool has_value = false;
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
          has_value=true;
        }
        ++itList;
      }
    if (has_value) {
      output<size_t>(std::cout,sum,i>0);
    }
    }
    if (!has_value) {
      std::cout << 0;

    }
  }

}
