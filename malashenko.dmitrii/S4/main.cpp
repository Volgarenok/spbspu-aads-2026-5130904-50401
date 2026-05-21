#include <iostream>
#include "BStree.hpp"
int main()
{
  using namespace malashenko;

  BSTree< int, int, std::less< int > > tree;
  tree.push(20, 100);
  tree.push(10, 200);
  tree.push(40, 300);
  tree.push(30, 400);
  tree.push(50, 400);

  for (auto b = tree.begin(); b != tree.end(); ++b)
  {
    std::cout << b->first << ' ' << b->second << '\n';
  }


}
