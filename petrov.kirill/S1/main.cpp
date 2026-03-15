#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include "list.hpp"

namespace petrov {

  bool isDigit(char symbol)
  {
    return symbol >= '0' && symbol <= '9';
  }

  bool isNumber(const std::string & str)
  {
    if (str.empty()) {
      return false;
    }
    size_t start_index = 0;
    if (str[0] == '-') {
      start_index = 1;
    }
    if (start_index == str.length()) {
      return false;
    }
    for (size_t i = start_index; i < str.length(); ++i) {
      if (! isDigit(str[i])) {
        return false;
      }
    }
    return true;
  }

  size_t parseSizeT(const std::string & str)
  {
    size_t result = 0;
    const size_t base = 10;
    for (size_t i = 0; i < str.length(); ++i) {
      result = result * base + static_cast< size_t >(str[i] - '0');
    }
    return result;
  }
}

int main()
{
  using namespace petrov;
  List< std::pair< std::string, List< size_t > > > data;
  std::string token;

  while (std::cin >> token) {
    if (! isNumber(token)) {
      std::pair< std::string, List< size_t > > new_column;
      new_column.first = token;
      data.pushBack(new_column);
    } else {
      if (! data.isEmpty()) {
        data.getBack().second.pushBack(parseSizeT(token));
      }
    }
  }

  if (data.isEmpty()) {
    std::cout << "0\n";
    return 0;
  }

  bool is_first_name = true;
  for (auto it = data.begin(); it != data.end(); ++it) {
    if (! is_first_name) {
      std::cout << " ";
    }
    std::cout << it->first;
    is_first_name = false;
  }
  std::cout << "\n";

  List< List< size_t >::Iterator > iterators;
  for (auto it = data.begin(); it != data.end(); ++it) {
    iterators.pushBack(it->second.begin());
  }

  List< size_t > row_sums;
  bool has_active_elements = true;

  while (has_active_elements) {
    has_active_elements = false;
    bool is_first_in_row = true;
    size_t current_row_sum = 0;
    bool has_overflow = false;

    auto iter_it = iterators.begin();
    auto data_it = data.begin();

    for (; iter_it != iterators.end(); ++iter_it, ++data_it) {
      if (* iter_it != data_it->second.end()) {
        size_t value = ** iter_it;

        if (! is_first_in_row) {
          std::cout << " ";
        }
        std::cout << value;

        if (std::numeric_limits< size_t >::max() - value < current_row_sum) {
          has_overflow = true;
        }
        current_row_sum += value;

        ++(* iter_it);
        has_active_elements = true;
        is_first_in_row = false;
      }
    }

    if (has_active_elements) {
      std::cout << "\n";
      if (has_overflow) {
        std::cerr << "Overflow detected\n";
        return 1;
      }
      row_sums.pushBack(current_row_sum);
    }
  }

  bool is_first_sum = true;
  for (auto it = row_sums.begin(); it != row_sums.end(); ++it) {
    if (! is_first_sum) {
      std::cout << " ";
    }
    std::cout << * it;
    is_first_sum = false;
  }
  if (! row_sums.isEmpty()) {
    std::cout << "\n";
  }

  return 0;
}
