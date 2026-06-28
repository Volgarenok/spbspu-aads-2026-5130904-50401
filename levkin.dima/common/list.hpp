#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
namespace levkin {
  template < class T > class List;
  template < class T > class LIter;
  template < class T > class LCIter;
  namespace detail {
    struct NodeBase {
      NodeBase* prev;
      NodeBase* next;
    };
    template < class T > struct Node: public NodeBase {
      T val;
    };
  }
  template < class T > class List
  {
  public:
    LIter< T > begin() noexcept;
    LCIter< T > begin() const noexcept;
    LCIter< T > cbegin() const noexcept;
    LIter< T > end() noexcept;
    LCIter< T > end() const noexcept;
    LCIter< T > cend() const noexcept;
    void pushFront(T val);
    void pushBack(T val);
    List();
    size_t size() const noexcept;
    void clearAndInit(size_t size, T val);
    void erase(LIter< T > from, LIter< T > to);
    LIter< T > erase(LIter< T > pos);
    void popFront();
    void popBack();
    LIter< T > insertAfter(LIter< T > it, const T& val);
    void clear() noexcept;
    List(T val);
    ~List();
    List(const List< T >& a);
    List(List< T >&& a) noexcept;
    List< T >& operator=(List< T > a) noexcept;
  private:
    void swap(List< T >& a) noexcept;
    List(detail::NodeBase* pseudo_node);
    LIter< T > __eraseFast(LIter< T > pos) noexcept;
    detail::NodeBase* pseudo;
  };
  template < class T > class LCIter
  {
    friend class List< T >;
    friend class LIter< T >;
  public:
    LCIter(detail::NodeBase* node = nullptr) noexcept;
    LCIter(LIter< T > it) noexcept;
    const T& operator*() const noexcept;
    T const* operator->() const noexcept;
    LCIter& operator++() noexcept;
    LCIter operator++(int) noexcept;
    LCIter& operator--() noexcept;
    LCIter operator--(int) noexcept;
    bool operator==(const LCIter& other) const noexcept;
    bool operator!=(const LCIter& other) const noexcept;
    bool operator==(const LIter< T >& other) const noexcept;
    bool operator!=(const LIter< T >& other) const noexcept;
  private:
    detail::NodeBase* curr = nullptr;
  };
  template < class T > class LIter
  {
    friend class List< T >;
    friend class LCIter< T >;
  public:
    LIter(detail::NodeBase* node) noexcept;
    T& operator*() noexcept;
    T* operator->() noexcept;
    LIter& operator++() noexcept;
    LIter operator++(int) noexcept;
    LIter& operator--() noexcept;
    LIter operator--(int) noexcept;
    bool operator==(const LIter& other) const noexcept;
    bool operator!=(const LIter& other) const noexcept;
    bool operator==(const LCIter< T >& other) const noexcept;
    bool operator!=(const LCIter< T >& other) const noexcept;
  private:
    detail::NodeBase* curr = nullptr;
  };
  template < class T >
  LIter< T > List< T >::begin() noexcept
  {
    return LIter< T >(pseudo->next);
  }
  template < class T >
  LCIter< T > List< T >::begin() const noexcept
  {
    return cbegin();
  }
  template < class T >
  LCIter< T > List< T >::cbegin() const noexcept
  {
    return LCIter< T >(pseudo->next);
  }
  template < class T >
  LIter< T > List< T >::end() noexcept
  {
    return LIter< T >(pseudo);
  }
  template < class T >
  LCIter< T > List< T >::end() const noexcept
  {
    return cend();
  }
  template < class T >
  LCIter< T > List< T >::cend() const noexcept
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
  List< T >::List() : pseudo(new detail::NodeBase())
  {
    pseudo->next = pseudo;
    pseudo->prev = pseudo;
  }
  template < class T >
  size_t List< T >::size() const noexcept
  {
    auto it = cbegin();
    size_t count = 0;
    for (; it != cend(); ++it) {
      count++;
    }
    return count;
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
  void List< T >::erase(LIter< T > from, LIter< T > to)
  {
    while (from != to) {
      erase(from++);
    }
  }
  template < class T >
  LIter< T > List< T >::erase(LIter< T > pos)
  {
    if (pos == end()) {
      return pos;
    }
    return __eraseFast(pos);
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
      throw std::out_of_range("out of bounds or null. wierd");
    }
    detail::Node< T >* newNode = new detail::Node< T >();
    newNode->val = val;
    newNode->prev = it.curr;
    newNode->next = it.curr->next;
    newNode->next->prev = newNode;
    it.curr->next = newNode;
    return LIter< T >(newNode);
  }
  template < class T >
  void List< T >::clear() noexcept
  {
    while (pseudo->next != pseudo) {
      __eraseFast(LIter< T >(pseudo->next));
    }
  }
  template < class T >
  List< T >::List(T val) : List()
  {
    pushBack(val);
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
  List< T >::List(const List< T >& a) : List()
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
  List< T >::List(List< T >&& a) noexcept : List(a.pseudo)
  {
    a.pseudo = nullptr;
  }
  template < class T >
  List< T >& List< T >::operator=(List< T > a) noexcept
  {
    a.swap(*this);
    return *this;
  }
  template < class T >
  void List< T >::swap(List< T >& a) noexcept
  {
    std::swap(a.pseudo, this->pseudo);
  }
  template < class T >
  List< T >::List(detail::NodeBase* pseudo_node) : pseudo(pseudo_node)
  {
  }
  template < class T >
  LIter< T > List< T >::__eraseFast(LIter< T > pos) noexcept
  {
    detail::NodeBase* toDelete = pos.curr;
    toDelete->prev->next = toDelete->next;
    toDelete->next->prev = toDelete->prev;
    detail::NodeBase* nxt = toDelete->next;
    delete static_cast< detail::Node< T >* >(toDelete);
    return LIter< T >(nxt);
  }
  template < class T >
  LCIter< T >::LCIter(detail::NodeBase* node) noexcept : curr(node)
  {
  }
  template < class T >
  LCIter< T >::LCIter(LIter< T > it) noexcept : curr(it.curr)
  {
  }
  template < class T >
  const T& LCIter< T >::operator*() const noexcept
  {
    return static_cast< detail::Node< T >* >(curr)->val;
  }
  template < class T >
  LCIter< T >& LCIter< T >::operator++() noexcept
  {
    curr = curr->next;
    return *this;
  }
  template < class T >
  LCIter< T > LCIter< T >::operator++(int) noexcept
  {
    LCIter temp = *this;
    ++(*this);
    return temp;
  }
  template < class T >
  LCIter< T >& LCIter< T >::operator--() noexcept
  {
    curr = curr->prev;
    return *this;
  }
  template < class T >
  LCIter< T > LCIter< T >::operator--(int) noexcept
  {
    LCIter temp = *this;
    --(*this);
    return temp;
  }
  template < class T >
  T const* LCIter< T >::operator->() const noexcept
  {
    return &static_cast< detail::Node< T >* >(curr)->val;
  }
  template < class T >
  bool LCIter< T >::operator==(const LCIter& other) const noexcept
  {
    return curr == other.curr;
  }
  template < class T >
  bool LCIter< T >::operator!=(const LCIter& other) const noexcept
  {
    return !(*this == other);
  }
  template < class T >
  bool LCIter< T >::operator==(const LIter< T >& other) const noexcept
  {
    return curr == other.curr;
  }
  template < class T >
  bool LCIter< T >::operator!=(const LIter< T >& other) const noexcept
  {
    return curr != other.curr;
  }
  template < class T >
  LIter< T >::LIter(detail::NodeBase* node) noexcept : curr(node)
  {
  }
  template < class T >
  T& LIter< T >::operator*() noexcept
  {
    return static_cast< detail::Node< T >* >(curr)->val;
  }
  template < class T >
  LIter< T >& LIter< T >::operator++() noexcept
  {
    curr = curr->next;
    return *this;
  }
  template < class T >
  LIter< T > LIter< T >::operator++(int) noexcept
  {
    LIter temp = *this;
    ++(*this);
    return temp;
  }
  template < class T >
  LIter< T >& LIter< T >::operator--() noexcept
  {
    curr = curr->prev;
    return *this;
  }
  template < class T >
  LIter< T > LIter< T >::operator--(int) noexcept
  {
    LIter temp = *this;
    --(*this);
    return temp;
  }
  template < class T >
  T* LIter< T >::operator->() noexcept
  {
    return &(static_cast< detail::Node< T >* >(curr)->val);
  }
  template < class T >
  bool LIter< T >::operator==(const LIter& other) const noexcept
  {
    return curr == other.curr;
  }
  template < class T >
  bool LIter< T >::operator!=(const LIter& other) const noexcept
  {
    return !(*this == other);
  }
  template < class T >
  bool LIter< T >::operator==(const LCIter< T >& other) const noexcept
  {
    return curr == other.curr;
  }
  template < class T >
  bool LIter< T >::operator!=(const LCIter< T >& other) const noexcept
  {
    return curr != other.curr;
  }
}
#endif
