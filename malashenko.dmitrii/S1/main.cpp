#include <utility>
#include <string>
#include <iostream>
#include "list.hpp"
#include "functions.hpp"
#include "iterator.hpp"
int main()
{
  using namespace malashenko;
  List< int > l;
  std::cout << l.size() << '\n';

  l.push_back(1);
  
  l.push_back(2);
  l.push_front(3);
  // std::cout << l.size() << '\n';

  for (LIter< int > s = l.begin(); s != l.end(); ++s)
  {
    std::cout << *s << '\n';
  }

  std::cout << '\n';
  // l.erase(l.begin());
  // l.erase(l.begin());
  // l.erase(l.begin());

  // l.pop_front();
  // l.clear();
  List< int > k = l;
  k.push_back(4);
  for (LIter< int > s = k.begin(); s != k.end(); ++s)
  {
    std::cout << *s << '\n';
  }
}
