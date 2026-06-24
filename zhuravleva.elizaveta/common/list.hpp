#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include "node.hpp"
#include "iterator.hpp"

namespace zhuravleva
{

  template< class T >
  class List
  {
  public:
    List();
    List(const List& other);
    List(List&& other);
    ~List() noexcept;

    List& operator=(const List& other);
    List& operator=(List&& other) noexcept;

    LIter< T > begin() noexcept;
    LCIter< T > cbegin() const noexcept;

    LIter< T > end() noexcept;
    LCIter< T > cend() const noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

    LIter< T > insertAfter(LIter< T > pos, const T& value);
    LIter< T > pushBack(const T& value);
    LIter< T > pushFront(const T& value);
    LIter< T > beforeBegin();

    LIter< T > insertAfter(LIter< T > pos, T&& value);
    LIter< T > pushBack(T&& value);
    LIter< T > pushFront(T&& value);

    template< class... Args >
    LIter< T > emplaceAfter(LIter< T > pos, Args&&... args);

    template< class... Args >
    LIter< T > emplaceFront(Args&&... args);

    template< class... Args >
    LIter< T > emplaceBack(Args&&... args);

    void spliceAfter(LIter< T > pos, List& other, LIter< T > beforeElement) noexcept;
    void spliceAfter(LIter< T > pos, List& other) noexcept;
    void spliceAfter(LIter< T > pos, List& other,
        LIter< T > first, LIter< T > last) noexcept;

    template< class Compare >
    void merge(List& other, Compare comp) noexcept;

    template< class Compare >
    void sort(Compare comp);

    template< class Predicate >
    void partition(Predicate pred);

    void popFront() noexcept;
    void popBack() noexcept;
    void eraseAfter(LIter< T > pos);
     void erase(LIter< T > pos);
    void clear() noexcept;
    void swap(List& other) noexcept;

  private:
    detail::Node< T >* fake_;
    detail::Node< T >* createFake();
    detail::Node< T >* unlinkAfter(detail::Node< T >* pos) noexcept;
    void linkAfter(detail::Node< T >* pos, detail::Node< T >* node) noexcept;
    template< class Compare >
    detail::Node< T >* sortNodes(detail::Node< T >* head, Compare comp);

    template< class Compare >
    detail::Node< T >* mergeNodes(detail::Node< T >* left, detail::Node< T >* right,
        Compare comp);
  };

  template< class T >
  List< T >::List():
    fake_(createFake())
  {}

  template< class T >
  List< T >::~List() noexcept
  {
    if (fake_)
    {
      clear();
      delete fake_;
    }
  }

  template< class T >
  List< T >::List(const List& other):
    fake_(createFake())
  {
    try
    {
      for (LCIter< T > it = other.cbegin(); it != other.cend(); ++it)
      {
        pushBack(*it);
      }
    }
    catch (...)
    {
      clear();
      delete fake_;
      throw;
    }
  }

  template< class T >
  List< T >::List(List&& other):
    fake_(std::exchange(other.fake_, createFake()))
  {}

  template< class T >
  List< T >& List< T >::operator=(const List& other)
  {
    if (this != &other)
    {
      List< T > temp(other);
      swap(temp);
    }
    return *this;
  }

  template< class T >
  List< T >& List< T >::operator=(List&& other) noexcept
  {
    if (this != &other)
    {
      clear();
      if (!other.empty())
      {
        fake_->next = other.fake_->next;
        other.fake_->next = other.fake_;
        detail::Node< T > * cur = fake_->next;
        while (cur->next != other.fake_)
        {
          cur = cur->next;
        }
        cur->next = fake_;
      }
    }
    return *this;
  }

  template< class T >
  bool List< T >::empty() const noexcept
  {
    return fake_->next == fake_;
  }

  template< class T >
  size_t List< T >::size() const noexcept
  {
    size_t count = 0;
    detail::Node< T >* cur = fake_->next;
    while (cur != fake_)
    {
      ++count;
      cur = cur->next;
    }
    return count;
  }

  template< class T >
  detail::Node< T >* List< T >::createFake()
  {
    detail::Node< T >* node = new detail::Node< T >(T());
    node->next = node;
    return node;
  }

  template< class T >
  detail::Node< T >* List< T >::unlinkAfter(detail::Node< T >* pos) noexcept
  {
    detail::Node< T >* node = pos->next;
    pos->next = node->next;
    node->next = node;
    return node;
  }

  template< class T >
  void List< T >::linkAfter(detail::Node< T >* pos, detail::Node< T >* node) noexcept
  {
    node->next = pos->next;
    pos->next = node;
  }

