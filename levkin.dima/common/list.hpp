#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>

namespace levkin {
  template < class T >
  class List;

  template < class T >
  class LIter;

  template < class T >
  class LCIter;

  struct NodeBase
  {
    NodeBase* prev;
    NodeBase* next;
  };

  template < class T >
  struct Node: public NodeBase
  {
    T val;
  };

  template < class T >
  class List
  {
  public:
    List();
    List(T val);
    List(const List< T >& a);
    List(List< T >&& a) noexcept;
    ~List();

    List< T >& operator=(List< T > a);

    LIter< T > begin();
    LCIter< T > begin() const;
    LCIter< T > cbegin() const;

    LIter< T > end();
    LCIter< T > end() const;
    LCIter< T > cend() const;

    void pushFront(T val);
    void pushBack(T val);
    void popFront();
    void popBack();

    LIter< T > insertAfter(LIter< T > it, const T& val);
    LIter< T > erase(LIter< T > pos);
    void erase(LIter< T > from, LIter< T > to);
    void clearAndInit(size_t size, T val);
    void clear();
    size_t size() const;

  private:
    NodeBase* pseudo;

    List(NodeBase* pseudoNode);
    void swap(List< T >& a);
    LIter< T > eraseFast(LIter< T > pos);
  };

  template < class T >
  List< T >::List():
    pseudo(new NodeBase())
  {
    pseudo->next = pseudo;
    pseudo->prev = pseudo;
  }

  template < class T >
  List< T >::List(T val):
    List()
  {
    pushBack(val);
  }

  template < class T >
  List< T >::List(const List< T >& a):
    List()
  {
    try {
      for (LCIter< T > i = a.cbegin(); i != a.cend(); i++) {
        pushBack(*i);
      }
    } catch (...) {
      clear();
      delete pseudo;
      throw;
    }
  }

  template < class T >
  List< T >::List(List< T >&& a) noexcept:
    pseudo(a.pseudo)
  {
    a.pseudo = nullptr;
  }

  template < class T >
  List< T >::~List()
  {
    if (pseudo == nullptr) {
      return;
    }
    clear();
    delete pseudo;
  }

  template < class T >
  List< T >& List< T >::operator=(List< T > a)
  {
    a.swap(*this);
    return *this;
  }

  template < class T >
  LIter< T > List< T >::begin()
  {
    return LIter< T >(pseudo->next);
  }

  template < class T >
  LCIter< T > List< T >::begin() const
  {
    return cbegin();
  }

  template < class T >
  LCIter< T > List< T >::cbegin() const
  {
    return LCIter< T >(pseudo->next);
  }

  template < class T >
  LIter< T > List< T >::end()
  {
    return LIter< T >(pseudo);
  }

  template < class T >
  LCIter< T > List< T >::end() const
  {
    return cend();
  }

  template < class T >
  LCIter< T > List< T >::cend() const
  {
    return LCIter< T >(pseudo);
  }

  template < class T >
  void List< T >::pushFront(T val)
  {
    insertAfter(LIter< T >(pseudo), val);
  }

  template < class T >
  void List< T >::pushBack(T val)
  {
    insertAfter(LIter< T >(pseudo->prev), val);
  }

  template < class T >
  void List< T >::popFront()
  {
    if (begin() != end()) {
      erase(begin());
    }
  }

  template < class T >
  void List< T >::popBack()
  {
    if (begin() != end()) {
      erase(LIter< T >(pseudo->prev));
    }
  }

  template < class T >
  LIter< T > List< T >::insertAfter(LIter< T > it, const T& val)
  {
    if (it.curr == nullptr) {
      throw std::out_of_range("out of bounds or null");
    }
    Node< T >* newNode = new Node< T >();
    newNode->val = val;
    newNode->prev = it.curr;
    newNode->next = it.curr->next;

    newNode->next->prev = newNode;
    it.curr->next = newNode;
    return LIter< T >(newNode);
  }

