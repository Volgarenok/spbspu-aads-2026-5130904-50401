#include "list.hpp"
#include "functions.hpp"
int main()
{
  using namespace malashenko;

  using pair_t = std::pair<std::string, List< int > >;

  // List< pair_t > res;
  // std::string name1 = "aaa";
  // List< int > nums;
  // nums.push_back(1);
  // nums.push_back(2);
  // nums.push_back(3);

  // res.push_back({name1, nums});


  // std::string name2 = "B";
  // List< int > nums2;

  // res.push_front({name2, nums2});


  List< pair_t > res = getSequences(std::cin);
  std::cout << "func is done\n";
  for (LIter< pair_t > s = res.begin(), f = res.end(); s != f; ++s)
  {
    std::cout << (*s).first << '\n';
    if ((*s).second.size())
    {
      std::cout << (*s).second.front() << '\n';
    }
  // std::cout << (*s).second.front() << '\n';
  }
  std::cout << res.back().first <<'\n';
  std::cout << res.back().second.front() << '\n';

}
