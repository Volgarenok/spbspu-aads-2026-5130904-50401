#include <iostream>
#include "stack.hpp"
#include "queue.hpp"

int main()
{
  burukov::Stack< int > s;
  s.push(42);
  std::cout << s.top() << "\n";

  burukov::Queue< int > q;
  q.push(10);
  q.push(20);
  std::cout << q.front() << "\n";
  return 0;
}
