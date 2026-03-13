#ifndef LIST_HPP
#define LIST_HPP

#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>

#include "LCIter.hpp"
#include "LIter.hpp"

namespace khasnulin
{

  template < class T > class BiList
  {
  public:
    BiList() noexcept;

    BiList(const BiList< T > &list);
    BiList(BiList< T > &&list) noexcept;

    ~BiList() noexcept;

    BiList< T > &operator=(const BiList< T > &list);
    BiList< T > &operator=(BiList< T > &&list) noexcept;
    LIter< T > begin() noexcept;
    LCIter< T > begin() const noexcept;
    LCIter< T > cbegin() const noexcept;

    LIter< T > end() noexcept;
    LCIter< T > end() const noexcept;
    LCIter< T > cend() noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;

    LIter< T > insert(LCIter< T > pos, const T &val);
    LIter< T > insert(LCIter< T > pos, T &&val);

    void push_back(const T &val);
    void push_back(T &&val);

    void clear() noexcept;
    LIter< T > erase(LIter< T > pos);
    LIter< T > erase(LIter< T > begin, LIter< T > end);

    T &front();
    const T &front() const;
    T &back();
    const T &back() const;

    // TODO: to the future version
    template < class... Args > LIter< T > emplace(LCIter< T > pos, Args &&...args);

    friend class LIter< T >;
    friend class LCIter< T >;

  private:
    template < class K > struct LNode
    {
      K val;
      LNode< K > *prev;
      LNode< K > *next;
    };
    LNode< T > *h_;
    size_t s_;

    static LNode< T > *copy(const LNode< T > *h);
    static void clear(LNode< T > *h) noexcept;
    template < class... Args > static LNode< T > *createNew(Args &&...args);
    template < class... Args > static LNode< T > *insert_before(LNode< T > *currNode, Args &&...args);
  };

  template < typename T > std::ostream &operator<<(std::ostream &os, const LIter< T > &it)
  {
    return os << "LIter";
  }

  template < class T >
  BiList< T >::BiList() noexcept:
      h_(nullptr),
      s_(0)
  {
  }

  template < class T > void BiList< T >::clear(LNode< T > *h) noexcept
  {
    if (h)
    {
      LNode< T > *item = h;
      h->prev->next = nullptr;
      while (item != nullptr)
      {
        LNode< T > *n = item->next;
        delete item;
        item = n;
      }
    }
  }

  template < class T > typename BiList< T >::template LNode< T > *BiList< T >::copy(const LNode< T > *h)
  {
    LNode< T > *new_h = nullptr;
    if (h)
    {
      new_h = createNew(h->val);
      LNode< T > *tail = new_h;
      LNode< T > *Ltail = h->next;
      try
      {
        while (Ltail != h)
        {
          tail->next = new LNode< T >{Ltail->val, tail, new_h};
          tail = tail->next;
          Ltail = Ltail->next;
        }
        new_h->prev = tail;
      }
      catch (...)
      {
        clear(new_h);
        throw;
      }
    }
    return new_h;
  }

  template < class T >
  template < class... Args >
  typename BiList< T >::template LNode< T > *BiList< T >::createNew(Args &&...args)
  {
    LNode< T > *head = new LNode< T >{T(std::forward< Args >(args)...), nullptr, nullptr};
    head->next = head;
    head->prev = head;
    return head;
  }

  template < class T >
  template < class... Args >
  typename BiList< T >::template LNode< T > *BiList< T >::insert_before(LNode< T > *currNode, Args &&...args)
  {
    if (!currNode)
    {
      return createNew(std::forward< Args >(args)...);
    }
    LNode< T > *prev = currNode->prev;
    LNode< T > *curr = currNode;
    prev->next = new LNode< T >{T(std::forward< Args >(args)...), prev, curr};
    curr->prev = prev->next;
    return curr->prev;
  }

  template < class T >
  BiList< T >::BiList(const BiList< T > &list):
      h_(nullptr),
      s_(0)
  {
    h_ = copy(list.h_);
    s_ = list.s_;
  }

  template < class T >
  BiList< T >::BiList(BiList< T > &&list) noexcept:
      h_(list.h_),
      s_(list.s_)
  {
    list.h_ = nullptr;
    list.s_ = 0;
  }

  template < class T > BiList< T >::~BiList() noexcept
  {
    clear(h_);
  }

  template < class T > BiList< T > &BiList< T >::operator=(const BiList< T > &list)
  {
    if (this == std::addressof(list))
    {
      return *this;
    }

    LNode< T > *new_h = copy(list.h_);
    clear(h_);
    h_ = new_h;
    s_ = list.s_;

    return *this;
  }

