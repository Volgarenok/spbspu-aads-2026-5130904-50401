#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>
#include <limits>

namespace vasyakin
{
  template< class T > class Node;
  template< class T > class List;
  const int MAX = std::numeric_limits< int >::max();

  template< class T >
  class LIter
  {
  private:
    friend class List< T >;
    Node< T >* ptr;
    explicit LIter(Node< T >* p);

  public:
    T& operator*() const;
    T* operator->() const;
    LIter& operator++();
    LIter operator++(int);
    bool operator==(const LIter& other) const;
    bool operator!=(const LIter& other) const;
    Node< T >* get_ptr() const;
  };

  template< class T >
  class LCIter
  {
  private:
    friend class List< T >;
    const Node< T >* ptr;
    explicit LCIter(const Node< T >* p);
    explicit LCIter(const LIter< T >& it);

  public:
    const T& operator*() const;
    const T* operator->() const;
    LCIter& operator++();
    LCIter operator++(int);
    bool operator==(const LCIter& other) const;
    bool operator!=(const LCIter& other) const;
    const Node< T >* get_ptr() const;
  };

  template< class T >
  class Node
  {
  private:
    T val;
    Node< T >* next;
    friend class List< T >;
    friend class LIter< T >;
    friend class LCIter< T >;

  public:
    explicit Node(const T& value);
  };

  template< class T >
  class List
  {
  private:
    Node< T >* fake_node;
    size_t size;

  public:
    List();
    ~List();
    List(const List& other);
    List(List&& other) noexcept;
    List& operator=(const List& other);
    List& operator=(List&& other) noexcept;
    explicit List(const T& value);
    
    Node< T >* insert(Node< T >* h, const T& value);
    Node< T >* erase(Node< T >* h);
    Node< T >* push_back(const T& value);
    
    LIter< T > begin();
    LIter< T > end();
    LCIter< T > begin() const;
    LCIter< T > end() const;
    LCIter< T > cbegin() const;
    LCIter< T > cend() const;
    
    void swap(List& other) noexcept;
    size_t get_size() const;
    Node< T >* get_fake();
    Node< T >* get_first() const;
    void clear();
  };
  
  template< class T >
  LIter< T >::LIter(Node< T >* p):
    ptr(p)
  {}

  template< class T >
  T& LIter< T >::operator*() const
  {
    return ptr->val;
  }

  template< class T >
  T* LIter< T >::operator->() const
  {
    return &(ptr->val);
  }
  
  template< class T >
  LIter< T >& LIter< T >::operator++()
  {
    ptr = ptr->next;
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator++(int)
  {
    LIter< T > tmp = *this;
    ptr = ptr->next;
    return tmp;
  }

  template< class T >
  bool LIter< T >::operator==(const LIter& other) const
  {
    return ptr == other.ptr;
  }

  template< class T >
  bool LIter< T >::operator!=(const LIter& other) const
  {
    return ptr != other.ptr;
  }

  template< class T >
  Node< T >* LIter< T >::get_ptr() const
  {
    return ptr;
  }

  template< class T >
  LCIter< T >::LCIter(const Node< T >* p):
    ptr(p)
  {}

  template< class T >
  LCIter< T >::LCIter(const LIter< T >& it):
    ptr(it.ptr)
  {}

  template< class T >
  const T& LCIter< T >::operator*() const
  {
    return ptr->val;
  }

  template< class T >
  const T* LCIter< T >::operator->() const
  {
    return &(ptr->val);
  }

}

#endif