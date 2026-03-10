#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <stdexcept>
#include "node.hpp"


template<class T>
class List;

template<class T>
class LIter
{
  friend class List<T>;

private:
  Node<T>* current;

public:

  LIter() noexcept:
    current(nullptr)
  {}

  LIter(Node<T>* ptr) noexcept:
    current(ptr)
  {}

  bool hasNext() const noexcept
  {
    return current != nullptr;
  }

  LIter next() const
  {
    if(!current)
    {
      throw std::runtime_error("null iterator");
    }
    return LIter(current->next);
  }

  T& operator*() const
  {
    if(!current)
    {
      throw std::runtime_error("null iterator");
    }
    return current->data;
  }

  bool operator==(const LIter& other) const noexcept
  {
    return current == other.current;
  }

  bool operator!=(const LIter& other) const noexcept
  {
    return current != other.current;
  }
};



#endif