#ifndef STACK_HPP
#define STACK_HPP
#include "../common/list.hpp"
#include <initializer_list>

namespace donkeev
{
  template< class T >
  class Stack
  {
    List< T > list_;
  
  public:
    Stack() = default;
    explicit Stack(std::initializer_list< T >);

    void push(const T&);
    void pop();

    T& top();
    const T& top() const;

    bool isEmpty() const;
    size_t size() const;
    void clear();

    ~Queue() = default;
  };
}
#endif
