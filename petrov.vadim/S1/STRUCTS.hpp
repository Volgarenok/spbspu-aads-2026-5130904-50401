#ifndef STRUCTS_HPP
#define STRUCTS_HPP
#include <cstddef>
#include <stdexcept>
namespace petrov
{
  template< class T >
  struct Node
  {
    T val;
    Node< T >* next;
  };

  template< class T >
  class List;

  template< class T >
  class LIter
  {
    friend class List< T >;
    Node< T >* nd;

    public:
      LIter() noexcept;
      LIter(Node< T >* p) noexcept;
      bool hasNext() const noexcept;
      LIter< T > next() const;
      T& operator*() const;
      bool operator==(const LIter< T >& it) const noexcept;
      bool operator!=(const LIter< T >& it) const noexcept;
  };

  template< class T >
  class LCIter
  {
    friend class List< T >;
    const Node< T >* nd;

    public:
      LCIter() noexcept;
      LCIter(const Node< T >* p) noexcept;
      bool hasNext() const noexcept;
      LCIter< T > next() const;
      const T& operator*() const;
      bool operator==(const LCIter< T >& it) const noexcept;
      bool operator!=(const LCIter< T >& it) const noexcept;
  };

  template< class T >
  class List
  {
    Node< T >* head;
    Node< T >* tail;
    size_t size_;

    public:
      List();
      List(const List< T >& l);
      List(List< T >&& l);
      List< T >& operator=(const List< T >& l);
      List< T >& operator=(List< T >&& l);
      ~List();
      void clear() noexcept;
      size_t size() const noexcept;

      LIter< T > begin() noexcept;
      LCIter< T > begin() const noexcept;
      LIter< T > end() noexcept;
      LCIter< T > end() const noexcept;

      LIter< T > getLast() noexcept;
      LCIter< T > getLast() const noexcept;

      LIter< T > addStart(const T& a);
      void popStart() noexcept;

      LIter< T > insert(LIter< T > id, const T& a);
      LIter< T > insert(LIter< T > id, const T&& a);
  };
}

namespace petrov
{
  template< class T >
  LIter< T >::LIter() noexcept:
    nd{nullptr}
  {}

  template< class T >
  LIter< T >::LIter(Node< T >* n) noexcept:
    nd{n}
  {}

  template< class T >
  bool LIter< T >::hasNext() const noexcept
  {
    return nd != nullptr;
  }

  template< class T >
  LIter< T > LIter< T >::next() const
  {
    if(nd)
    {
      return LIter< T >(nd->next);
    }
    else
    {
      throw std::runtime_error("Go to nullptr");
    }
  }

  template< class T >
  T& LIter< T >::operator*() const
  {
    if (nd)
    {
      return nd->val;
    }
    else
    {
      throw std::runtime_error("Go to nullptr");
    }
  }

  template< class T >
  bool LIter< T >::operator==(const LIter< T >& it) const noexcept
  {
    return nd == it.nd;
  }

  template< class T >
  bool LIter< T >::operator!=(const LIter< T >& it) const noexcept
  {
    return nd != it.nd;
  }

  template< class T >
  LCIter< T >::LCIter() noexcept:
    nd{nullptr}
  {}

  template< class T >
  LCIter< T >::LCIter(const Node< T >* n) noexcept:
    nd{n}
  {}

  template< class T >
  bool LCIter< T >::hasNext() const noexcept
  {
    return nd != nullptr;
  }

  template< class T >
  LCIter< T > LCIter< T >::next() const
  {
    if(nd)
    {
      return LCIter< T >(nd->next);
    }
    else
    {
      throw std::runtime_error("Go to nullptr");
    }
  }

  template< class T >
  const T& LCIter< T >::operator*() const
  {
    if(nd)
    {
      return nd->val;
    }
    else
    {
      throw std::runtime_error("Go to nullptr");
    }
  }

  template< class T >
  bool LCIter< T >::operator==(const LCIter< T >& it) const noexcept
  {
    return nd == it.nd;
  }

  template< class T >
  bool LCIter< T >::operator!=(const LCIter< T >& it) const noexcept
  {
    return nd != it.nd;
  }

  template< class T >
  List< T >::List():
    head{nullptr}, tail{nullptr}, size_{0}
  {}

  template< class T >
  List< T >::List(const List< T >& l):
    head{nullptr}, tail{nullptr}, size_{0}
  {
    if (l.head != nullptr)
    {
      head = new Node< T >{l.head->val, nullptr};
      Node< T >* n = head;
      size_ = 1;
      Node< T >* nl = l.head->next;
      while(nl)
      {
        n->next = new Node< T >{nl->val, nullptr};
        n = n->next;
        size_++;
        nl = nl->next;
      }
      tail = n;
    }
  }

  template< class T >
  List< T >::List(List< T >&& l):
    head{l.head}, tail{l.tail}, size_{l.size_}
  {
    l.head = nullptr;
    l.tail = nullptr;
    l.size_ = 0;
  }

  template< class T >
  List< T >& List< T >::operator=(const List< T >& l)
  {
    if (this != &l)
    {
      clear();
      Node< T >* nl = l.head;
      while(nl)
      {
        LIter< T > pos = getLast();
        insert(pos, nl->val);
        nl = nl->next;
      }
    }
    return *this;
  }

  template< class T >
  List< T >& List< T >::operator=(List< T >&& l)
  {
    if (this != &l)
    {
      clear();
      head = l.head;
      tail = l.tail;
      size_ = l.size_;
      l.head = nullptr;
      l.tail = nullptr;
      l.size_ = 0;
    }
    return *this;
  }

  template< class T >
  List< T >::~List()
  {
    clear();
  }

  template< class T >
  void List< T >::clear() noexcept
  {
    while(head)
    {
      Node< T >* tmp = head;
      head = head->next;
      delete tmp;
    }
    tail = nullptr;
    size_ = 0;
  }

  template< class T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }

  template< class T >
  LIter< T > List< T >::begin() noexcept
  {
    return LIter< T >(head);
  }

  template< class T >
  LCIter< T > List< T >::begin() const noexcept
  {
    return LCIter< T >(head);
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
    return LIter< T >(tail);
  }

  template< class T >
  LCIter< T > List< T >::getLast() const noexcept
  {
    return LCIter< T >(tail);
  }

  template< class T >
  LIter< T > List< T >::addStart(const T& a)
  {
    Node< T >* n = new Node< T >{a, head};
    head = n;
    if (tail == nullptr)
    {
      tail = n;
    }
    size_++;
    return LIter< T >(n);
  }

  template< class T >
  void List< T >::popStart() noexcept
  {
    if (head)
    {
      Node< T >* tmp = head;
      head = head->next;
      if (head == nullptr)
      {
        tail = nullptr;
      }
      delete tmp;
      size_--;
    }
  }

  template< class T >
  LIter< T > List< T >::insert(LIter< T > id, const T& a)
  {
    if(id.hasNext())
    {
      Node< T >* n = new Node< T >{a, id.nd->next};
      id.nd->next = n;
      if (id.nd == tail)
      {
        tail = n;
      }
      size_++;
      return LIter< T >(n);
    }
    else
    {
      return addStart(a);
    }
  }

  template< class T >
  LIter< T > List< T >::insert(LIter< T > id, const T&& a)
  {
    if(id.hasNext())
    {
      Node< T >* n = new Node< T >{a, id.nd->next};
      id.nd->next = n;
      if (id.nd == tail)
      {
        tail = n;
      }
      size_++;
      return LIter< T >(n);
    }
    else
    {
      return addStart(a);
    }
  }
}
#endif
