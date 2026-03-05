#include "list.hpp"

int main()
{
  using namespace malashenko;
  List< int > l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);

  List< int > k(l);
  

}
