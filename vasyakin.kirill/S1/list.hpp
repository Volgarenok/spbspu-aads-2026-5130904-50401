#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <utility>
#include <limits>

namespace vasyakin
{
  template< class T > class Node;
  template< class T > class List;
  const size_t MAX = std::numeric_limits< size_t >::max();

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

  template< class T >
  LCIter< T >& LCIter< T >::operator++()
  {
    ptr = ptr->next;
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator++(int)
  {
    LCIter< T > tmp = *this;
    ptr = ptr->next;
    return tmp;
  }

  template< class T >
  bool LCIter< T >::operator==(const LCIter& other) const
  {
    return ptr == other.ptr;
  }

  template< class T >
  bool LCIter< T >::operator!=(const LCIter& other) const
  {
    return ptr != other.ptr;
  }

  template< class T >
  const Node< T >* LCIter< T >::get_ptr() const
  {
    return ptr;
  }

  template< class T >
  Node< T >::Node(const T& value):
    val(value),
    next(nullptr)
  {}

  template< class T >
  List< T >::List():
    size(0)
  {
    fake_node = new Node< T >(T{});
    fake_node->next = fake_node;
  }

  template< class T >
  List< T >::~List()
  {
    if (!fake_node)
    {
      return;
    }
    clear();
    delete fake_node;
    fake_node = nullptr;
  }

  template< class T >
  void List< T >::clear()
  {
    if (!fake_node)
    {
      return;
    }
    if (fake_node->next == fake_node)
    {
      size = 0;
      return;
    }

    Node< T >* current = fake_node->next;
    while (current != fake_node)
    {
      Node< T >* next = current->next;
      delete current;
      current = next;
    }
    fake_node->next = fake_node;
    size = 0;
  }

  template< class T >
  List< T >::List(const List& other):
    size(0)
  {
    fake_node = new Node< T >(T{});
    fake_node->next = fake_node;

    if (other.fake_node->next == other.fake_node)
    {
      return;
    }

    Node< T >* other_cur = other.fake_node->next;
    Node< T >* prev = fake_node;

    while (other_cur != other.fake_node)
    {
      Node< T >* new_node = new Node< T >(other_cur->val);
      prev->next = new_node;
      prev = new_node;
      other_cur = other_cur->next;
      ++size;
    }
    prev->next = fake_node;
  }

  template< class T >
  List< T >::List(List&& other) noexcept:
    fake_node(other.fake_node),
    size(other.size)
  {
    other.fake_node = nullptr;
    other.size = 0;
  }

  template< class T >
  List< T >& List< T >::operator=(const List& other)
  {
    if (this != &other)
    {
      List tmp(other);
      swap(tmp);
    }
    return *this;
  }

  template< class T >
  List< T >& List< T >::operator=(List&& other) noexcept
  {
    if (this != &other)
    {
      List tmp(std::move(other));
      swap(tmp);
    }
    return *this;
  }

  template< class T >
  List< T >::List(const T& value):
    size(1)
  {
    fake_node = new Node< T >(T{});
    Node< T >* head = new Node< T >(value);
    head->next = fake_node;
    fake_node->next = head;
  }

  template< class T >
  Node< T >* List< T >::insert(Node< T >* h, const T& value)
  {
    Node< T >* newNode = new Node< T >(value);

    if (fake_node->next == fake_node)
    {
      newNode->next = fake_node;
      fake_node->next = newNode;
    }
    else
    {
      newNode->next = h->next;
      h->next = newNode;
    }
    ++size;
    return newNode;
  }

  template< class T >
  Node< T >* List< T >::erase(Node< T >* h)
  {
    if (!fake_node || h->next == fake_node)
    {
      return fake_node;
    }
    Node< T >* to_delete = h->next;
    h->next = to_delete->next;
    delete to_delete;
    --size;
    return h;
  }

  template< class T >
  Node< T >* List< T >::push_back(const T& value)
  {
    Node< T >* newNode = new Node< T >(value);
    Node< T >* last = fake_node;
    while (last->next != fake_node)
    {
      last = last->next;
    }
    newNode->next = fake_node;
    last->next = newNode;
    ++size;
    return newNode;
  }

  template< class T >
  LIter< T > List< T >::begin()
  {
    return LIter< T >(fake_node->next);
  }

  template< class T >
  LIter< T > List< T >::end()
  {
    return LIter< T >(fake_node);
  }

  template< class T >
  LCIter< T > List< T >::begin() const
  {
    return LCIter< T >(fake_node->next);
  }

  template< class T >
  LCIter< T > List< T >::end() const
  {
    return LCIter< T >(fake_node);
  }

  template< class T >
  LCIter< T > List< T >::cbegin() const
  {
    return LCIter< T >(fake_node->next);
  }

  template< class T >
  LCIter< T > List< T >::cend() const
  {
    return LCIter< T >(fake_node);
  }

  template< class T >
  void List< T >::swap(List& other) noexcept
  {
    std::swap(fake_node, other.fake_node);
    std::swap(size, other.size);
  }

  template< class T >
  size_t List< T >::get_size() const
  {
    return size;
  }

  template< class T >
  Node< T >* List< T >::get_fake()
  {
    return fake_node;
  }

  template< class T >
  Node< T >* List< T >::get_first() const
  {
    return fake_node->next;
  }
}

#endif
