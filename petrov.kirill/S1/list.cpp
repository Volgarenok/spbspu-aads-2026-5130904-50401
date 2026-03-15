#include "list.hpp"

  template< typename T >
  petrov::List< T >::Iterator::Iterator(Node< T > * node):
    current_(node)
  {}

  template< typename T >
  T & petrov::List< T >::Iterator::operator*()
  {
    return current_->data_;
  }

  template< typename T >
  T * petrov::List< T >::Iterator::operator->()
  {
    return std::addressof(current_->data_);
  }

  template< typename T >
  typename petrov::List< T >::Iterator & petrov::List< T >::Iterator::operator++()
  {
    if (current_) {
      current_ = current_->next_;
    }
    return * this;
  }

  template< typename T >
  bool petrov::List< T >::Iterator::operator==(const Iterator & other) const
  {
    return current_ == other.current_;
  }

  template< typename T >
  bool petrov::List< T >::Iterator::operator!=(const Iterator & other) const
  {
    return ! (* this == other);
  }

  template< typename T >
  petrov::List< T >::ConstIterator::ConstIterator(const Node< T > * node):
    current_(node)
  {}

  template< typename T >
  const T & petrov::List< T >::ConstIterator::operator*() const
  {
    return current_->data_;
  }

  template< typename T >
  const T * petrov::List< T >::ConstIterator::operator->() const
  {
    return std::addressof(current_->data_);
  }

  template< typename T >
  typename petrov::List< T >::ConstIterator & petrov::List< T >::ConstIterator::operator++()
  {
    if (current_) {
      current_ = current_->next_;
    }
    return * this;
  }

  template< typename T >
  bool petrov::List< T >::ConstIterator::operator==(const ConstIterator & other) const
  {
    return current_ == other.current_;
  }

  template< typename T >
  bool petrov::List< T >::ConstIterator::operator!=(const ConstIterator & other) const
  {
    return ! (* this == other);
  }

  template< typename T >
  petrov::List< T >::List():
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {}

  template< typename T >
  petrov::List< T >::List(const petrov::List< T > & other):
    head_(nullptr), tail_(nullptr), size_(0)
  {
    for (Node< T > * curr = other.head_; curr != nullptr; curr = curr->next_) {
      pushBack(curr->data_);
    }
  }

  template< typename T >
  petrov::List< T >::~List()
  {
    clear();
  }

  template< typename T >
  petrov::List< T > & petrov::List< T >::operator=(petrov::List< T > other)
  {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
    return * this;
  }

  template< typename T >
  void petrov::List< T >::pushBack(const T & value)
  {
    Node< T > * new_node = new Node< T >(value);
    if (! head_) {
      head_ = tail_ = new_node;
    } else {
      tail_->next_ = new_node;
      tail_ = new_node;
    }
    ++size_;
  }

  template< typename T >
  void petrov::List< T >::clear()
  {
    while (head_) {
      Node< T > * temp = head_;
      head_ = head_->next_;
      delete temp;
    }
    tail_ = nullptr;
    size_ = 0;
  }

  template< typename T >
  T & petrov::List< T >::getBack()
  {
    return tail_->data_;
  }

  template< typename T >
  const T & petrov::List< T >::getBack() const
  {
    return tail_->data_;
  }

  template< typename T >
  typename petrov::List< T >::Iterator petrov::List< T >::begin()
  {
    return Iterator(head_);
  }

  template< typename T >
  typename petrov::List< T >::Iterator petrov::List< T >::end()
  {
    return Iterator(nullptr);
  }

  template< typename T >
  typename petrov::List< T >::ConstIterator petrov::List< T >::begin() const
  {
    return ConstIterator(head_);
  }

  template< typename T >
  typename petrov::List< T >::ConstIterator petrov::List< T >::end() const
  {
    return ConstIterator(nullptr);
  }

  template< typename T >
  bool petrov::List< T >::isEmpty() const
  {
    return head_ == nullptr;
  }

  template< typename T >
  size_t petrov::List< T >::getSize() const
  {
    return size_;
  }