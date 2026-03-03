#include <iostream>
#include "mylist.hpp"

int main()
{
  goltsov::List< int > a;
  a.push_start(1);
  goltsov::LIter< int > ai = a.begin();
  a.insert(ai, 10);
  ai = ai.next();
  ai.insert(2);
  ai = ai.next();
  ai.insert(3);
  ai = a.begin();
  ai = a.begin();
  ai.insert(20);
  while(ai.hasNext())
  {
    std::cout << *ai << '\n';
    ai = ai.next();
  }
  std::cout << *ai << '\n';
  ai = a.getLast();
  std::cout << *ai << '\n';
}
