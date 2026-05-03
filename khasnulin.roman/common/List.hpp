#ifndef LIST_HPP
#define LIST_HPP

#include <initializer_list>

#include "LCIter.hpp"
#include "LIter.hpp"
#include "RLCIter.hpp"
#include "RLIter.hpp"

namespace khasnulin
{

  template < class T > class BiList
  {
  public:
    BiList() noexcept;

    BiList(const BiList< T > &list);
    BiList(BiList< T > &&list) noexcept;

    BiList(std::initializer_list< T > init);

    ~BiList() noexcept;

    BiList< T > &operator=(const BiList< T > &list);
    BiList< T > &operator=(BiList< T > &&list) noexcept;

    BiList< T > &operator=(std::initializer_list< T > ilist);

    template < class K > friend bool operator==(const BiList< K > &lhs, const BiList< K > &rhs);
    template < class K > friend bool operator!=(const BiList< K > &lhs, const BiList< K > &rhs);

    LIter< T > begin() noexcept;
    LCIter< T > begin() const noexcept;
    LCIter< T > cbegin() const noexcept;

    LIter< T > end() noexcept;
    LCIter< T > end() const noexcept;
    LCIter< T > cend() const noexcept;

    RLIter< T > rbegin() noexcept;
    RLCIter< T > rbegin() const noexcept;
    RLCIter< T > crbegin() const noexcept;

    RLIter< T > rend() noexcept;
    RLCIter< T > rend() const noexcept;
    RLCIter< T > crend() const noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

    LIter< T > insert(LCIter< T > pos, const T &val);
    LIter< T > insert(LCIter< T > pos, T &&val);

    LIter< T > insert(LCIter< T > pos, LIter< T > begin, LIter< T > end);

    void push_back(const T &val);
    void push_back(T &&val);

    void push_front(const T &val);
    void push_front(T &&val);

    void clear() noexcept;
    LIter< T > erase(LIter< T > pos);
    LIter< T > erase(LIter< T > begin, LIter< T > end);

    void pop_front();
    void pop_back();

    T &front();
    const T &front() const;
    T &back();
    const T &back() const;

    void swap(BiList< T > &other) noexcept;

    template < class... Args > LIter< T > emplace(LCIter< T > pos, Args &&...args);
    template < class... Args > void emplace_back(Args &&...args);
    template < class... Args > void emplace_front(Args &&...args);

    void splice(LCIter< T > pos, BiList< T > &other);
    void splice(LCIter< T > pos, BiList< T > &&other);

    void splice(LCIter< T > pos, BiList< T > &other, LCIter< T > it);

    void splice(LCIter< T > pos, BiList< T > &other, LCIter< T > first, LCIter< T > last);

    friend class LIter< T >;
    friend class LCIter< T >;
    friend class RLIter< T >;
    friend class RLCIter< T >;

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
    LNode< T > *erase(LNode< T > *node);
    static LNode< T > *sliceNodes(LNode< T > *pos, LNode< T > *newNodes);

    void insertNodesBefore(LCIter< T > pos, LNode< T > *newNodes);

    void insertInterval(LCIter< T > pos, BiList< T > &other, LCIter< T > first, LCIter< T > last);

    template < class... Args > static LNode< T > *createNew(Args &&...args);
    template < class... Args > static LNode< T > *insert_before(LNode< T > *currNode, Args &&...args);
  };

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
    LNode< T > *node = new LNode< T >{T(std::forward< Args >(args)...), prev, currNode};
    prev->next = node;
    currNode->prev = node;
    return node;
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
    BiList< T > cpy(std::move(list));
    swap(cpy);
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
  template < class T > LCIter< T > BiList< T >::cend() const noexcept
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
    LNode< T > *node = insert_before(h_, val);
    if (!h_)
    {
      h_ = node;
    }
    s_++;
  }

  template < class T > void BiList< T >::push_back(T &&val)
  {
    LNode< T > *node = insert_before(h_, std::forward< T >(val));
    if (!h_)
    {
      h_ = node;
    }
    s_++;
  }

  template < class T > void BiList< T >::clear() noexcept
  {
    clear(h_);
    h_ = nullptr;
    s_ = 0;
  }

  template < class T > typename BiList< T >::template LNode< T > *BiList< T >::erase(LNode< T > *node)
  {
    LNode< T > *next = node->next;
    LNode< T > *prev = node->prev;

    if (node == h_)
    {
      h_ = h_->next;
    }

    delete node;
    --s_;

    if (s_ == 0)
    {
      h_ = nullptr;
      return nullptr;
    }

    next->prev = prev;
    prev->next = next;
    return next;
  }

  template < class T > LIter< T > BiList< T >::erase(LIter< T > pos)
  {
    if (!pos.curr_ || pos.is_end_)
    {
      throw std::runtime_error("can't erase iterator to the end");
    }
    LNode< T > *node = pos.curr_;
    bool is_head = node == h_;

    LNode< T > *next = erase(node);

    if (!next)
    {
      return LIter< T >{this, nullptr, true};
    }

    bool is_tail = (next == h_) && !is_head;
    return LIter< T >{this, is_tail ? next->prev : next, is_tail};
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

  template < class T > RLIter< T > BiList< T >::rbegin() noexcept
  {
    return RLIter< T >(this, h_ ? h_->prev : nullptr, h_ == nullptr);
  }

  template < class T > RLCIter< T > BiList< T >::rbegin() const noexcept
  {
    return RLCIter< T >(this, h_ ? h_->prev : nullptr, h_ == nullptr);
  }

  template < class T > RLCIter< T > BiList< T >::crbegin() const noexcept
  {
    return RLCIter< T >(this, h_ ? h_->prev : nullptr, h_ == nullptr);
  }

  template < class T > RLIter< T > BiList< T >::rend() noexcept
  {
    return RLIter< T >(this, h_, true);
  }

  template < class T > RLCIter< T > BiList< T >::rend() const noexcept
  {
    return RLCIter< T >(this, h_, true);
  }

  template < class T > RLCIter< T > BiList< T >::crend() const noexcept
  {
    return RLCIter< T >(this, h_, true);
  }

  template < class T > void BiList< T >::push_front(const T &val)
  {
    h_ = insert_before(h_, val);
    s_++;
  }
  template < class T > void BiList< T >::push_front(T &&val)
  {
    h_ = insert_before(h_, std::forward< T >(val));
    s_++;
  }

  template < class T > void BiList< T >::pop_back()
  {
    if (!h_)
    {
      throw std::runtime_error("can't pop back, list is empty");
    }
    erase(h_->prev);
  }

  template < class T > void BiList< T >::pop_front()
  {
    if (!h_)
    {
      throw std::runtime_error("can't pop back, list is empty");
    }
    erase(h_);
  }

  template < class T > void BiList< T >::swap(BiList< T > &other) noexcept
  {
    std::swap(h_, other.h_);
    std::swap(s_, other.s_);
  }

  template < class T >
  template < class... Args >
  LIter< T > BiList< T >::emplace(LCIter< T > pos, Args &&...args)
  {
    LNode< T > *node = insert_before(pos.it_.curr_, std::forward< Args >(args)...);
    if (!h_)
    {
      h_ = node;
    }
    s_++;
    return LIter< T >{this, node, false};
  }

  template < class T > template < class... Args > void BiList< T >::emplace_back(Args &&...args)
  {
    LNode< T > *node = insert_before(h_, std::forward< Args >(args)...);
    if (!h_)
    {
      h_ = node;
    }
    s_++;
  }

  template < class T > template < class... Args > void BiList< T >::emplace_front(Args &&...args)
  {
    h_ = insert_before(h_, std::forward< Args >(args)...);
    s_++;
  }

  template < class T >
  BiList< T >::BiList(std::initializer_list< T > init):
      BiList()
  {
    try
    {
      for (const T &item : init)
      {
        push_back(item);
      }
    }
    catch (...)
    {
      clear();
      throw;
    }
  }
  template < class T > BiList< T > &BiList< T >::operator=(std::initializer_list< T > ilist)
  {
    auto listIt = ilist.begin();
    auto listEnd = ilist.end();

    BiList< T > new_list(*this);
    auto it = new_list.begin();
    auto lend = new_list.end();
    try
    {
      for (; listIt != listEnd && it != lend; ++it, ++listIt)
      {
        *it = *listIt; // T::operator=();
      }

      while (listIt != listEnd)
      {
        new_list.push_back(*listIt);
        listIt++;
      }
    }
    catch (...)
    {
      new_list.clear();
      throw;
    }
    swap(new_list);
    return *this;
  }

  template < class T > LIter< T > BiList< T >::insert(LCIter< T > pos, LIter< T > begin, LIter< T > end)
  {
    LNode< T > *new_list = nullptr;
    size_t new_s = 0;
    try
    {
      for (; begin != end; ++begin)
      {
        if (!new_list)
        {
          new_list = insert_before(new_list, *begin);
        }
        else
        {
          insert_before(new_list, *begin);
        }
        new_s++;
      }
    }
    catch (...)
    {
      clear(new_list);
      throw;
    }

    insertNodesBefore(pos, new_list);
    s_ += new_s;
    return LIter< T >{this, new_list, new_list == nullptr};
  }

  template < class T > bool operator==(const BiList< T > &lhs, const BiList< T > &rhs)
  {
    if (lhs.size() != rhs.size())
    {
      return false;
    }
    auto it1 = lhs.cbegin();
    auto it2 = rhs.cbegin();
    for (; it1 != lhs.cend() && it2 != rhs.cend(); ++it1, ++it2)
    {
      if ((*it1) != (*it2))
      {
        return false;
      }
    }
    return true;
  }

  template < class T > bool operator!=(const BiList< T > &lhs, const BiList< T > &rhs)
  {
    return !(lhs == rhs);
  }

  template < class T > std::ostream &operator<<(std::ostream &out, BiList< T > list)
  {
    return out << list.size() << "\n";
  }

  template < class T >
  typename BiList< T >::template LNode< T > *BiList< T >::sliceNodes(LNode< T > *pos, LNode< T > *newNodes)
  {
    LNode< T > *end = pos;
    LNode< T > *start = end->prev;
    LNode< T > *new_last = newNodes->prev;
    start->next = newNodes;
    end->prev = new_last;
    newNodes->prev = start;
    new_last->next = end;
    return newNodes;
  }

  template < class T > void BiList< T >::insertNodesBefore(LCIter< T > pos, LNode< T > *newNodes)
  {
    if (!h_)
    {
      h_ = newNodes;
    }
    else
    {
      LNode< T > *currPos = pos.it_.is_end_ ? h_ : pos.it_.curr_;
      sliceNodes(currPos, newNodes);
      if (pos == begin())
      {
        h_ = newNodes;
      }
    }
  }

  template < class T > void BiList< T >::splice(LCIter< T > pos, BiList< T > &other)
  {
    if (this != std::addressof(other) && other.h_)
    {
      size_t new_s = other.size();
      insertNodesBefore(pos, other.h_);
      other.h_ = nullptr;
      other.s_ = 0;
      s_ += new_s;
    }
  }

  template < class T > void BiList< T >::splice(LCIter< T > pos, BiList< T > &&other)
  {
    splice(pos, other);
  }

  template < class T > void BiList< T >::splice(LCIter< T > pos, BiList< T > &other, LCIter< T > it)
  {
    if (other.h_ && !it.it_.is_end_ && pos != it)
    {
      auto next_it = it;
      ++next_it;
      insertInterval(pos, other, it, next_it);
      if (std::addressof(other) != this)
      {
        s_++;
        other.s_--;
      }
    }
  }

  template < class T >
  void BiList< T >::insertInterval(LCIter< T > pos, BiList< T > &other, LCIter< T > first, LCIter< T > last)
  {
    LNode< T > *start = first.it_.curr_;
    LNode< T > *end = (--last).it_.curr_;

    LNode< T > *prev_elem = start->prev;
    LNode< T > *next_elem = end->next;

    if (other.h_ == start)
    {
      if (other.h_ == next_elem)
      {
        other.h_ = nullptr;
      }
      else
      {
        other.h_ = next_elem;
      }
    }

    prev_elem->next = next_elem;
    next_elem->prev = prev_elem;
    start->prev = end;
    end->next = start;
    insertNodesBefore(pos, start);
  }

  template < class T >
  void BiList< T >::splice(LCIter< T > pos, BiList< T > &other, LCIter< T > first, LCIter< T > last)
  {
    if (other.h_ && !first.it_.is_end_ && first != last)
    {
      size_t new_s = 0;
      for (auto it = first; it != last; ++it)
      {
        new_s++;
      }
      insertInterval(pos, other, first, last);
      if (std::addressof(other) != this)
      {
        s_ += new_s;
        other.s_ -= new_s;
      }
    }
  }
}
#endif
