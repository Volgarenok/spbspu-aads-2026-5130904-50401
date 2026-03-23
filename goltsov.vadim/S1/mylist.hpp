#ifndef MYLIST_HPP
#define MYLIST_HPP
#include <stdexcept>
namespace goltsov
{
  template< class T >
  struct Node
  {
    T value;
    Node< T >* next;
  };

  template< class T >
  class List;

  template< class T >
  class LIter
  {
    friend class List< T >;
    Node< T >* ptr;
  public:
    LIter() noexcept;
    LIter(Node< T >* p) noexcept;
    bool hasNext() const noexcept;
    LIter< T > next() const;
    T& operator*() const;
    bool operator==(const LIter< T >& other) const noexcept;
    bool operator!=(const LIter< T >& other) const noexcept;
  };

  template< class T >
  class LCIter
  {
    friend class List< T >;
    const Node< T >* ptr;
  public:
    LCIter() noexcept;
    LCIter(const Node< T >* p) noexcept;
    bool hasNext() const noexcept;
    LCIter< T > next() const;
    const T& operator*() const;
    bool operator==(const LCIter< T >& other) const noexcept;
    bool operator!=(const LCIter< T >& other) const noexcept;
  };

  template< class T >
  class List
  {
    Node< T >* fake;
    Node< T >* createFake();
    void rmFake() noexcept;
  public:
    List();
    ~List();
    List(const List< T >& other);
    List(List< T >&& other);
    List< T >& operator=(const List< T >& other);
    List< T >& operator=(List< T >&& other);

    LIter< T > begin() noexcept;
    LCIter< T > begin() const noexcept;
    LIter< T > end() noexcept;
    LCIter< T > end() const noexcept;
    LIter< T > getLast() noexcept;
    LCIter< T > getLast() const noexcept;
    LIter< T > push_start(const T& a);
    void pop_start() noexcept;
    void pop_end() noexcept;
    LIter< T > insert(LIter< T > i, const T& a);
    LIter< T > insert(LIter< T > i, const T&& a);
    void clear() noexcept;
  };
}

namespace goltsov
{
  template< class T >
  LIter< T >::LIter() noexcept:
    ptr(nullptr)
  {}
  template< class T >
  LIter< T >::LIter(Node< T >* p) noexcept:
    ptr(p)
  {}
  template< class T >
  bool LIter< T >::hasNext() const noexcept
  {
    return ptr != nullptr;
  }
  template< class T >
  LIter< T > LIter< T >::next() const
  {
    if (ptr)
    {
      return {ptr->next};
    }
    else
    {
      throw std::runtime_error("Null pointer dereference");
    }
  }
  template< class T >
  T& LIter< T >::operator*() const
  {
    if (ptr)
    {
      return ptr->value;
    }
    else
    {
      throw std::runtime_error("Null pointer dereference");
    }
  }
  template< class T >
  bool LIter< T >::operator==(const LIter< T >& other) const noexcept
  {
    return ptr == other.ptr;
  }
  template< class T >
  bool LIter< T >::operator!=(const LIter< T >& other) const noexcept
  {
    return !((* this) == other);
  }

  template< class T >
  LCIter< T >::LCIter() noexcept:
    ptr(nullptr)
  {}
  template< class T >
  LCIter< T >::LCIter(const Node< T >* p) noexcept:
    ptr(p)
  {}
  template< class T >
  bool LCIter< T >::hasNext() const noexcept
  {
    return ptr != nullptr;
  }
  template< class T >
  LCIter< T > LCIter< T >::next() const
  {
    if (ptr)
    {
      return {ptr->next};
    }
    else
    {
      throw std::runtime_error("Null pointer dereference");
    }
  }
  template< class T >
  const T& LCIter< T >::operator*() const
  {
    if (ptr)
    {
      return ptr->value;
    }
    else
    {
      throw std::runtime_error("Null pointer dereference");
    }
  }
  template< class T >
  bool LCIter< T >::operator==(const LCIter< T >& other) const noexcept
  {
    return ptr == other.ptr;
  }
  template< class T >
  bool LCIter< T >::operator!=(const LCIter< T >& other) const noexcept
  {
    return !((* this) == other);
  }

