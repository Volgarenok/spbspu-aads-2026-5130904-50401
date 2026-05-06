#include "commands.hpp"
#include <utility>

namespace vasyakin
{
  template< class T, class Cmp >
  void sort_list(vasyakin::List< T >& list, Cmp cmp)
  {
    for (auto i = list.begin(); i != list.end(); ++i)
    {
      for (auto j = list.begin(); j != list.end(); ++j)
      {
        if (cmp(*i, *j))
        {
          std::swap(*i, *j);
        }
      }
    }
  }

  struct OutputLine
  {
    std::string name;
    vasyakin::List< size_t > weights;
  };

  static inline auto p = [](const std::string& a, const std::string& b)
  {
    return a < b;
  };

  static inline auto f = [](const OutputLine& a, const OutputLine& b)
  {
    return a.name < b.name;
  };

  static inline auto g = [](size_t a, size_t b)
  {
    return a < b;
  };

  void output_int(std::ostream& out, const vasyakin::List< size_t >& list)
  {
    for (auto it = list.begin(); it != list.end(); ++it)
    {
      out << " " << *it;
    }
    out << '\n';
  }
}
