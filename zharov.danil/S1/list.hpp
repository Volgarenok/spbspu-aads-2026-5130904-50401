#ifndef LIST_HPP
#define LIST_HPP

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
    LCIter< T > constBegin();
    LCIter< T > constEnd();
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
    size_t size();
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
}

#endif
