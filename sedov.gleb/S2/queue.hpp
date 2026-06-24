#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <list.hpp>

namespace sedov
{
  template< class T >
  class Queue
  {
  public:
    Queue() = default;
    Queue(const Queue< T > & q) = default;
    Queue(Queue< T > && q) = default;
    ~Queue() = default;

    Queue & operator=(const Queue< T > & q) = default;
    Queue & operator=(Queue< T > && q) = default;

    T & front() noexcept;
    const T & front() const noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

    void push(const T& v);
    void push(T && v) noexcept;

    template< class... Args >
    LIter< T > emplace(Args&&... args);

    void pop() noexcept;
    void swap(Queue< T > & q) noexcept;
    void clear() noexcept;
  private:
    List< T > list_;
  };

  template< class T >
  T & Queue< T >::front() noexcept
  {
    return list_.front();
  }

  template< class T >
  const T & Queue< T >::front() const noexcept
  {
    return list_.front();
  }

  template< class T >
  bool Queue< T >::empty() const noexcept
  {
    return list_.size() == 0;
  }

  template< class T >
  size_t Queue< T >::size() const noexcept
  {
    return list_.size();
  }

  template< class T >
  void Queue< T >::push(const T& v)
  {
    list_.pushBack(v);
  }

  template< class T >
  void Queue< T >::push(T && v) noexcept
  {
    list_.pushBack(std::forward< T >(v));
  }

  template< class T >
  template< class... Args >
  LIter< T > Queue< T >::emplace(Args&&... args)
  {
    return list_.emplaceBack(std::forward< Args >(args)...);
  }

  template< class T >
  void Queue< T >::pop() noexcept
  {
    list_.popFront();
  }

  template< class T >
  void Queue< T >::swap(Queue< T > & q) noexcept
  {
    list_.swap(q.list_);
  }

  template< class T >
  void Queue< T >::clear() noexcept
  {
    list_.clear();
  }
}
#endif
