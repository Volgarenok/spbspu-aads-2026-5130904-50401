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

  std::cout << tree.get(20) << '\n';
  std::cout << tree.get(10) << '\n';
  std::cout << tree.get(40) << '\n';
  std::cout << tree.get(30) << '\n';
  std::cout << tree.get(50) << '\n';
  std::cout << tree.height() << '\n';


}
