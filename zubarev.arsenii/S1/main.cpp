#include <iostream>
#include <stdexcept>

#include "../common/my_list/data.hpp"
#include "functions.hpp"
#include "../common/my_list/iter.hpp"
#include "../common/my_list/list.hpp"

int main()
{
  bool error = false;

  zubarev::List< zubarev::Data > list = zubarev::input(std::cin, error);

  if (error) {
    return 1;
  }

  if (list.empty()) {
    std::cout << "0\n";
    return 0;
  }
  zubarev::output_names(list);
  std::cout << '\n';

  if (zubarev::max_sequences(list) > 0) {
    zubarev::output_sequences(list);
    std::cout << '\n';
  }

  try {
    const zubarev::List< size_t > sums = zubarev::calculate_sums(list);

    if (sums.empty()) {
      std::cout << "0\n";
    } else {
      zubarev::print_sums(sums);
      std::cout << '\n';
    }
  } catch (const std::overflow_error&) {
    std::cerr << "sum overflow\n";
    return 1;
  }

  return 0;
}
