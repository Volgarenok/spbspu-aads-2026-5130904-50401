#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "bidir_list.hpp"
namespace tarasenko
{
  template< class T >
  class Queue
  {
  public:
    void push(const T& rhs);
    T pop();
    const T& front() const;
    const T& back() const;
    size_t size() const noexcept;
    bool empty() const noexcept;
    void clear() noexcept;

  private:
    BidirList< T > data_;
  };
}
#endif
