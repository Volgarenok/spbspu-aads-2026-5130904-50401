#ifndef UTILITY_HPP
#define UTILITY_HPP
#include <string>
#include "list.hpp"

namespace hvostov {
  List< std::pair< std::string, List< size_t > > > getData(std::istream& in);
  size_t fromStringToNumber(const std::string& str);
  void printResult(std::ostream& out, List< std::pair< std::string, List< size_t > > > list);
}

size_t hvostov::fromStringToNumber(const std::string& str)
{
  size_t result = 0;
  for (size_t i = 0; i < str.size(); i++) {
    result = result * 10 + (str[i] - '0');
  }
  return result;
}

hvostov::List< std::pair< std::string, hvostov::List< size_t > > > hvostov::getData(std::istream& in)
{
  List< std::pair< std::string, List< size_t > > > list;
  Liter< std::pair< std::string, List< size_t > > > list_it = list.begin();
  std::string name;
  char next = ' ';
  while (in >> name) {
    List< size_t > numbers;
    next = in.peek();
    if (next == '\n') {
      list_it = list.insertAfter(list_it, { name, numbers });
      continue;
    }
    if (next == EOF) {
      list_it = list.insertAfter(list_it, { name, numbers });
      break;
    }
    Liter< size_t > it = numbers.begin();
    std::string number;
    while (in >> number) {
      it = numbers.insertAfter(it, hvostov::fromStringToNumber(number));
      next = in.peek();
      if (next == '\n' || next == EOF) {
        break;
      }
    }
    list_it = list.insertAfter(list_it, { name, numbers });
  }
  return list;
}

void hvostov::printResult(std::ostream& out, List< std::pair< std::string, List< size_t > > > list)
{
  Liter< std::pair< std::string, List< size_t > > > it = list.begin();
  out << (*it).first;
  it++;
  for (; it != list.end(); it++) {
    out << " " << (*it).first;
  }
  out << '\n';
  List< Liter< size_t > > list_it;
  Liter< Liter< size_t > > lit = list_it.begin();
  for (Liter< std::pair< std::string, List< size_t > > > it = list.begin(); it != list.end(); it++) {
    lit = list_it.insertAfter(lit, (*it).second.begin());
  }
  bool F = true;
  bool first = true;
  List< size_t > result;
  Liter< size_t > result_it = result.begin();
  while (F) {
    F = false;
    first = true;
    size_t sum = 0;
    for (Liter< Liter< size_t > > it = list_it.begin(); it != list_it.end(); it++) {
      if (*(*(it))) {
        size_t value = *(*(it));
        sum += value;
        if (first) {
          out << value;
          first = false;
        } else {
          out << " " << value;
        }
        F = true;
        (*it)++;
      }
    }
    if (F) {
      result_it = result.insertAfter(result_it, sum);
      out << '\n';
    }
  }
  result_it = result.begin();
  out << (*result_it);
  result_it++;
  for (; result_it != result.end(); result_it++) {
    out << " " << *result_it;
  }
  out << '\n';
}

#endif
