#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <cassert>
#include <utility>

#include "iter.hpp"
#include "const-iter.hpp"

namespace lukashevich
{
  namespace detail
  {
    template< class T >
    class Node
    {
      public:
        Node(T value, Node<T>* n, Node<T>* p):
          val(value),
          next(n),
          prev(p)
        {}

        T val;
        Node< T >* next;
        Node< T >* prev;
    };
  }

  template< class T >
  class LIter;

  template< class T >
  class LCIter;

  template< class T >
  class List
  {
    public:
      List() noexcept;
      ~List() noexcept;
      List(const List< T >& list);
      List< T >& operator=(const List< T >& list);
      List(List< T >&& list) noexcept;
      List< T >& operator=(List< T >&& list) noexcept;

      T& front() noexcept;
      T& back() noexcept;
      const T& front() const noexcept;
      const T& back() const noexcept;

      void pushFront(const T& value);
      void pushBack(const T& value);
      void pushFront(T&& value);
      void pushBack(T&& value);

      void popFront() noexcept;
      void popBack() noexcept;

      void clear() noexcept;

      LIter< T > begin() noexcept;
      LIter< T > end() noexcept;
      LCIter< T > begin() const noexcept;
      LCIter< T > end() const noexcept;

      size_t size() const noexcept;
      bool empty() const noexcept;

      void swap(List< T > & list) noexcept;

    private:
      detail::Node< T >* fake_;
      size_t size_;
  };

  template< class T >
  detail::Node< T >* makeFake()
  {
    detail::Node< T >* fake = static_cast< detail::Node< T >* >(::operator new(sizeof(detail::Node< T >)));
    fake->next = nullptr;
    fake->prev = nullptr;
    return fake;
  }

  template< class T >
  void removeFake(detail::Node< T >* fake) noexcept
  {
    ::operator delete(fake);
  }

  template< class T >
  List< T >::List() noexcept:
    fake_(makeFake< T >()),
    size_(0)
  {}

  template< class T >
  List< T >::~List() noexcept
  {
    clear();
    removeFake(fake_);
  }

  template< class T >
  void List< T >::clear() noexcept
  {
    detail::Node< T >* cur = fake_->next;
    while (cur != nullptr) {
      detail::Node< T >* next = cur->next;
      delete cur;
      cur = next;
    }
    fake_->next = nullptr;
    fake_->prev = nullptr;
    size_ = 0;
  }

  template< class T >
  void List< T >::pushFront(const T& value)
  {
    detail::Node< T >* node = new detail::Node< T >(value, fake_->next, fake_);

    if (fake_->next != nullptr) {
      fake_->next->prev = node;
    } else {
      fake_->prev = node;
    }
    fake_->next = node;
    size_++;
  }

  template< class T >
  void List< T >::pushBack(const T& value)
  {
    detail::Node< T >* prev_node = fake_->prev != nullptr ? fake_->prev : fake_;
    detail::Node< T >* node = new detail::Node< T >(value, nullptr, prev_node);

    if (fake_->prev != nullptr) {
      fake_->prev->next = node;
    } else {
      fake_->next = node;
    }

    fake_->prev = node;
    size_++;
  }

  template< class T >
  void List< T >::pushFront(T&& value)
  {
    detail::Node< T >* node = new detail::Node< T >(std::move(value), fake_->next, fake_);

    if (fake_->next != nullptr) {
      fake_->next->prev = node;
    } else {
      fake_->prev = node;
    }

    fake_->next = node;

    ++size_;
  }

  template< class T >
  void List< T >::pushBack(T&& value)
  {
    detail::Node< T >* prev_node = fake_->prev != nullptr ? fake_->prev : fake_;
    detail::Node< T >* node = new detail::Node< T >(std::move(value), nullptr, prev_node);

    if (fake_->prev != nullptr) {
      fake_->prev->next = node;
    } else {
      fake_->next = node;
    }

    fake_->prev = node;

    ++size_;
  }

  template< class T >
  const T& List< T >::front() const noexcept
  {
    assert(fake_->next != nullptr);
    return fake_->next->val;
  }

  template< class T >
  T& List< T >::front() noexcept
  {
    assert(fake_->next != nullptr);
    return fake_->next->val;
  }

  template< class T >
  const T& List< T >::back() const noexcept
  {
    assert(fake_->prev != nullptr);
    return fake_->prev->val;
  }

  template< class T >
  T& List< T >::back() noexcept
  {
    assert(fake_->prev != nullptr);
    return fake_->prev->val;
  }

  template< class T >
  void List< T >::popFront() noexcept
  {
    assert(fake_->next != nullptr);

    detail::Node< T >* node = fake_->next;
    fake_->next = node->next;

    if (fake_->next != nullptr) {
      fake_->next->prev = fake_;
    } else {
      fake_->prev = nullptr;
    }

    delete node;
    size_--;
  }

  template< class T >
  void List< T >::popBack() noexcept
  {
    assert(fake_->prev != nullptr);

    detail::Node< T >* node = fake_->prev;
    fake_->prev = node->prev;

    if (fake_->prev != fake_) {
      fake_->prev->next = nullptr;
    } else {
      fake_->prev = nullptr;
      fake_->next = nullptr;
    }

    delete node;
    size_--;
  }

  template< class T >
  LIter< T > List< T >::begin() noexcept
  {
    return LIter< T >(fake_->next);
  }

  template< class T >
  LCIter< T > List< T >::begin() const noexcept
  {
    return LCIter< T >(fake_->next);
  }

  template< class T >
  LIter< T > List< T >::end() noexcept
  {
    return LIter< T >(nullptr);
  }

  template< class T >
  LCIter< T > List< T >::end() const noexcept
  {
    return LCIter< T >(nullptr);
  }

  template< class T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }

  template< class T >
  bool List< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class T >
  void List< T >::swap(List< T >& list) noexcept
  {
    std::swap(fake_, list.fake_);
    std::swap(size_, list.size_);
  }

  template< class T >
  List< T >::List(const List< T >& list):
    fake_(makeFake< T >()),
    size_(0)
  {
    detail::Node< T >* cur = list.fake_->next;

    while (cur != nullptr) {
      pushBack(cur->val);
      cur = cur->next;
    }
  }

  template< class T >
  List< T >& List< T >::operator=(const List< T >& list)
  {
    assert(this != &list);
    List< T > temp(list);
    swap(*this, temp);
    return *this;
  }

  template< class T >
  List< T >::List(List< T >&& list) noexcept:
    fake_(std::exchange(list.fake_, makeFake< T >())),
    size_(std::exchange(list.size_, 0))
  {}

  template< class T >
  List< T >& List< T >::operator=(List< T >&& list) noexcept
  {
    assert(this != &list);
    List< T > temp(std::move(list));
    swap(*this, temp);
    return *this;
  }
}
#endif