  template < class T > BiList< T > &BiList< T >::operator=(BiList< T > &&list) noexcept
  {
    if (this == std::addressof(list))
    {
      return *this;
    }
    std::swap(h_, list.h_);
    std::swap(s_, list.s_);
    return *this;
  }

  template < class T > LIter< T > BiList< T >::begin() noexcept
  {
    return LIter< T >(this, h_, h_ == nullptr);
  }
  template < class T > LCIter< T > BiList< T >::begin() const noexcept
  {
    return LCIter< T >(this, h_, h_ == nullptr);
  }
  template < class T > LCIter< T > BiList< T >::cbegin() const noexcept
  {
    return LCIter< T >(this, h_, h_ == nullptr);
  }

  template < class T > LIter< T > BiList< T >::end() noexcept
  {
    return LIter< T >(this, h_ ? h_->prev : nullptr, true);
  }
  template < class T > LCIter< T > BiList< T >::end() const noexcept
  {
    return LCIter< T >(this, h_ ? h_->prev : nullptr, true);
  }
  template < class T > LCIter< T > BiList< T >::cend() noexcept
  {
    return LCIter< T >(this, h_ ? h_->prev : nullptr, true);
  }

  template < class T > bool BiList< T >::empty() const noexcept
  {
    return s_ == 0;
  }
  template < class T > size_t BiList< T >::size() const noexcept
  {
    return s_;
  }

  template < class T > LIter< T > BiList< T >::insert(LCIter< T > pos, const T &val)
  {
    LNode< T > *item = insert_before(pos.it_.curr_, val);
    if (pos.it_.curr_ == h_)
    {
      h_ = item;
    }
    s_++;
    return LIter< T >(this, item, false);
  }

  template < class T > LIter< T > BiList< T >::insert(LCIter< T > pos, T &&val)
  {
    LNode< T > *item = insert_before(pos.it_.curr_, std::forward< T >(val));
    if (pos.it_.curr_ == h_)
    {
      h_ = item;
    }
    s_++;
    return LIter< T >(this, item, false);
  }

  template < class T > void BiList< T >::push_back(const T &val)
  {
    if (h_)
    {
      insert_before(h_, val);
    }
    else
    {
      h_ = createNew(val);
    }
    s_++;
  }

  template < class T > void BiList< T >::push_back(T &&val)
  {
    if (h_)
    {
      insert_before(h_, std::forward< T >(val));
    }
    else
    {
      h_ = createNew(std::forward< T >(val));
    }
    s_++;
  }

  template < class T > void BiList< T >::clear() noexcept
  {
    clear(h_);
    h_ = nullptr;
    s_ = 0;
  }

  template < class T > LIter< T > BiList< T >::erase(LIter< T > pos)
  {
    if (!pos.curr_ || pos.is_end_)
    {
      throw std::runtime_error("can't erase iterator to the end");
    }
    LNode< T > *curr = pos.curr_;
    if (s_ == 1)
    {
      delete curr;
      h_ = nullptr;
      s_ = 0;
      return end();
    }
    LNode< T > *next = curr->next;
    LNode< T > *prev = curr->prev;
    next->prev = prev;
    prev->next = next;
    bool at_start = false;
    if (curr == h_)
    {
      h_ = next;
      at_start = true;
    }
    delete curr;
    --s_;
    if (at_start)
    {
      return LIter< T >(this, next, false);
    }
    if (next == h_)
    {
      return LIter< T >(this, next->prev, true);
    }
    return LIter< T >(this, next, false);
  }

  template < class T > LIter< T > BiList< T >::erase(LIter< T > begin, LIter< T > end)
  {
    if (!begin.curr_ || begin.is_end_)
    {
      throw std::runtime_error("can't erase iterator to the end");
    }

    while (!begin.is_end_ && begin != end)
    {
      begin = erase(begin);
    }
    return begin;
  }

  template < class T > T &BiList< T >::front()
  {
    if (empty())
    {
      throw std::runtime_error("list is empty, can't get front element");
    }
    return h_->val;
  }
  template < class T > const T &BiList< T >::front() const
  {
    if (empty())
    {
      throw std::runtime_error("list is empty, can't get front element");
    }
    return h_->val;
  }

  template < class T > T &BiList< T >::back()
  {
    if (empty())
    {
      throw std::runtime_error("list is empty, can't get back element");
    }
    return h_->prev->val;
  }

  template < class T > const T &BiList< T >::back() const
  {
    if (empty())
    {
      throw std::runtime_error("list is empty, can't get back element");
    }
    return h_->prev->val;
  }
}

#endif
