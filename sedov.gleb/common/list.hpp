#ifndef LIST_HPP
#define LIST_HPP
#include <utility>
#include <cstddef>
#include <limits>

namespace sedov
{
  const size_t MAX = std::numeric_limits< size_t >::max();

  template< class T >
  class List;

  template< class T >
  struct Node
  {
    T val_;
    Node< T > * next_;
    Node< T > * prev_;
    Node(const T & value):
      val_(value), next_(nullptr), prev_(nullptr)
    {}
    Node(T && value):
      val_(value), next_(nullptr), prev_(nullptr)
    {}
  };

  template< class T >
  class LIter
  {
    friend class List< T >;
    Node< T > * ptr_;
  public:
    LIter(Node< T > * p = nullptr):
      ptr_(p)
    {}

    T & operator*() const
    {
      return ptr_->val_;
    }

    T * operator->() const
    {
      return &ptr_->val_;
    }

    LIter & operator++()
    {
      ptr_ = ptr_->next_;
      return *this;
    }

    LIter operator++(int)
    {
      LIter temp = *this;
      ptr_ = ptr_->next_;
      return temp;
    }

    LIter & operator--()
    {
      ptr_ = ptr_->prev_;
      return *this;
    }

    LIter operator--(int)
    {
      LIter temp = *this;
      ptr_ = ptr_->prev_;
      return temp;
    }

    bool operator==(const LIter & other) const
    {
      return ptr_ == other.ptr_;
    }

    bool operator!=(const LIter & other) const
    {
      return !(ptr_ == other.ptr_);
    }
  };

  template< class T >
  class LCIter
  {
    friend class List< T >;
    const Node< T > * ptr_;
  public:
    LCIter(const Node< T > * p = nullptr):
      ptr_(p)
    {}

    const T & operator*() const
    {
        return ptr_->val_;
    }

    const T * operator->() const
    {
        return &ptr_->val_;
    }

    LCIter & operator++()
    {
        ptr_ = ptr_->next_;
        return *this;
    }

    LCIter operator++(int)
    {
        LCIter tmp = *this;
        ptr_ = ptr_->next_;
        return tmp;
    }

    LCIter & operator--()
    {
        ptr_ = ptr_->prev_;
        return *this;
    }

    LCIter operator--(int)
    {
        LCIter tmp = *this;
        ptr_ = ptr_->prev_;
        return tmp;
    }

    bool operator==(const LCIter & other) const
    {
        return ptr_ == other.ptr_;
    }

    bool operator!=(const LCIter & other) const
    {
        return !(ptr_ == other.ptr_);
    }
  };

  template< class T >
  class List
  {
    Node< T > * head_;
    Node< T > * tail_;
    size_t size_;
  public:
    List():
      head_(nullptr), tail_(nullptr), size_(0)
    {}

    List(const List & h):
      List()
    {
      for (Node< T > * cur = h.head_; cur != nullptr; cur = cur->next_)
      {
        pushBack(cur->val_);
      }
    }

    List(List && h) noexcept:
      head_(h.head_), tail_(h.tail_), size_(h.size_)
    {
      h.head_ = nullptr;
      h.tail_ = nullptr;
      h.size_ = 0;
    }

    ~List()
    {
      clear();
    }

    List & operator=(const List & h)
    {
      if (this != &h)
      {
        List temp(h);
        swap(temp);
      }
      return *this;
    }

    LIter< T > begin()
    {
      return LIter< T >(head_);
    }

    LIter< T > end()
    {
      return LIter< T >(nullptr);
    }

    LCIter< T > cbegin() const
    {
      return LCIter< T >(head_);
    }

    LCIter< T > cend() const
    {
      return LCIter< T >(nullptr);
    }

    T & front()
    {
      return head_->val_;
    }

    const T & front() const
    {
      return head_->val_;
    }

    T & back()
    {
      return tail_->val_;
    }

    const T & back() const
    {
      return tail_->val_;
    }

    void pushFront(const T & v)
    {
      Node< T > * newNode = new Node< T >(v);
      newNode->next_ = head_;
      if (head_)
      {
        head_->prev_ = newNode;
      }
      else
      {
        tail_ = newNode;
      }
      head_ = newNode;
      ++size_;
    }

    void pushBack(const T & v)
    {
      Node< T > * newNode = new Node< T >(v);
      newNode->prev_ = tail_;
      if (tail_)
      {
        tail_->next_ = newNode;
      }
      else
      {
        head_ = newNode;
      }
      tail_ = newNode;
      ++size_;
    }

    LIter< T > insert(LIter< T > p, const T & v)
    {
      if (!p.ptr_)
      {
        pushBack(v);
        return LIter< T >(tail_);
      }
      if (p.ptr_ == head_)
      {
        pushFront(v);
        return LIter< T >(head_);
      }
      Node< T > * newNode = new Node< T >(v);
      Node< T > * next = p.ptr_;
      Node< T > * prev = next->prev_;
      newNode->prev_ = prev;
      newNode->next_ = next;
      prev->next_ = newNode;
      next->prev_ = newNode;
      ++size_;
      return LIter< T >(newNode);
    }

    void popFront()
    {
      if (!head_)
      {
        return;
      }
      Node< T > * temp = head_;
      head_ = head_->next_;
      if (head_)
      {
        head_->prev_ = nullptr;
      }
      else
      {
        tail_ = nullptr;
      }
      delete temp;
      --size_;
    }

    void popBack()
    {
      if (!tail_)
      {
        return;
      }
      Node< T > * temp = tail_;
      tail_ = tail_->prev_;
      if (tail_)
      {
        tail_->next_ = nullptr;
      }
      else
      {
        head_ = nullptr;
      }
      delete temp;
      --size_;
    }

    LIter< T > erase(LIter< T > p)
    {
      if (!p.ptr_)
      {
        return end();
      }
      Node< T > * next = p.ptr_->next_;
      Node< T > * prev = p.ptr_->prev_;
      if (prev)
      {
        prev->next_ = next;
      }
      else
      {
        head_ = next;
      }
      if (next)
      {
        next->prev_ = prev;
      }
      else
      {
        tail_ = prev;
      }
      delete p.ptr_;
      --size_;
      return LIter< T >(next);
    }

    void clear()
    {
      while (head_)
      {
        Node< T > * temp = head_;
        head_ = head_->next_;
        delete temp;
      }
      tail_ = nullptr;
      size_ = 0;
    }

    size_t size() const
    {
      return size_;
    }

    void swap(List & h) noexcept
    {
      std::swap(head_, h.head_);
      std::swap(tail_, h.tail_);
      std::swap(size_, h.size_);
    }
  };
}

#endif
