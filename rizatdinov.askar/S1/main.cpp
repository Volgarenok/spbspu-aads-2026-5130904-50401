#include <iostream>
#include <string>
#include <limits>

#include "list.hpp"

int main() {
  namespace riz = rizatdinov;

  size_t max_size = 1;

  riz::List<std::pair<std::string, riz::List<long unsigned>>> result;
  riz::List<riz::List<size_t>::const_iterator> range;
  riz::List<long unsigned> summ{0};

  bool overflow = false;

  try {
    auto result_iterator = result.before_begin();
    auto range_iterator = range.before_begin();
    while (!std::cin.eof()) {
      std::string name;
      std::cin >> name;
      if (std::cin.eof()) {
        break;
      }

      riz::List<long unsigned> lnum;

      size_t size = 0;
      long unsigned num = 0;
      auto lnum_iterator = lnum.before_begin();
      auto summ_iterator = summ.before_begin();
      while (std::cin >> num) {
        auto temp_iterator = summ_iterator++;
        if (summ_iterator == summ.end()) {
          summ_iterator = summ.insert_after(temp_iterator, 0);
        }

        lnum_iterator = lnum.insert_after(lnum_iterator, num);
        if (!overflow && (*summ_iterator) > std::numeric_limits<long unsigned>::max() - num) {
          overflow = true;
        } else {
          (*summ_iterator) += num;
        }
        ++size;
     }

      if (!std::cin.eof() && std::cin.fail()) {
        std::cin.clear();
      }

      max_size = max_size < size ? size : max_size;

      std::pair<std::string, riz::List<long unsigned>> pair{ name, lnum };
      result_iterator = result.insert_after(result_iterator, pair);
      range_iterator = range.insert_after(range_iterator, (*result_iterator).second.cbegin());
    }
  } catch (const std::bad_alloc& e) {
    std::cerr << "Error: failed to allocate memory" << std::endl;
    return 1;
  }

  bool flag = true;
  for (auto i = result.cbegin(); i != result.cend(); ++i) {
    if (!flag) {
      std::cout << ' ';
    }
    flag = false;
    std::cout << (*i).first;
  }

  for (size_t i = 0; i < max_size; ++i) {
    if (!flag) {
      std::cout << '\n';
    }

    flag = true;
    for (auto j = range.begin(); j != range.end(); ++j) {
      if ((*j) == riz::List<long unsigned>::const_iterator(nullptr)) {
        continue;
      }
      if (!flag) {
        std::cout << ' ';
      }
      flag = false;
      std::cout << *((*j)++);
    }
  }
  if (!flag) {
    std::cout << '\n';
  }

  if (overflow) {
    std::cerr << "Error: overflow\n";
    return 1;
  }
  flag = true;
  for (auto i = summ.begin(); i != summ.end(); ++i) {
    if (!flag) {
      std::cout << ' ';
    }
    flag = false;
    std::cout << *i;
  }
  std::cout << '\n';

  return 0;
}
