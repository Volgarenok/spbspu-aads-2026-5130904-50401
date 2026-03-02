#include <iostream>
#include "mylist.hpp"

int main()
{
  goltsov::List< int > a;
  a.push_back(1);
  goltsov::LIter< int > ai = a.begin();
  std::cout << *ai;
}
