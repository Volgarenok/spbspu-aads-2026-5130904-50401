#ifndef NODE_HPP
#define NODE_HPP

#include <boost/optional.hpp>

#include <utility>

namespace yalovsky
{
  namespace detail
  {
    template< class T >
    class Node
    {
    public:
      Node* next_;
      Node* prev_;

      Node() noexcept:
        next_(this),
        prev_(this),
        value_()
      {}

      explicit Node(const T& value):
        next_(this),
        prev_(this),
        value_(value)
      {}

      explicit Node(T&& value):
        next_(this),
        prev_(this),
        value_(std::move(value))
      {}

      T& value()
      {
        return *value_;
      }

      const T& value() const
      {
        return *value_;
      }

    private:
      boost::optional< T > value_;
    };

    template< class T >
    void linkBefore(Node< T >* position, Node< T >* node) noexcept
    {
      node->next_ = position;
      node->prev_ = position->prev_;
      position->prev_->next_ = node;
      position->prev_ = node;
    }

    template< class T >
    void unlink(Node< T >* node) noexcept
    {
      node->prev_->next_ = node->next_;
      node->next_->prev_ = node->prev_;
      node->next_ = node;
      node->prev_ = node;
    }
  }
}

#endif