  template < class T >
  LIter< T > List< T >::erase(LIter< T > pos)
  {
    if (pos == end()) {
      return pos;
    }
    return eraseFast(pos);
  }

  template < class T >
  void List< T >::erase(LIter< T > from, LIter< T > to)
  {
    while (from != to) {
      erase(from++);
    }
  }

  template < class T >
  void List< T >::clearAndInit(size_t size, T val)
  {
    clear();
    for (size_t i = 0; i < size; ++i) {
      this->pushBack(val);
    }
  }

  template < class T >
  void List< T >::clear()
  {
    while (pseudo->next != pseudo) {
      eraseFast(LIter< T >(pseudo->next));
    }
  }

  template < class T >
  size_t List< T >::size() const
  {
    auto it = cbegin();
    size_t count = 0;
    for (; it != cend(); ++it) {
      count++;
    }
    return count;
  }

  template < class T >
  List< T >::List(NodeBase* pseudoNode):
    pseudo(pseudoNode)
  {}

  template < class T >
  void List< T >::swap(List< T >& a)
  {
    std::swap(a.pseudo, this->pseudo);
  }

  template < class T >
  LIter< T > List< T >::eraseFast(LIter< T > pos)
  {
    NodeBase* toDelete = pos.curr;
    toDelete->prev->next = toDelete->next;
    toDelete->next->prev = toDelete->prev;

    NodeBase* nxt = toDelete->next;
    delete static_cast< Node< T >* >(toDelete);
    return LIter< T >(nxt);
  }

  template < class T >
  class LCIter
  {
    friend class List< T >;
    friend class LIter< T >;

  public:
    LCIter(NodeBase* node = nullptr):
      curr(node)
    {}

    LCIter(LIter< T > it):
      curr(it.curr)
    {}

    const T& operator*() const
    {
      return static_cast< Node< T >* >(curr)->val;
    }

    LCIter& operator++()
    {
      curr = curr->next;
      return *this;
    }

    LCIter operator++(int)
    {
      LCIter temp = *this;
      ++(*this);
      return temp;
    }

    LCIter& operator--()
    {
      curr = curr->prev;
      return *this;
    }

    LCIter operator--(int)
    {
      LCIter temp = *this;
      --(*this);
      return temp;
    }

    const T* operator->() const
    {
      return &static_cast< Node< T >* >(curr)->val;
    }

    bool operator==(const LCIter& other) const
    {
      return curr == other.curr;
    }

    bool operator!=(const LCIter& other) const
    {
      return !(*this == other);
    }

    bool operator==(const LIter< T >& other) const
    {
      return curr == other.curr;
    }

    bool operator!=(const LIter< T >& other) const
    {
      return curr != other.curr;
    }

  private:
    NodeBase* curr = nullptr;
  };

  template < class T >
  class LIter
  {
    friend class List< T >;
    friend class LCIter< T >;

  public:
    LIter(NodeBase* node):
      curr(node)
    {}

    T& operator*()
    {
      return static_cast< Node< T >* >(curr)->val;
    }

    LIter& operator++()
    {
      curr = curr->next;
      return *this;
    }

    LIter operator++(int)
    {
      LIter temp = *this;
      ++(*this);
      return temp;
    }

    LIter& operator--()
    {
      curr = curr->prev;
      return *this;
    }

    LIter operator--(int)
    {
      LIter temp = *this;
      --(*this);
      return temp;
    }

    T* operator->()
    {
      return &(static_cast< Node< T >* >(curr)->val);
    }

    bool operator==(const LIter& other) const
    {
      return curr == other.curr;
    }

    bool operator!=(const LIter& other) const
    {
      return !(*this == other);
    }

    bool operator==(const LCIter< T >& other) const
    {
      return curr == other.curr;
    }

    bool operator!=(const LCIter< T >& other) const
    {
      return curr != other.curr;
    }

  private:
    NodeBase* curr = nullptr;
  };
}

#endif
