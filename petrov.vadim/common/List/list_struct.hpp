#ifndef LIST_STRUCT_HPP
#define LIST_STRUCT_HPP
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <memory>

namespace petrov
{
  namespace detail
  {
    template< class T >
    struct Node
    {
      T val;
      Node< T >* next;
    };
  }
  using namespace detail;
  template< class T >
  class List;

  template< class T >
  class LIter
  {
    public:
      LIter() noexcept;
      T* operator->();
      T& operator*();
      bool operator==(const LIter< T >& it) const noexcept;
      bool operator!=(const LIter< T >& it) const noexcept;

      LIter< T >& operator++();
      LIter< T > operator++(int);

    private:
      LIter(Node< T >* p) noexcept;
      friend class List< T >;
      Node< T >* nd_;
  };

  template< class T >
  class LCIter
  {
    public:
      LCIter() noexcept;
      const T* operator->() const;
      const T& operator*() const;
      bool operator==(const LCIter< T >& it) const noexcept;
      bool operator!=(const LCIter< T >& it) const noexcept;

      LCIter< T >& operator++();
      LCIter< T > operator++(int);

    private:
      LCIter(const Node< T >* p) noexcept;
      friend class List< T >;
      const Node< T >* nd_;
  };

  template< class T >
  class List
  {
    public:
      List() noexcept;
      List(const List< T >& l);
      List(List< T >&& l) noexcept;
      List< T >& operator=(const List< T >& l);
      List< T >& operator=(List< T >&& l) noexcept;
      ~List() noexcept;
      void clear() noexcept;
      size_t size() const noexcept;

      LIter< T > begin() noexcept;
      LCIter< T > begin() const noexcept;
      LIter< T > end() noexcept;
      LCIter< T > end() const noexcept;

      LIter< T > back() noexcept;
      LCIter< T > back() const noexcept;

      LIter< T > push_front(const T& a);
      LIter< T > push_front(const T&& a);
      void pop_front() noexcept;

      LIter< T > insert_after(LIter< T > id, const T& a);
      LIter< T > insert_after(LIter< T > id, T&& a);

      void swap(List< T >& l) noexcept;

    private:
      Node< T >* head;
      Node< T >* tail;
      size_t size_;
  };
}

namespace petrov
{
  using namespace detail;

  template< class T >
  LIter< T >::LIter() noexcept:
    nd_{nullptr}
  {}

  template< class T >
  LIter< T >::LIter(Node< T >* n) noexcept:
    nd_{n}
  {}

  template< class T >
  T* LIter< T >::operator->()
  {
    if (nd_)
    {
      return std::addressof(nd_->val);
    }
    else
    {
      throw std::runtime_error("Go to nullptr");
    }
  }

  template< class T >
  T& LIter< T >::operator*()
  {
    if (nd_)
    {
      return nd_->val;
    }
    else
    {
      throw std::runtime_error("Go to nullptr");
    }
  }

  template< class T >
  bool LIter< T >::operator==(const LIter< T >& it) const noexcept
  {
    return nd_ == it.nd_;
  }

  template< class T >
  bool LIter< T >::operator!=(const LIter< T >& it) const noexcept
  {
    return nd_ != it.nd_;
  }

  template< class T >
  LIter< T >& LIter< T >::operator++()
  {
    if (!nd_)
    {
      throw std::runtime_error("Go to nullptr");
    }
    nd_ = nd_->next;
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator++(int)
  {
    LIter< T > tmp = *this;
    ++(*this);
    return tmp;
  }

  template< class T >
  LCIter< T >::LCIter() noexcept:
    nd_{nullptr}
  {}

  template< class T >
  LCIter< T >::LCIter(const Node< T >* n) noexcept:
    nd_{n}
  {}

  template< class T >
  const T* LCIter< T >::operator->() const
  {
    if (!nd_)
    {
      throw std::runtime_error("Go to nullptr");
    }
    return &(nd_->val);
  }

  template< class T >
  const T& LCIter< T >::operator*() const
  {
    if (nd_)
    {
      return nd_->val;
    }
    else
    {
      throw std::runtime_error("Go to nullptr");
    }
  }

  template< class T >
  bool LCIter< T >::operator==(const LCIter< T >& it) const noexcept
  {
    return nd_ == it.nd_;
  }

  template< class T >
  bool LCIter< T >::operator!=(const LCIter< T >& it) const noexcept
  {
    return nd_ != it.nd_;
  }

  template< class T >
  LCIter< T >& LCIter< T >::operator++()
  {
    if (!nd_)
    {
      throw std::runtime_error("Go to nullptr");
    }
    nd_ = nd_->next;
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator++(int)
  {
    LCIter< T > tmp = *this;
    ++(*this);
    return tmp;
  }

  template< class T >
  List< T >::List() noexcept:
    head{nullptr},
    tail{nullptr},
    size_{0}
  {}

  template< class T >
  List< T >::List(const List< T >& l):
    head{nullptr},
    tail{nullptr},
    size_{0}
  {
    try
    {
      LIter< T > pos;
      for (Node< T >* i = l.head; i; i = i->next)
      {
        pos = insert_after(pos, i->val);
      }
    }
    catch(...)
    {
      clear();
      throw;
    }

  }

  template< class T >
  List< T >::List(List< T >&& l) noexcept:
    head{std::exchange(l.head, nullptr)},
    tail{std::exchange(l.tail, nullptr)},
    size_{std::exchange(l.size_, 0)}
  {}

  template< class T >
  List< T >& List< T >::operator=(const List< T >& l)
  {
    if (this != std::addressof(l))
    {
      List<T> tmp(l);
      swap(tmp);
    }
    return *this;
  }

  template< class T >
  List< T >& List< T >::operator=(List< T >&& l) noexcept
  {
    List<T> temp(std::move(l));
    swap(temp);
    return *this;
  }

  template< class T >
  List< T >::~List() noexcept
  {
    clear();
  }

  template< class T >
  void List< T >::clear() noexcept
  {
    while (head)
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
    return LCIter< T >(nullptr);
  }

  template< class T >
  LIter< T > List< T >::back() noexcept
  {
    return LIter< T >(tail);
  }

  template< class T >
  LCIter< T > List< T >::back() const noexcept
  {
    return LCIter< T >(tail);
  }

  template< class T >
  LIter< T > List< T >::push_front(const T& a)
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
  LIter< T > List< T >::push_front(const T&& a)
  {
    Node< T >* n = new Node< T >{std::move(a), head};
    head = n;
    if (tail == nullptr)
    {
      tail = n;
    }
    size_++;
    return LIter< T >(n);
  }

  template< class T >
  void List< T >::pop_front() noexcept
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
  LIter< T > List< T >::insert_after(LIter< T > id, const T& a)
  {
    if (id.nd_)
    {
      Node< T >* n = new Node< T >{a, id.nd_->next};
      id.nd_->next = n;
      if (id.nd_ == tail)
      {
        tail = n;
      }
      size_++;
      return LIter< T >(n);
    }
    return push_front(a);
  }

  template< class T >
  LIter< T > List< T >::insert_after(LIter< T > id, T&& a)
  {
    if (!id.nd_)
    {
      return push_front(std::forward< T >(a));
    }
    Node< T >* n = new Node< T >{std::forward< T >(a), id.nd_->next};
    id.nd_->next = n;
    if (id.nd_ == tail)
    {
      tail = n;
    }
    ++size_;
    return LIter< T >(n);
  }

  template< class T >
  void List< T >::swap(List< T >& l) noexcept
  {
    std::swap(head, l.head);
    std::swap(tail, l.tail);
    std::swap(size_, l.size_);
  }
}

#endif
