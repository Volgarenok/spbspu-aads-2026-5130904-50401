#ifndef EQUAL_FUNCTOR_HPP
#define EQUAL_FUNCTOR_HPP

namespace malashenko
{
  template< class T >
  struct Equal {
    bool operator()(const T& lhs, const T& rhs);
  };

  template< class T >
  bool Equal< T >::operator()(const T& lhs, const T& rhs)
  {
    return lhs == rhs;
  }
}

#endif