  template< class T >
  LIter< T > List< T >::begin() noexcept
  {
    return LIter< T >(fake_->next);
  }

  template< class T >
  LCIter< T > List< T >::cbegin() const noexcept
  {
    return LCIter< T >(fake_->next);
  }

  template< class T >
  LIter< T > List< T >::end() noexcept
  {
    return LIter< T >(fake_);
  }

  template< class T >
  LCIter< T > List< T >::cend() const noexcept
  {
    return LCIter< T >(fake_);
  }

  template< class T >
  template< class Compare >
  void List< T >::merge(List& other, Compare comp) noexcept
  {
    if (this == &other)
    {
      return;
    }
    detail::Node< T >* thisPrev = fake_;
    detail::Node< T >* otherPrev = other.fake_;
    while ((thisPrev->next != fake_) && (otherPrev->next != other.fake_))
    {
      if (comp(otherPrev->next->data, thisPrev->next->data))
      {
        detail::Node< T >* node = other.unlinkAfter(otherPrev);
        linkAfter(thisPrev, node);
      }
      thisPrev = thisPrev->next;
    }
    if (otherPrev->next != other.fake_)
    {
      spliceAfter(LIter< T >(thisPrev), other);
    }
  }

  template< class T >
  template< class Compare >
  void List< T >::sort(Compare comp)
  {
    if ((fake_->next == fake_) || (fake_->next->next == fake_))
    {
      return;
    }
    detail::Node< T >* head = fake_->next;
    detail::Node< T >* tail = head;
    while (tail->next != fake_)
    {
      tail = tail->next;
    }
    tail->next = nullptr;
    head = sortNodes(head, comp);
    fake_->next = head;
    tail = head;
    while (tail->next)
    {
      tail = tail->next;
    }
    tail->next = fake_;
  }

  template< class T >
  template< class Compare >
  detail::Node< T >* List< T >::sortNodes(detail::Node< T >* head, Compare comp)
  {
    if (!head || !head->next)
    {
      return head;
    }
    detail::Node< T >* slow = head;
    detail::Node< T >* fast = head->next;
    while (fast && fast->next)
    {
      slow = slow->next;
      fast = fast->next->next;
    }
    detail::Node< T >* right = slow->next;
    slow->next = nullptr;
    detail::Node< T >* left = sortNodes(head, comp);
    right = sortNodes(right, comp);
    return mergeNodes(left, right, comp);
  }

  template< class T >
  template< class Compare >
  detail::Node< T >* List< T >::mergeNodes(detail::Node< T >* left, detail::Node< T >* right,
      Compare comp)
  {
    if (!left)
    {
      return right;
    }
    if (!right)
    {
      return left;
    }
    detail::Node< T >* result = nullptr;
    if (comp(right->data, left->data))
    {
      result = right;
      right = right->next;
    }
    else
    {
      result = left;
      left = left->next;
    }
    detail::Node< T >* tail = result;
    while (left && right)
    {
      if (comp(right->data, left->data))
      {
        tail->next = right;
        right = right->next;
      }
      else
      {
        tail->next = left;
        left = left->next;
      }
      tail = tail->next;
    }
    if (left)
    {
      tail->next = left;
    }
    else
    {
      tail->next = right;
    }
    return result;
  }

  template< class T >
  template< class Predicate >
  void List< T >::partition(Predicate pred)
  {
    List< T > selected;
    detail::Node< T >* selectedTail = selected.fake_;
    detail::Node< T >* prev = fake_;
    while (prev->next != fake_)
    {
      if (pred(prev->next->data))
      {
        detail::Node< T >* node = unlinkAfter(prev);
        selected.linkAfter(selectedTail, node);
        selectedTail = selectedTail->next;
      }
      else
      {
        prev = prev->next;
      }
    }
    spliceAfter(beforeBegin(), selected);
  }

  template< class T >
  LIter< T > List< T >::pushFront(const T& value)
  {
    return emplaceFront(value);
  }

  template< class T >
  void List< T >::spliceAfter(LIter< T > pos, List& other,
      LIter< T > beforeElement) noexcept
  {
    if (!pos.current_ || !beforeElement.current_)
    {
      return;
    }
    if (beforeElement.current_->next == other.fake_)
    {
      return;
    }
    detail::Node< T >* node = other.unlinkAfter(beforeElement.current_);
    linkAfter(pos.current_, node);
  }

