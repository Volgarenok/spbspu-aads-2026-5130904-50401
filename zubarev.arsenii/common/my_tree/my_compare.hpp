#ifndef MY_COMPARE_HPP
#define MY_COMPARE_HPP
namespace zubarev
{
  template< class T >
  struct Comparator
  {
    bool operator()(const T& a, const T& b) const
    {
      return a < b;
    }
  };
}
#endif
