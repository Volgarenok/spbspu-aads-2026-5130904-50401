#ifndef MY_EQUAL_HPP
#define MY_EQUAL_HPP
namespace zubarev
{
  template < class T >
  struct Equaler
  {
    bool operator()(const T& a, const T& b) const noexcept
    {
      return a == b;
    }
  };
}
#endif