  template< class T >
  Node< T >* List< T >::createFake()
  {
    Node< T >* el = new Node< T > {T(), nullptr};
    return el;
  }
  template< class T >
  void List< T >::rmFake() noexcept
  {
    delete fake;
  }
  template< class T >
  List< T >::List()
  {
    fake = createFake();
    fake->next = nullptr;
  }
  template< class T >
  List< T >::~List()
  {
    clear();
    rmFake();
  }
  template< class T >
  List< T >::List(const List< T >& other)
  {
    fake = createFake();
    fake->next = nullptr;
    if (!other.fake->next)
    {
      return;
    }
    Node< T >* now_old = other.fake->next;
    Node< T >* now_new = new Node< T >{now_old->value, nullptr};
    fake->next = now_new;
    now_old = now_old->next;
    while(now_old != nullptr)
    {
        now_new->next = new Node< T >{now_old->value, nullptr};
        now_new = now_new->next;
        now_old = now_old->next;
    }
  }
  template< class T >
  List< T >::List(List< T >&& other)
  {
    fake = createFake();
    fake->next = other.fake->next;
    other.fake->next = nullptr;
  }
  template< class T >
  List< T >& List< T >::operator=(const List< T >& other)
  {
    if (this != &other)
    {
      clear();
      fake->next = nullptr;
      if (other.fake->next)
      {
        Node< T >* now_old = other.fake->next;
        Node< T >* now_new = new Node< T > {now_old->value, nullptr};
        fake->next = now_new;
        now_old = now_old->next;
        while(now_old != nullptr)
        {
          now_new->next = new Node< T > {now_old->value, nullptr};
          now_new = now_new->next;
          now_old = now_old->next;
        }
      }
    }
    return *this;
  }
  template< class T >
  List< T >& List< T >::operator=(List< T >&& other)
  {
    if (this != & other)
    {
      clear();
      fake->next = other.fake->next;
      other.fake->next = nullptr;
    }
    return * this;
  }
  template< class T >
  LIter< T > List< T >::begin() noexcept
  {
    return LIter< T >(fake->next);
  }
  template< class T >
  LCIter< T > List< T >::begin() const noexcept
  {
    return LCIter< T >(fake->next);
  }
  template< class T >
  LIter< T > List< T >::end() noexcept
  {
    return LIter< T > (nullptr);
  }
  template< class T >
  LCIter< T > List< T >::end() const noexcept
  {
    return LCIter< T > (nullptr);
  }
  template< class T >
  LIter< T > List< T >::getLast() noexcept
  {
    LIter< T > now = this->begin();
    while(now.hasNext() && now.next() != nullptr)
    {
      now = now.next();
    }
    return now;
  }
  template< class T >
  LCIter< T > List< T >::getLast() const noexcept
  {
    LCIter< T > now = this->begin();
    while(now.hasNext() && now.next() != nullptr)
    {
      now = now.next();
    }
    return now;
  }
  template< class T >
  LIter< T > List< T >::push_start(const T& a)
  {
    Node< T >* new_el = new Node< T > {a, fake->next};
    fake->next = new_el;
    return LIter< T >(new_el);
  }
  template< class T >
  void List< T >::pop_start() noexcept
  {
    if (begin() != LIter< int > (nullptr))
    {
      Node< T >* n = fake->next->next;
      delete fake->next;
      fake->next = n;
    }
    else
    {
      return;
    }
  }
  template< class T >
  void List< T >::pop_end() noexcept
  {
    Node< T >* now = fake;
    if (now->next == nullptr)
    {
      return;
    }
    while (now->next->next != nullptr)
    {
      now = now->next;
    }
    delete now->next;
    now->next = nullptr;
  }
  template< class T >
  LIter< T > List< T >::insert(LIter< T > i, const T& a)
  {
    if (i.ptr == nullptr)
    {
      i = push_start(a);
      return LIter< T >(i);
    }
    else
    {
      Node< T >* new_el = new Node< T > {a, i.next().ptr};
      i.ptr->next = new_el;
      i = i.ptr->next;
      return LIter< T >(i);
    }
  }
  template< class T >
  LIter< T > List< T >::insert(LIter< T > i, const T&& a)
  {
    if (i.ptr == nullptr)
    {
      i = push_start(a);
      return LIter< T >(i);
    }
    else
    {
      Node< T >* new_el = new Node< T > {a, i.next().ptr};
      i.ptr->next = new_el;
      i = i.ptr->next;
      return LIter< T >(i);
    }
  }
  template< class T >
  void List< T >::clear() noexcept
  {
    while (fake->next != nullptr)
    {
      Node< T >* temp = fake->next;
      fake->next = temp->next;
      delete temp;
    }
  }
}
#endif
