#include <iostream>
#include "list.hpp"
#include "data.hpp"
#include "functions.hpp"
#include "iter.hpp"
int main()
{

  zubarev::List< zubarev::Data > list;
  list = zubarev::input(std::cin);

  zubarev::output_names(&list);
  zubarev::output_sequences(&list);

  zubarev::output_sums(&list);
}
