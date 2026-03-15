#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <utility>

namespace petrov {
  template< typename T >
  struct Node {
    T data_;
    Node< T > * next_;

    Node(const T & data, Node< T > * next = nullptr):
      data_(data),
      next_(next)
    {}
  };

  template< typename T >
  class List {
  public:
    class Iterator {
    public:
      Iterator(Node< T > * node);

      T & operator*();
      T * operator->();
      Iterator & operator++();
      bool operator==(const Iterator & other) const;
      bool operator!=(const Iterator & other) const;

    private:
      Node< T > * current_;
    };

    class ConstIterator {
    public:
      ConstIterator(const Node< T > * node);

      const T & operator*() const;
      const T * operator->() const;
      ConstIterator & operator++();
      bool operator==(const ConstIterator & other) const;
      bool operator!=(const ConstIterator & other) const;

    private:
      const Node< T > * current_;
    };

    List();
    List(const List< T > & other);
    ~List();

    List< T > & operator=(List< T > other);

    void pushBack(const T & value);
    void clear();

    T & getBack();
    const T & getBack() const;

    Iterator begin();
    Iterator end();
    ConstIterator begin() const;
    ConstIterator end() const;

    bool isEmpty() const;
    size_t getSize() const;

  private:
    Node< T > * head_;
    Node< T > * tail_;
    size_t size_;
  };

  template< typename T >
  List< T >::Iterator::Iterator(Node< T > * node):
    current_(node)
  {}

  template< typename T >
  T & List< T >::Iterator::operator*()
  {
    return current_->data_;
  }

  template< typename T >
  T * List< T >::Iterator::operator->()
  {
    return std::addressof(current_->data_);
  }

  template< typename T >
  typename List< T >::Iterator & List< T >::Iterator::operator++()
  {
    if (current_) {
      current_ = current_->next_;
    }
    return * this;
  }

  template< typename T >
  bool List< T >::Iterator::operator==(const Iterator & other) const
  {
    return current_ == other.current_;
  }

  template< typename T >
  bool List< T >::Iterator::operator!=(const Iterator & other) const
  {
    return ! (* this == other);
  }

  template< typename T >
  List< T >::ConstIterator::ConstIterator(const Node< T > * node):
    current_(node)
  {}

  template< typename T >
  const T & List< T >::ConstIterator::operator*() const
  {
    return current_->data_;
  }

  template< typename T >
  const T * List< T >::ConstIterator::operator->() const
  {
    return std::addressof(current_->data_);
  }

  template< typename T >
  typename List< T >::ConstIterator & List< T >::ConstIterator::operator++()
  {
    if (current_) {
      current_ = current_->next_;
    }
    return * this;
  }

  template< typename T >
  bool List< T >::ConstIterator::operator==(const ConstIterator & other) const
  {
    return current_ == other.current_;
  }

  template< typename T >
  bool List< T >::ConstIterator::operator!=(const ConstIterator & other) const
  {
    return ! (* this == other);
  }

  template< typename T >
  List< T >::List():
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {}

  template< typename T >
  List< T >::List(const List< T > & other):
    List()
  {
    for (Node< T > * curr = other.head_; curr != nullptr; curr = curr->next_) {
      pushBack(curr->data_);
    }
  }

  template< typename T >
  List< T >::~List()
  {
    clear();
  }

  template< typename T >
  List< T > & List< T >::operator=(List< T > other)
  {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
    return * this;
  }

  template< typename T >
  void List< T >::pushBack(const T & value)
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
  void List< T >::clear()
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
  T & List< T >::getBack()
  {
    return tail_->data_;
  }

  template< typename T >
  const T & List< T >::getBack() const
  {
    return tail_->data_;
  }

  template< typename T >
  typename List< T >::Iterator List< T >::begin()
  {
    return Iterator(head_);
  }

  template< typename T >
  typename List< T >::Iterator List< T >::end()
  {
    return Iterator(nullptr);
  }

  template< typename T >
  typename List< T >::ConstIterator List< T >::begin() const
  {
    return ConstIterator(head_);
  }

  template< typename T >
  typename List< T >::ConstIterator List< T >::end() const
  {
    return ConstIterator(nullptr);
  }

  template< typename T >
  bool List< T >::isEmpty() const
  {
    return head_ == nullptr;
  }

  template< typename T >
  size_t List< T >::getSize() const
  {
    return size_;
  }
}

#endif