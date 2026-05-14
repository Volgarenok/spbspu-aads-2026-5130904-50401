#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <stdexcept>
#include "node.hpp"
namespace zhuravleva
{
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

    LIter& operator++()
    {
      if (!current)
      {
        throw std::runtime_error("null iterator");
      }
      current = current->next;
      return *this;
    }

    LIter operator++(int)
    {
      LIter<T> temp(*this);
      ++(*this);
      return temp;
    }

    T* operator->() const
    {
      if (!current)
      {
        throw std::runtime_error("null iterator");
      }
      return &(current->data);
    }
  };


  template<class T>
  class LCIter
  {
    friend class List<T>;

  private:
    const Node<T>* current;

  public:

    LCIter() noexcept:
      current(nullptr)
    {}

    LCIter(const Node<T>* ptr) noexcept:
      current(ptr)
    {}

    bool hasNext() const noexcept
    {
      return current != nullptr;
    }

    LCIter next() const
    {
      if(!current)
      {
        throw std::runtime_error("null iterator");
      }
      return LCIter(current->next);
    }

    const T& operator*() const
    {
      if(!current)
      {
        throw std::runtime_error("null iterator");
      }
      return current->data;
    }

    bool operator==(const LCIter& other) const noexcept
    {
      return current == other.current;
    }

    bool operator!=(const LCIter& other) const noexcept
    {
      return current != other.current;
    }
  
    LCIter& operator++()
    {
      if (!current)
      {
        throw std::runtime_error("null iterator");
      }
      current = current->next;
      return *this;
    }
  
    LCIter operator++(int)
    {
      LCIter<T> temp(*this);
      ++(*this);
      return temp;
    }
  
    const T* operator->() const
    {
      if (!current)
      {
        throw std::runtime_error("null iterator");
      }
      return &(current->data);
    }
  };

}
#endif
