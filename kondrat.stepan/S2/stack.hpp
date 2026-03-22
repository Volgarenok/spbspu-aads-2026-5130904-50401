#ifndef STACK_HPP
#define STACK_HPP
#include "../Common/list.hpp"

template< class T >
class Stack
{
  public:
    Stack();
    ~Stack() = default;
    Stack(const Stack< T > & queue);
    Stack< T > & operator=(const Stack< T > & queue);
    Stack(Stack< T > && stack);
    Stack< T > & operator=(Stack< T > && queue);

    bool empty();
    size_t size();

    T & first() const;

    void push(const T & rhs);
    T drop();
    void clear();

  private:
    List< T > list_
};

#endif