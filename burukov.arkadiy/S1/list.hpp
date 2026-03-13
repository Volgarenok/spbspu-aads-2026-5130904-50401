#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>

namespace burukov
{
  template< class T >
  class List;

  template< class T >
  struct Node
  {
    T val_;
    Node< T > *next_;
    Node(const T &value, Node< T > *next):
      val_(value),
      next_(next)
    {}
  };

  template< class T >
  class LIter
  {
    friend class List< T >;
  public:
    LIter():
      ptr_(nullptr)
    {}
    explicit LIter(Node< T > *p):
      ptr_(p)
    {}
    T &operator*()
    {
      return ptr_->val_;
    }
    T *operator->()
    {
      return &(ptr_->val_);
    }
    LIter< T > &operator++()
    {
      ptr_ = ptr_->next_;
      return *this;
    }
    LIter< T > operator++(int)
    {
      LIter< T > temp(*this);
      ptr_ = ptr_->next_;
      return temp;
    }
    bool operator==(const LIter< T > &rhs) const
    {
      return ptr_ == rhs.ptr_;
    }
    bool operator!=(const LIter< T > &rhs) const
    {
      return ptr_ != rhs.ptr_;
    }

  private:
    Node< T > *ptr_;
  };

  template< class T >
  class LCIter
  {
    friend class List< T >;
  public:
    LCIter():
      ptr_(nullptr)
    {}
    explicit LCIter(const Node< T > *p):
      ptr_(p)
    {}
    const T &operator*() const
    {
      return ptr_->val_;
    }
    const T *operator->() const
    {
      return &(ptr_->val_);
    }
    LCIter< T > &operator++()
    {
      ptr_ = ptr_->next_;
      return *this;
    }
    LCIter< T > operator++(int)
    {
      LCIter< T > temp(*this);
      ptr_ = ptr_->next_;
      return temp;
    }
    bool operator==(const LCIter< T > &rhs) const
    {
      return ptr_ == rhs.ptr_;
    }
    bool operator!=(const LCIter< T > &rhs) const
    {
      return ptr_ != rhs.ptr_;
    }

  private:
    const Node< T > *ptr_;
  };

  template< class T >
  class List
  {
  public:
    List():
      head_(nullptr),
      size_(0)
    {}
    ~List()
    {
      clear();
    }
    bool empty() const
    {
      return size_ == 0;
    }
    size_t size() const
    {
      return size_;
    }
    T &front()
    {
      return head_->val_;
    }
    const T &front() const
    {
      return head_->val_;
    }
    void pushFront(const T &val)
    {
      head_ = new Node< T >(val, head_);
      ++size_;
    }
    void popFront()
    {
      Node< T > *old = head_;
      head_ = head_->next_;
      delete old;
      --size_;
    }
    LIter< T > insertAfter(LIter< T > pos, const T &val)
    {
      Node< T > *created = new Node< T >(val, pos.ptr_->next_);
      pos.ptr_->next_ = created;
      ++size_;
      return LIter< T >(created);
    }
    LIter< T > eraseAfter(LIter< T > pos)
    {
      Node< T > *victim = pos.ptr_->next_;
      pos.ptr_->next_ = victim->next_;
      delete victim;
      --size_;
      return LIter< T >(pos.ptr_->next_);
    }
    void clear()
    {
      while (head_)
      {
        Node< T > *tmp = head_;
        head_ = head_->next_;
        delete tmp;
      }
      size_ = 0;
    }
    void swap(List< T > &other)
    {
      Node< T > *tmpHead = head_;
      head_ = other.head_;
      other.head_ = tmpHead;
      const size_t tmpSize = size_;
      size_ = other.size_;
      other.size_ = tmpSize;
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

  private:
    Node< T > *head_;
    size_t size_;
  };
}

#endif