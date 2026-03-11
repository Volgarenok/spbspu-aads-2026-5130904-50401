#include "data.hpp"
#include "functions.hpp"
#include "iter.hpp"
#include "list.hpp"
#include <iostream>
int main() {

  zubarev::List<zubarev::Data> list;
  list = zubarev::input(std::cin);
  try {
    zubarev::output_names(&list);
    zubarev::output_sequences(&list);

    zubarev::output_sums(&list);
  } catch (...) {
    return 1;
  }
  return 0;
}
