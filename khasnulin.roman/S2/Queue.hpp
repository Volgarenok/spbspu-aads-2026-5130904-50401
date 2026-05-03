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
    Queue< T > &operator=(Queue< T > &&) noexcept = default;

    bool operator==(const Queue< T > &rhs) const noexcept;
    bool operator!=(const Queue< T > &rhs) const noexcept;

    ~Queue() noexcept = default;

    T &front();
    const T &front() const;

    T &back();
    const T &back() const;

    bool empty() const noexcept;
    size_t size() const noexcept;

    void push(const T &elem);
    void push(T &&elem);
    void pop();
    template < class... Args > void emplace(Args &&...args);

    void swap(Queue< T > &rhs) noexcept;

  private:
    BiList< T > list_;
  };

  template < class T > bool Queue< T >::operator==(const Queue< T > &rhs) const noexcept
  {
    return list_ == rhs.list_;
  }
  template < class T > bool Queue< T >::operator!=(const Queue< T > &rhs) const noexcept
  {
    return !(list_ == rhs.list_);
  }

  template < class T > T &Queue< T >::front()
  {
    return list_.front();
  }

  template < class T > const T &Queue< T >::front() const
  {
    return static_cast< const BiList< T > & >(list_).front();
  }

  template < class T > T &Queue< T >::back()
  {
    return list_.back();
  }

  template < class T > const T &Queue< T >::back() const
  {
    return static_cast< const BiList< T > & >(&list_).back();
  }

  template < class T > bool Queue< T >::empty() const noexcept
  {
    return list_.empty();
  }

  template < class T > size_t Queue< T >::size() const noexcept
  {
    return list_.size();
  }

  template < class T > void Queue< T >::push(const T &elem)
  {
    list_.push_back(elem);
  }

  template < class T > void Queue< T >::push(T &&elem)
  {
    list_.push_back(std::move(elem));
  }

  template < class T > void Queue< T >::pop()
  {
    list_.pop_front();
  }

  template < class T > template < class... Args > void Queue< T >::emplace(Args &&...args)
  {
    list_.emplace_back(std::forward< Args >(args)...);
  }

  template < class T > void Queue< T >::swap(Queue< T > &rhs) noexcept
  {
    list_.swap(rhs.list_);
  }
}

#endif
