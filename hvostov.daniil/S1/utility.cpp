#include <iostream>
#include <limits>
#include "utility.hpp"
#include "list.hpp"

bool hvostov::isCorrectNumber(const std::string& str)
{
  std::string max_string = std::to_string(std::numeric_limits< size_t >::max());
  if (str.size() != max_string.size()) {
    return str.size() < max_string.size();
  }
  for (size_t i = 0; i < str.size(); i++) {
    if (str[i] > max_string[i]) return false;
    if (str[i] < max_string[i]) return true;
  }
  return true;
}

size_t hvostov::fromStringToNumber(const std::string& str)
{
  size_t result = 0;
  for (size_t i = 0; i < str.size(); i++) {
    result = result * 10 + (str[i] - '0');
  }
  return result;
}

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
      if (!isCorrectNumber(number)) {
        list_it = list.insertAfter(list_it, { name, numbers });
        throw std::overflow_error("Too BIG number!");
      }
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

void hvostov::printInfo(std::ostream& out, const List< std::pair< std::string, List< size_t > > >& list)
{
  Liter< std::pair< std::string, List< size_t > > > it = list.begin();
  out << (*it).first;
  it++;
  for (; it != list.end(); it++) {
    out << " " << (*it).first;
  }
  out << "\n";
  List< Liter< size_t > > list_it;
  Liter< Liter< size_t > > lit = list_it.begin();
  for (Liter< std::pair< std::string, List< size_t > > > it = list.begin(); it != list.end(); it++) {
    lit = list_it.insertAfter(lit, (*it).second.begin());
  }
  bool F = true;
  bool first = true;
  while (F) {
    F = false;
    first = true;
    for (Liter< Liter< size_t > > it = list_it.begin(); it != list_it.end(); it++) {
      if (*(*(it))) {
        size_t value = *(*(it));
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
      out << "\n";
    }
  }
}

void hvostov::printResult(std::ostream& out, const List< std::pair< std::string, List< size_t > > >& list)
{
  List< Liter< size_t > > list_it;
  Liter< Liter< size_t > > lit = list_it.begin();
  for (Liter< std::pair< std::string, List< size_t > > > it = list.begin(); it != list.end(); it++) {
    lit = list_it.insertAfter(lit, (*it).second.begin());
  }
  bool F = true;
  List< size_t > result;
  Liter< size_t > result_it = result.begin();
  while (F) {
    F = false;
    size_t sum = 0;
    for (Liter< Liter< size_t > > it = list_it.begin(); it != list_it.end(); it++) {
      if (*(*(it))) {
        size_t value = *(*(it));
        try {
          sum = sumNumbersWithOverflow(sum, value);
        } catch (const std::overflow_error &e) {
          throw;
        }
        F = true;
        (*it)++;
      }
    }
    if (F) {
      result_it = result.insertAfter(result_it, sum);
    }
  }
  result_it = result.begin();
  out << (*result_it);
  result_it++;
  for (; result_it != result.end(); result_it++) {
    out << " " << *result_it;
  }
  out << "\n";
}
