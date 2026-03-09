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
    Node< T >* fake;
    Node< T >* mkFake();
    void delFake() noexcept;

    public:
      List();
      List(const List< T >& l);
      List(List< T >&& l);
      List< T >& operator=(const List< T >& l);
      List< T >& operator=(List< T >&& l);
      ~List();
      void clear() noexcept;

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
      return nd->next;
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
    return !(this == it);
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
      return nd->next;
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
    return !(this == it);
  }

  template< class T >
  Node< T >* List< T >::mkFake()
  {
    Node< T >* f = new Node< T >;
    return f;
  }

  template< class T >
  void List< T >::delFake() noexcept
  {
    delete fake;
  }

  template< class T >
  List< T >::List():
    fake{mkFake()}
  {}

  template< class T >
  List< T >::List(const List< T >& l):
    fake{mkFake()}
  {
    Node< T >* nl = l.fake->next;
    Node< T >* n = fake;
    while(nl)
    {
      n->next = new Node< T >{nl->val, nullptr};
      n = n->next;
      nl = nl->next;
    }
  }

  template< class T >
  List< T >::List(List< T >&& l):
    fake{l.fake}
  {
    fake->next = l.fake->next;
    l.fake->next = nullptr;
  }

  template< class T >
  List< T >& List< T >::operator=(const List< T >& l)
  {
    if (this != &l)
    {
      clear();
      fake->next = nullptr;
      if (l.fake->next)
      {
        Node< T >* nl = l.fake->next;
        Node< T >* n = fake;
        while(nl)
        {
          n->next = new Node< T >{nl->val, nullptr};
          n = n->next;
          nl = nl->next;
        }
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
      delFake();
      fake = l.fake->next;
      l.fake->next = nullptr;
    }
    return *this;
  }

  template< class T >
  List< T >::~List()
  {
    clear();
    delFake();
  }

  template< class T >
  void List< T >::clear() noexcept
  {
    while(fake->next)
    {
      Node< T >* tmp = fake->next;
      fake->next = tmp->next;
      delete tmp;
    }
  }

  template< class T >
  LIter< T > List< T >::begin() noexcept
  {
    return fake->next;
  }

  template< class T >
  LCIter< T > List< T >::begin() const noexcept
  {
    return fake->next;
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
    if (fake->next == nullptr)
    {
      return end();
    }
    LIter< T > it = begin();
    while (it.next().nd != nullptr)
    {
      it = it.next();
    }
    return it;
  }

  template< class T >
  LCIter< T > List< T >::getLast() const noexcept
  {
    if (fake->next == nullptr)
    {
      return end();
    }
    LCIter< T > it = begin();
    while (it.next().nd != nullptr)
    {
      it = it.next();
    }
    return it;
  }

  template< class T >
  LIter< T > List< T >::addStart(const T& a)
  {
    Node< T >* n = new Node< T >{a, fake->next};
    fake->next = n;
    return n;
  }

  template< class T >
  void List< T >::popStart() noexcept
  {
    if (fake->next)
    {
      Node< T >* tmp = fake->next->next;
      delete fake->next;
      fake->next = tmp;
    }
  }

  template< class T >
  LIter< T > List< T >::insert(LIter< T > id, const T& a)
  {
    if(id.hasNext())
    {
      Node< T >* n = new Node< T > {a, id.next().nd};
      id.nd->next = n;
      id = id.nd->next;
      return LIter< T >(id);
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
      Node< T >* n = new Node< T > {a, id.next().nd};
      id.nd->next = n;
      id = id.nd->next;
      return LIter< T >(id);
    }
    else
    {
      return addStart(a);
    }
  }
}
#endif
