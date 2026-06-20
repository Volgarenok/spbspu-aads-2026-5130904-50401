#include "utility.hpp"
#include <iostream>
#include <limits>
#include <stdexcept>
#include <list.hpp>

size_t hvostov::sumNumbersWithOverflow(size_t a, size_t b)
{
  size_t max = std::numeric_limits< size_t >::max();
  if (a <= max - b) {
    return a + b;
  }
  throw std::overflow_error("Sum is overflow!");
}

hvostov::List< std::pair< std::string, hvostov::List< size_t > > > hvostov::getData(std::istream& in)
{
  List< std::pair< std::string, List< size_t > > > list;
  Liter< std::pair< std::string, List< size_t > > > list_it = list.begin();
  std::string name;
  while (in >> name) {
    List< size_t > numbers;
    size_t number;
    Liter< size_t > it = numbers.begin();
    while (in >> number) {
      it = numbers.insertAfter(it, number);
    }
    if (in.bad()) {
      throw std::logic_error("Bad input!");
    }
    list_it = list.insertAfter(list_it, {name, numbers});
    in.clear();
  }
  if (!in.eof()) {
    throw std::logic_error("Bad input!");
  }
  return list;
}

hvostov::List< hvostov::LCiter< size_t > >
hvostov::createIteratorList(const List< std::pair< std::string, List< size_t > > >& list)
{
  List< LCiter< size_t > > iter_list;
  Liter< LCiter< size_t > > lit = iter_list.begin();
  for (LCiter< std::pair< std::string, List< size_t > > > it = list.begin(); it != list.end(); it++) {
    lit = iter_list.insertAfter(lit, it->second.begin());
  }
  return iter_list;
}

hvostov::List< size_t > hvostov::calculateSums(const List< std::pair< std::string, List< size_t > > >& list)
{
  List< LCiter< size_t > > iter_list = createIteratorList(list);
  List< size_t > result;
  Liter< size_t > result_it = result.begin();
  bool has_more = true;
  while (has_more) {
    has_more = false;
    size_t sum = 0;
    for (Liter< LCiter< size_t > > it = iter_list.begin(); it != iter_list.end(); it++) {
      if (*(*(it))) {
        size_t value = *(*(it));
        sum = sumNumbersWithOverflow(sum, value);
        has_more = true;
        (*(it))++;
      }
    }
    if (has_more) {
      result_it = result.insertAfter(result_it, sum);
    }
  }
  return result;
}

void hvostov::printNames(std::ostream& out, const List< std::pair< std::string, List< size_t > > >& list)
{
  LCiter< std::pair< std::string, List< size_t > > > it = list.begin();
  if (it == list.end()) {
    return;
  }
  out << it->first;
  it++;
  for (; it != list.end(); it++) {
    out << " " << it->first;
  }
  out << "\n";
}

void hvostov::printNumbers(std::ostream& out, List< LCiter< size_t > >& list_it)
{
  bool has_more = true;
  while (has_more) {
    has_more = false;
    Liter< LCiter< size_t > > cur = list_it.begin();
    while (cur != list_it.end() && !*(*(cur))) {
      cur++;
    }
    if (cur != list_it.end()) {
      size_t value = *(*(cur));
      out << value;
      has_more = true;
      (*(cur))++;
      cur++;
    }
    for (; cur != list_it.end(); cur++) {
      if (*(*(cur))) {
        size_t value = *(*(cur));
        out << " " << value;
        has_more = true;
        (*(cur))++;
      }
    }
    if (has_more) {
      out << "\n";
    }
  }
}

void hvostov::printList(std::ostream& out, const List< size_t >& list)
{
  if (list.empty()) {
    return;
  }
  LCiter< size_t > it = list.begin();
  out << *it;
  it++;
  for (; it != list.end(); it++) {
    out << " " << *it;
  }
}

void hvostov::printInfo(std::ostream& out, const List< std::pair< std::string, List< size_t > > >& list)
{
  printNames(out, list);
  List< LCiter< size_t > > list_it = createIteratorList(list);
  printNumbers(out, list_it);
}

void hvostov::printResult(std::ostream& out, const List< std::pair< std::string, List< size_t > > >& list)
{
  List< size_t > result = calculateSums(list);
  if (result.empty()) {
    out << "0";
  } else {
    printList(out, result);
  }
}
