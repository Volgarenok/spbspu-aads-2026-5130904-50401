#include "list.hpp"

int main()
{
  using namespace malashenko;
  List< int > l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);

  using pair_t = std::pair<std::string, List< int > >;

  List< pair_t > k;
  
  std::cout << l.front() << '\n';
  

}
