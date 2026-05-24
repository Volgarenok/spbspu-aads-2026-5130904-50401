#ifndef YALOVSKY_NODE_HPP
#define YALOVSKY_NODE_HPP

#include <utility>

namespace yalovsky
{
  struct NodeBase
  {
    NodeBase* next_;
    NodeBase* prev_;

    NodeBase() noexcept:
      next_(this),
      prev_(this)
    {}
  };

  template< class T >
  struct Node: NodeBase
  {
    T value_;

    template< class U >
    explicit Node(U&& value):
      NodeBase(),
      value_(std::forward< U >(value))
    {}
  };
}

#endif