  template< class T >
  void List< T >::spliceAfter(LIter< T > pos, List& other) noexcept
  {
    if (!pos.current_ || other.empty())
    {
      return;
    }
    detail::Node< T >* first = other.fake_->next;
    detail::Node< T >* last = first;
    while (last->next != other.fake_)
    {
      last = last->next;
    }
    last->next = pos.current_->next;
    pos.current_->next = first;
    other.fake_->next = other.fake_;
  }

  template< class T >
  void List< T >::spliceAfter(LIter< T > pos, List& other,
      LIter< T > first, LIter< T > last) noexcept
  {
    if (!pos.current_ || !first.current_ || !last.current_)
    {
      return;
    }
    if (first.current_->next == last.current_)
    {
      return;
    }
    detail::Node< T > * rangeFirst = first.current_->next;
    detail::Node< T > * rangeLast = rangeFirst;
    while (rangeLast->next != last.current_)
    {
      rangeLast = rangeLast->next;
    }
    first.current_->next = last.current_;
    rangeLast->next = pos.current_->next;
    pos.current_->next = rangeFirst;
  }

  template< class T >
  void List< T >::popFront() noexcept
  {
    if (fake_->next != fake_)
    {
      detail::Node< T >* tmp = fake_->next;
      fake_->next = tmp->next;
      delete tmp;
    }
  }

  template< class T >
  void List< T >::eraseAfter(LIter< T > pos)
  {
    if ((pos.current_) && (pos.current_->next != fake_))
    {
      detail::Node< T >* tmp = pos.current_->next;
      pos.current_->next = tmp->next;
      delete tmp;
    }
  }

  template< class T >
  void List< T >::popBack() noexcept
  {
    if (fake_->next != fake_)
    {
      detail::Node< T >* prev = fake_;
      detail::Node< T >* cur = fake_->next;
      while (cur->next != fake_)
      {
        prev = cur;
        cur = cur->next;
      }
      prev->next = fake_;
      delete cur;
    }
  }

  template< class T >
  void List< T >::erase(LIter< T > pos)
  {
    if (!pos.current_ || pos.current_ == fake_)
    {
      throw std::runtime_error("invalid iterator");
    }
    detail::Node< T >* prev = fake_;
    detail::Node< T >* cur = fake_->next;
    while (cur != fake_ && cur != pos.current_)
    {
      prev = cur;
      cur = cur->next;
    }
    if (cur == fake_)
    {
      throw std::runtime_error("element not found");
    }
    prev->next = cur->next;
    delete cur;
  }

  template< class T >
  LIter< T > List< T >::insertAfter(LIter< T > pos, const T& value)
  {
    return emplaceAfter(pos, value);
  }

  template< class T >
  LIter< T > List< T >::pushBack(const T& value)
  {
    return emplaceBack(value);
  }

  template< class T >
  LIter< T > List< T >::beforeBegin()
  {
    return LIter< T >(fake_);
  }

  template< class T >
  LIter< T > List< T >::insertAfter(LIter< T > pos, T&& value)
  {
    return emplaceAfter(pos, std::forward< T >(value));
  }

  template< class T >
  LIter< T > List< T >::pushFront(T&& value)
  {
    return emplaceFront(std::forward< T >(value));
  }

  template< class T >
  template< class... Args >
  LIter< T > List< T >::emplaceAfter(LIter< T > pos, Args&&... args)
  {
    if (!pos.current_)
    {
      throw std::runtime_error("invalid iterator");
    }
    detail::Node< T >* node = new detail::Node< T >(pos.current_->next,
        std::forward< Args >(args)...);
    pos.current_->next = node;
    return LIter< T >(node);
  }

  template< class T >
  template< class... Args >
  LIter< T > List< T >::emplaceFront(Args&&... args)
  {
    return emplaceAfter(beforeBegin(), std::forward< Args >(args)...);
  }

  template< class T >
  template< class... Args >
  LIter< T > List< T >::emplaceBack(Args&&... args)
  {
    detail::Node< T >* cur = fake_;
    while (cur->next != fake_)
    {
      cur = cur->next;
    }
    return emplaceAfter(LIter< T >(cur), std::forward< Args >(args)...);
  }

  template< class T >
  LIter< T > List< T >::pushBack(T&& value)
  {
    return emplaceBack(std::forward< T >(value));
  }

  template< class T >
  void List< T >::clear() noexcept
  {
    detail::Node< T >* cur = fake_->next;
    while (cur != fake_)
    {
      detail::Node< T >* tmp = cur;
      cur = cur->next;
      delete tmp;
    }
    fake_->next = fake_;
  }

  template< class T >
  void List< T >::swap(List& other) noexcept
  {
    std::swap(fake_, other.fake_);
  }
}

#endif
