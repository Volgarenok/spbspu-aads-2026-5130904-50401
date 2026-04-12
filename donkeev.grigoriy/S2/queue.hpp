#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "../common/list.hpp"
namespace donkeev
{
  template< class T >
  class Queue
  {
    List< T > list_;

  public:
    Queue();
    explicit Queue(std::initializer_list< T >);

    void push(const T&);
    void pop();
    
    T& front();
    const T& front() const;

    size_t size();
    bool isEmpty();
    void clear();

    ~Queue();
  };
}
#endif
