#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
#include <utility>
#include <memory>

namespace zharov {

  template< class T >
  struct Node {
    T val_;
    Node< T > * next_;
    Node< T > * prev_;
  };

  template< class T >
  class List;

  template< class T >
  class LIter {
    friend class List< T >;
    Node< T > * curr_;
    LIter(Node< T > * node);
  public:
    T & operator*() const;
    T * operator->() const;
    LIter & operator++();
    LIter operator++(int);
    LIter & operator--();
    LIter operator--(int);
    bool operator==(const LIter & it) const;
    bool operator!=(const LIter & it) const;
  };

    template< class T >
  class LCIter {
    friend class List< T >;
    const Node< T > * curr_;
    LCIter(const Node< T > * node);
  public:
    const T & operator*() const;
    const T * operator->() const;
    LCIter & operator++();
    LCIter operator++(int);
    LCIter & operator--();
    LCIter operator--(int);
    bool operator==(const LCIter & it) const;
    bool operator!=(const LCIter & it) const;
  };

  template< class T >
  class List {
    Node< T > * head_;
    Node< T > * tail_;
    size_t size_;
  public:
    List();
    List(const List & h);
    List(List && h) noexcept;
    ~List();
    List & operator=(const List & h);
    List & operator=(List && h) noexcept;
    LIter< T > begin();
    LIter< T > end();
    LCIter< T > constBegin() const;
    LCIter< T > constEnd() const;
    T & front();
    const T & front() const;
    T & back();
    const T & back() const;
    void pushFront(const T & v);
    void pushBack(const T & v);
    LIter< T > insert(LIter< T > pos, const T & v);
    void popFront();
    void popBack();
    LIter< T > erase(LIter< T > pos);
    void clear();
    size_t size() const;
  };

  template< class T >
  LIter< T >::LIter(Node< T > * node):
    curr_(node)
  {}

  template< class T >
  T & LIter< T >::operator*() const
  {
    return curr_->val_;
  }

  template< class T >
  T * LIter< T >::operator->() const
  {
    return std::addressof(curr_->val_);
  }

  template< class T >
  LIter< T > & LIter< T >::operator++()
  {
    curr_ = curr_->next_;
    return *this;
  }

  template< class T >
  LIter< T >  LIter< T >::operator++(int)
  {
    LIter< T > temp = *this;
    curr_ = curr_->next_;
    return temp;
  }

