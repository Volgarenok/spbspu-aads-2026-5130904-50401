#include <iostream>
#include "list.hpp"
#include "data.hpp"
#include "functions.hpp"
#include "iter.hpp"
int main()
{

  zubarev::List< zubarev::Data > list;
  list = zubarev::input(std::cin);
  zubarev::LIter< zubarev::Data > it = list.before_begin();
  while (it != list.end()) {
    std::cout << (*it).name << '\n';
    ++it;
  }
}
