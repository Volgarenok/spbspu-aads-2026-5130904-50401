#ifndef COMPARE_HPP
#define COMPARE_HPP

namespace lukashevich
{
  template< class T >
  struct Comparator
  {
    bool operator()(const T & lhs, const T & rhs) const
    {
      return lhs < rhs;
    }
  };
}

#endif
