#ifndef stack_HPP
#define stack_HPP
#include <cstddef>
#include "list.hpp"
#include <utility>

namespace hvostov {
  template< class T >
  class Stack {
    public:
      Stack();
      Stack(const Stack< T >& stack);
      Stack(Stack< T >&& stack) noexcept;
      ~Stack() = default;
      Stack< T >& operator=(const Stack< T >& stack);
      Stack< T >& operator=(Stack< T >&& stack) noexcept;

      T drop();
      void push(const T& rhs);
      void clear() noexcept;
      bool empty() const noexcept;
      T& top();
      const T& top() const;
      size_t getSize() const noexcept;
    private:
      List< T > list_;
  };
}

#endif

