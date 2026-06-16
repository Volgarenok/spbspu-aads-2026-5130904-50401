#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <memory>
#include <utility>

#include "const-iterator.hpp"
#include "iterator.hpp"
#include "node.hpp"

namespace yalovsky
{
  template< class T >
  class List
  {
  public:
    using iterator = LIter< T >;
    using const_iterator = LCIter< T >;

    List() noexcept;
    List(const List& other);
    List(List&& other) noexcept;
    ~List();

    List& operator=(const List& other);
    List& operator=(List&& other) noexcept;

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    iterator insert(iterator position, const T& value);
    iterator insert(iterator position, T&& value);
    iterator erase(iterator position) noexcept;

    void pushFront(const T& value);
    void pushFront(T&& value);
    void pushBack(const T& value);
    void pushBack(T&& value);
    void popFront() noexcept;
    void popBack() noexcept;

    void clear() noexcept;
    void swap(List& other) noexcept;

    bool empty() const noexcept;
    std::size_t size() const noexcept;

  private:
    detail::Node< T > fake_;
    std::size_t size_;
  };

  template< class T >
  List< T >::List() noexcept:
    fake_(),
    size_(0)
  {}

  template< class T >
  List< T >::List(const List& other):
    List()
  {
    try
    {
      for (const_iterator it = other.cbegin(); it != other.cend(); ++it)
      {
        pushBack(*it);
      }
    }
    catch (...)
    {
      clear();
      throw;
    }
  }

  template< class T >
  List< T >::List(List&& other) noexcept:
    List()
  {
    swap(other);
  }

  template< class T >
  List< T >::~List()
  {
    clear();
  }

  template< class T >
  List< T >& List< T >::operator=(const List& other)
  {
    if (this != std::addressof(other))
    {
      List< T > temp(other);
      swap(temp);
    }
    return *this;
  }

  template< class T >
  List< T >& List< T >::operator=(List&& other) noexcept
  {
    if (this != std::addressof(other))
    {
      clear();
      swap(other);
    }
    return *this;
  }

  template< class T >
  typename List< T >::iterator List< T >::begin() noexcept
  {
    return iterator(fake_.next_);
  }

  template< class T >
  typename List< T >::iterator List< T >::end() noexcept
  {
    return iterator(std::addressof(fake_));
  }

  template< class T >
  typename List< T >::const_iterator List< T >::begin() const noexcept
  {
    return const_iterator(fake_.next_);
  }

  template< class T >
  typename List< T >::const_iterator List< T >::end() const noexcept
  {
    return const_iterator(std::addressof(fake_));
  }

  template< class T >
  typename List< T >::const_iterator List< T >::cbegin() const noexcept
  {
    return begin();
  }

  template< class T >
  typename List< T >::const_iterator List< T >::cend() const noexcept
  {
    return end();
  }

  template< class T >
  T& List< T >::front()
  {
    return *begin();
  }

  template< class T >
  const T& List< T >::front() const
  {
    return *cbegin();
  }

  template< class T >
  T& List< T >::back()
  {
    iterator it = end();
    --it;
    return *it;
  }

  template< class T >
  const T& List< T >::back() const
  {
    const_iterator it = cend();
    --it;
    return *it;
  }

  template< class T >
  typename List< T >::iterator List< T >::insert(iterator position, const T& value)
  {
    detail::Node< T >* node = new detail::Node< T >(value);
    detail::linkBefore(position.node_, node);
    ++size_;
    return iterator(node);
  }

  template< class T >
  typename List< T >::iterator List< T >::insert(iterator position, T&& value)
  {
    detail::Node< T >* node = new detail::Node< T >(std::move(value));
    detail::linkBefore(position.node_, node);
    ++size_;
    return iterator(node);
  }

  template< class T >
  typename List< T >::iterator List< T >::erase(iterator position) noexcept
  {
    detail::Node< T >* next = position.node_->next_;
    detail::unlink(position.node_);
    delete position.node_;
    --size_;
    return iterator(next);
  }

  template< class T >
  void List< T >::pushFront(const T& value)
  {
    insert(begin(), value);
  }

  template< class T >
  void List< T >::pushFront(T&& value)
  {
    insert(begin(), std::move(value));
  }

  template< class T >
  void List< T >::pushBack(const T& value)
  {
    insert(end(), value);
  }

  template< class T >
  void List< T >::pushBack(T&& value)
  {
    insert(end(), std::move(value));
  }

  template< class T >
  void List< T >::popFront() noexcept
  {
    erase(begin());
  }

  template< class T >
  void List< T >::popBack() noexcept
  {
    iterator it = end();
    --it;
    erase(it);
  }

  template< class T >
  void List< T >::clear() noexcept
  {
    while (!empty())
    {
      popFront();
    }
  }

  template< class T >
  void List< T >::swap(List& other) noexcept
  {
    if (this == std::addressof(other))
    {
      return;
    }

    using std::swap;
    swap(fake_.next_, other.fake_.next_);
    swap(fake_.prev_, other.fake_.prev_);
    swap(size_, other.size_);

    if (size_ == 0)
    {
      fake_.next_ = std::addressof(fake_);
      fake_.prev_ = std::addressof(fake_);
    }
    else
    {
      fake_.next_->prev_ = std::addressof(fake_);
      fake_.prev_->next_ = std::addressof(fake_);
    }

    if (other.size_ == 0)
    {
      other.fake_.next_ = std::addressof(other.fake_);
      other.fake_.prev_ = std::addressof(other.fake_);
    }
    else
    {
      other.fake_.next_->prev_ = std::addressof(other.fake_);
      other.fake_.prev_->next_ = std::addressof(other.fake_);
    }
  }

  template< class T >
  bool List< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class T >
  std::size_t List< T >::size() const noexcept
  {
    return size_;
  }

  template< class T >
  void swap(List< T >& lhs, List< T >& rhs) noexcept
  {
    lhs.swap(rhs);
  }
}

#endif