  template< class T >
  LIter< T > & LIter< T >::operator--()
  {
    curr_ = curr_->prev_;
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator--(int)
  {
    LIter< T > temp = *this;
    curr_ = curr_->prev_;
    return temp;
  }

  template< class T >
  bool LIter< T >::operator==(const LIter & it) const
  {
    return curr_ == it.curr_;
  }

  template< class T >
  bool LIter< T >::operator!=(const LIter & it) const
  {
    return curr_ != it.curr_;
  }

    template< class T >
  LCIter< T >::LCIter(const Node< T > * node):
    curr_(node)
  {}

  template< class T >
  const T & LCIter< T >::operator*() const
  {
    return curr_->val_;
  }

  template< class T >
  const T * LCIter< T >::operator->() const
  {
    return std::addressof(curr_->val_);
  }

  template< class T >
  LCIter< T > & LCIter< T >::operator++()
  {
    curr_ = curr_->next_;
    return *this;
  }

  template< class T >
  LCIter< T >  LCIter< T >::operator++(int)
  {
    LCIter< T > temp = *this;
    curr_ = curr_->next_;
    return temp;
  }

  template< class T >
  LCIter< T > & LCIter< T >::operator--()
  {
    curr_ = curr_->prev_;
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator--(int)
  {
    LCIter< T > temp = *this;
    curr_ = curr_->prev_;
    return temp;
  }

  template< class T >
  bool LCIter< T >::operator==(const LCIter & it) const
  {
    return curr_ == it.curr_;
  }

  template< class T >
  bool LCIter< T >::operator!=(const LCIter & it) const
  {
    return curr_ != it.curr_;
  }

  template< class T >
  List< T >::List():
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {}

  template< class T >
  List< T >::List(const List< T > & h):
    List()
  {
    for (Node< T > * curr = h.head_; curr != nullptr; curr = curr->next_) {
      pushBack(curr->val_);
    }
  }

  template< class T >
  List< T >::List(List< T > && h) noexcept:
    head_(h.head_),
    tail_(h.tail_),
    size_(h.size_)
  {
    h.head_ = nullptr;
    h.tail_ = nullptr;
    h.size_ = 0;
  }

  template< class T >
  List< T >::~List()
  {
    clear();
  }

  template< class T >
  List< T > & List< T >::operator=(const List< T > & h)
  {
    if (this != std::addressof(h)) {
      List< T > temp(h);
      std::swap(temp.head_, head_);
      std::swap(temp.tail_, tail_);
      std::swap(temp.size_, size_);
    }
    return *this;
  }

  template< class T >
  List< T > & List< T >::operator=(List< T > && h) noexcept
  {
    if (this != std::addressof(h)) {
      clear();
      head_ = h.head_;
      tail_ = h.tail_;
      size_ = h.size_;
      h.head_ = nullptr;
      h.tail_ = nullptr;
      h.size_ = 0;
    }
    return *this;
  }

  template< class T >
  LIter< T > List< T >::begin()
  {
    return LIter< T >(head_);
  }

  template< class T >
  LIter< T > List< T >::end()
  {
    return LIter< T >(nullptr);
  }

  template< class T >
  LCIter< T > List< T >::constBegin() const
  {
    return LCIter< T >(head_);
  }

  template< class T >
  LCIter< T > List< T >::constEnd() const
  {
    return LCIter< T >(nullptr);
  }

  template< class T >
  T & List< T >::front()
  {
    return head_->val_;
  }

  template< class T >
  const T & List< T >::front() const
  {
    return head_->val_;
  }

  template< class T >
  T & List< T >::back()
  {
    return tail_->val_;
  }

  template< class T >
  const T & List< T >::back() const
  {
    return tail_->val_;
  }

  template< class T >
  void List< T >::pushFront(const T & v)
  {
    Node< T > * new_node = new Node< T > {v, nullptr, nullptr};
    new_node->next_ = head_;
    if (head_) {
      head_->prev_ = new_node;
    } else {
      tail_ = new_node;
    }
    head_ = new_node;
    ++size_;
  }

  template< class T >
  void List< T >::pushBack(const T & v)
  {
    Node< T > * new_node = new Node< T > {v, nullptr, nullptr};
    new_node->prev_ = tail_;
    if (tail_) {
      tail_->next_ = new_node;
    } else {
      head_ = new_node;
    }
    tail_ = new_node;
    ++size_;
  }

  template< class T >
  LIter< T > List< T >::insert(LIter< T > pos, const T & v)
  {
    if (!pos.curr_) {
      pushBack(v);
      return LIter< T >(tail_);
    } else if (pos.curr_ == head_) {
      pushFront(v);
      return LIter< T >(head_);
    }
    Node< T > * new_node = new Node< T > {v, nullptr, nullptr};
    Node< T > * next = pos.curr_;
    Node< T > * prev = next->prev_;
    new_node->next_ = next;
    new_node->prev_ = prev;
    prev->next_ = new_node;
    next->prev_ = new_node;
    ++size_;
    return LIter< T >(new_node);
  }

  template< class T >
  void List< T >::popFront()
  {
    if (!head_) {
      return;
    }
    Node< T > * next = head_->next_;
    if (next) {
      next->prev_ = nullptr;
    } else {
      tail_ = nullptr;
    }
    delete head_;
    head_ = next;
    --size_;
  }

  template< class T >
  void List< T >::popBack()
  {
    if (!tail_) {
      return;
    }
    Node< T > * prev = tail_->prev_;
    if (prev) {
      prev->next_ = nullptr;
    } else {
      head_ = nullptr;
    }
    delete tail_;
    tail_ = prev;
    --size_;
  }

  template< class T >
  LIter< T > List< T >::erase(LIter< T > pos)
  {
    if (!pos.curr_) {
      return end();
    }
    Node< T > * next = pos.curr_->next_;
    Node< T > * prev = pos.curr_->prev_;
    if (next) {
      next->prev_ = prev;
    } else {
      tail_ = prev;
    }
    if (prev) {
      prev->next_ = next;
    } else {
      head_ = next;
    }
    delete pos.curr_;
    --size_;
    return LIter< T >(next);
  }

  template< class T >
  void List< T >::clear()
  {
    while (head_) {
      Node< T > * next = head_->next_;
      delete head_;
      head_ = next;
      --size_;
    }
    tail_ = nullptr;
  }

  template< class T >
  size_t List< T >::size() const
  {
    return size_;
  }
}

#endif
