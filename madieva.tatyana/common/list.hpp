#ifndef LIST_HPP
#define LIST_HPP
#include <iostream>
#include <cassert>
namespace madieva {
  template< class T > class List;
  template< class T > class LIter;
  template< class T > class LCIter;

  template< class T >
  class Node {
    friend class List< T >;
    friend class LIter< T >;
    friend class LCIter< T >;
    T val;
    Node< T > * next;
    Node< T > * prev;
    Node(const T & a, Node< T > * n, Node< T > * p) noexcept;
  };

  template< class T >
  class LIter {
    friend class List< T >;
    Node< T > * it;
  public:
    LIter(Node< T > * a) noexcept;
    void operator++() noexcept;
    void operator--() noexcept;
    bool operator==(const LIter< T > & a) const noexcept;
    bool operator!=(const LIter< T > & a) const noexcept;
    T& operator*() noexcept;
  };

  template< class T >
  class LCIter {
    friend class List< T >;
    const Node< T > * it;
  public:
    LCIter(const Node< T > * a) noexcept;
    void operator++() noexcept;
    void operator--() noexcept;
    bool operator==(const LCIter< T > & a) const noexcept;
    bool operator!=(const LCIter< T > & a) const noexcept;
    const T & operator*() const noexcept;
  };

  template< class T >
  class List {
    Node< T > * head;
    size_t size_;
    public:
    List() noexcept;
    ~List() noexcept;
    List(const List& a);
    List& operator=(const List& a);
    void clear() noexcept;
    void push_front(const T & a);
    void push_back(const T & a);
    void pop_front() noexcept;
    void pop_back() noexcept;
    LIter< T > begin() noexcept;
    LCIter< T > begin() const noexcept;
    size_t size() const noexcept;
  };

  template< class T >
  Node< T >::Node(const T & a, Node< T > * n, Node< T > * p) noexcept
    : val(a), next(n), prev(p)
  {}

  template< class T >
  LIter< T >::LIter(Node< T > * a) noexcept
    :it(a)
  {}

  template< class T >
  void LIter< T >::operator++() noexcept
  {
    assert(it);
    it = it->next;
  }

  template< class T >
  void LIter< T >::operator--() noexcept
  {
    assert(it);
    it = it->prev;
  }

  template< class T >
  bool LIter< T >::operator==(const LIter< T > & a) const noexcept
  {
    return(it == a.it);
  }

  template< class T >
  bool LIter< T >::operator!=(const LIter< T > & a) const noexcept
  {
    return(it != a.it);
  }

  template< class T >
  T & LIter< T >::operator*() noexcept
  {
    assert(it);
    return it->val;
  }

  template< class T >
  LCIter< T >::LCIter(const Node< T > * a) noexcept
    : it(a)
  {}

  template< class T >
  void LCIter< T >::operator++() noexcept
  {
    assert(it);
    it = it->next;
  }

  template< class T >
  void LCIter< T >::operator--() noexcept
  {
    assert(it);
    it = it->prev;
  }

  template< class T >
  bool LCIter< T >::operator==(const LCIter< T > & a) const noexcept
  {
    return(it == a.it);
  }

  template< class T >
  bool LCIter< T >::operator!=(const LCIter< T > & a) const noexcept
  {
    return(it != a.it);
  }

  template< class T >
  const T & LCIter< T >::operator*() const noexcept
  {
    assert(it);
    return it->val;
  }

  template< class T >
  List< T >::List() noexcept
    : head(nullptr), size_(0)
  {}

  template< class T >
  List< T >::~List() noexcept
  {
    clear();
  }

  template< class T >
  List< T >::List(const List< T > & a) : head(nullptr), size_(0)
  {
    Node< T > * temp = a.head;
    for (size_t i = 0; i < a.size_; ++i) {
      try{
        this->push_back(temp->val);
        temp = temp->next;
      } catch (const std::bad_alloc&) {
        clear();
        throw;
      }
    }
  }

  template< class T >
  List< T > & List< T >::operator=(const List< T > & a)
  {
    if(this == &a) {
      return *(this);
    }
    this->clear();
    Node< T > * temp = a.head;
    for (size_t i = 0; i < a.size_; ++i) {
      this->push_back(temp->val);
      temp = temp->next;
    }
    return *(this);
  }

  template< class T >
  void List< T >::clear() noexcept
  {
    if (head) {
      Node< T > * end = head->prev;
      while (head != end) {
        Node< T > * temp = head->next;
        delete head;
        head = temp;
      }
      delete head;
      head = nullptr;
      size_ = 0;
    }
  }

  template< class T >
  LIter< T > List< T >::begin() noexcept
  {
    return LIter< T > (head);
  }

  template< class T >
  LCIter< T > List< T >::begin() const noexcept
  {
    return LCIter< T > (head);
  }

  template< class T >
  void List< T >::push_front(const T & a)
  {
    if (!head) {
      head = new Node< T >(a, nullptr, nullptr);
      head->next = head;
      head->prev = head;
      size_ = 1;
    } else {
      Node< T > * temp = new Node< T >(a, head, head->prev);
      head->prev->next = temp;
      head->prev = temp;
      head = temp;
      size_++;
    }
  }

  template< class T >
  void List< T >::push_back(const T & a)
  {
    if (!head) {
      head = new Node< T >(a, nullptr, nullptr);
      head->next = head;
      head->prev = head;
      size_ = 1;
    } else {
      Node< T > * temp = new Node< T >(a, head, head->prev);
      head->prev->next = temp;
      head->prev = temp;
      size_++;
    }
  }

  template< class T >
  void List< T >::pop_front() noexcept
  {
    if(size_) {
      if(size_ == 1) {
        delete head;
        head = nullptr;
        size_ = 0;
      } else {
        head->next->prev = head->prev;
        head->prev->next = head->next;
        Node< T > * a = head->next;
        delete head;
        head = a;
        size_--;
      }
    }
  }

  template< class T >
  void List< T >::pop_back() noexcept
  {
    if (size_) {
      if(size_ == 1) {
        delete head;
        head= nullptr;
        size_ = 0;
      } else{
        Node< T > * a = head->prev;
        a->next->prev = a->prev;
        a->prev->next = a->next;
        delete a;
        size_--;
      }
    }
  }

  template< class T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }
}
#endif
