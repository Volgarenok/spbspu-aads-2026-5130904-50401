#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include "list.hpp"

namespace petrov {

  bool isDigit(char sym)
  {
    return sym >= '0' && sym <= '9';
  }

  bool isNumber(const std::string & s)
  {
    if (s.empty()) {
      return 0;
    }
    size_t start_index = 0;
    if (s[0] == '-') {
      start_index = 1;
    }
    if (start_index == s.length()) {
      return 0;
    }
    for (size_t i = start_index; i < s.length(); ++i) {
      if (!isDigit(s[i])) {
        return 0;
      }
    }
    return 1;
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
  petrov::List< std::pair< std::string, petrov::List< size_t > > > data;
  std::string s;

  while (std::cin >> s) {
    if (!petrov::isNumber(s)) {
      std::pair< std::string, petrov::List< size_t > > new_column;
      new_column.first = s;
      data.pushBack(new_column);
    } else {
      if (!data.isEmpty()) {
        data.getBack().second.pushBack(petrov::parseSizeT(s));
      }
    }
  }
  if (data.isEmpty()) {
    std::cout << "0\n";
    return 0;
  }

  bool is_first_name = 1;
  for (auto it = data.begin(); it != data.end(); ++it) {
    if (!is_first_name) {
      std::cout << " ";
    }
    std::cout << it->first;
    is_first_name = 0;
  }
  std::cout << "\n";

  petrov::List< petrov::List< size_t >::Iterator > iterators;
  for (auto it = data.begin(); it != data.end(); ++it) {
    iterators.pushBack(it->second.begin());
  }

  petrov::List< size_t > row_sums;
  bool has_active_elements = 1;

  while (has_active_elements) {
    has_active_elements = 0;
    bool is_first_in_row = 1;
    size_t current_row_sum = 0;
    bool has_overflow = 0;

    auto iter_it = iterators.begin();
    auto data_it = data.begin();

    for (; iter_it != iterators.end(); ++iter_it, ++data_it) {
      if (* iter_it != data_it->second.end()) {
        size_t value = ** iter_it;

        if (!is_first_in_row) {
          std::cout << " ";
        }
        std::cout << value;

        if (std::numeric_limits< size_t >::max() - value < current_row_sum) {
          has_overflow = 1;
        }
        current_row_sum += value;

        ++(* iter_it);
        has_active_elements = 1;
        is_first_in_row = 0;
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

  if (row_sums.isEmpty()) {
    std::cout << "0\n";
  } else {
    bool is_first_sum = 1;
    for (auto it = row_sums.begin(); it != row_sums.end(); ++it) {
      if (!is_first_sum) {
        std::cout << " ";
      }
      std::cout << * it;
      is_first_sum = 0;
    }
    if (!row_sums.isEmpty()) {
      std::cout << "\n";
    }
  }

  return 0;
}
