#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "../Common/list.hpp"

template< class T >
class Queue
{
  public:
    Queue();
    ~Queue() = default;
    Queue(const Queue< T > & queue);
    Queue< T > & operator=(const Queue< T > & queue);
    Queue(Queue< T > && queue);
    Queue< T > & operator=(Queue< T > && list);

    bool empty() const;
    size_t size() const;

    T & first() const;
    T & last() const;

    void push(const T & rhs);
    T drop();
    void clear();

  private:
    List< T > list_;
};

#endif