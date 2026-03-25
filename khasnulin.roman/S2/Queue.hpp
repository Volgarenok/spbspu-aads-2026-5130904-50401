#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstddef>

#include <List.hpp>

namespace khasnulin
{
  template < class T > class Queue
  {
  public:
    Queue() = default;

    Queue(const Queue< T > &) = default;
    Queue(Queue< T > &&) noexcept = default;

    Queue< T > &operator=(const Queue< T > &) = default;
    Queue< T > &operator=(Queue< T > &&) = default;

    ~Queue< T >() = default;

    Queue< T > &front();
    Queue< T > &back();

    bool empty() const noexcept;
    size_t size() const noexcept;

    void push(const T &elem);
    void pop();
    template < class... Args > void emplace(Args... args);

    void swap(Queue< T > &rhs) noexcept;

  private:
    BiList< T > list_;
  };
}

#endif
