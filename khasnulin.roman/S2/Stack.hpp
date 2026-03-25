#ifndef STACK_HPP
#define STACK_HPP

#include "List.hpp"
#include <cstddef>

namespace khasnulin
{
  template < class T > class Stack
  {
  public:
    Stack() = default;

    Stack(const Stack< T > &rhs) = default;
    Stack(Stack< T > &&rhs) noexcept = default;

    Stack< T > &operator=(const Stack< T > &rhs) = default;
    Stack< T > &operator=(Stack< T > &&rhs) noexcept = default;

    bool operator==(const Stack< T > &rhs) const noexcept;
    bool operator!=(const Stack< T > &rhs) const noexcept;

    ~Stack() = default;

    T &top();
    const T &top() const;

    bool empty() const noexcept;
    size_t size() const noexcept;

    void push(const T &elem);
    void push(T &&elem);

    template < class... Args > void emplace(Args &&...args);

    void pop();

    void swap(Stack< T > &rhs) noexcept;

  private:
    BiList< T > list_;
  };
}

#endif
