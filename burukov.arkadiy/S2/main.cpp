#include <iostream>
#include "stack.hpp"

int main()
{
  burukov::Stack< int > s;
  s.push(42);
  std::cout << s.top() << "\n";
  return 0;
}
