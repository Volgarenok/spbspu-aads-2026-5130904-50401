#include "data.hpp"
#include "functions.hpp"
#include "iter.hpp"
#include "list.hpp"
#include <iostream>
int main()
{

  zubarev::List< zubarev::Data > list;
  bool error = false;
  list = zubarev::input(std::cin, error);
  if (error) {
    return 1;
  }
  error = zubarev::output_names(&list) || zubarev::output_sequences(&list) || zubarev::output_sums(&list);

  if (error) {
    return 1;
  }

  return 0;
}
