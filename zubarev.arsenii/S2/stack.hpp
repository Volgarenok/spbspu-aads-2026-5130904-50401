#include <iostream>
#include <string>
#include <vector>
namespace zubarev
{
  template < typename T >
  class Stack
  {
  private:


  public:
    void push(const T& rhs);
    T drop();
    const T& top();
    bool empty();
    size_t size();
  };
}
