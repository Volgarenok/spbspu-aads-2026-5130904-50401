#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <utility>
#include <limits>

namespace vasyakin
{
  namespace detail
  {
    template< class T > class Node;
  }

  template< class T > class List;
  const size_t max = std::numeric_limits< size_t >::max();

  template< class T >
  class LIter
  {
  public:
    T& operator*() const noexcept;
    T* operator->() const noexcept;
    LIter& operator++() noexcept;
    LIter operator++(int) noexcept;
    bool operator==(const LIter& other) const noexcept;
    bool operator!=(const LIter& other) const noexcept;
    detail::Node< T >* getPtr() const noexcept;

  private:
    friend class List< T >;
    detail::Node< T >* ptr_;
    explicit LIter(detail::Node< T >* p) noexcept;
  };

  template< class T >
  class LCIter
  {
  public:
    const T& operator*() const noexcept;
    const T* operator->() const noexcept;
    LCIter& operator++() noexcept;
    LCIter operator++(int) noexcept;
    bool operator==(const LCIter& other) const noexcept;
    bool operator!=(const LCIter& other) const noexcept;
    const detail::Node< T >* getPtr() const noexcept;

  private:
    friend class List< T >;
    const detail::Node< T >* ptr_;
    explicit LCIter(const detail::Node< T >* p) noexcept;
    explicit LCIter(const LIter< T >& it) noexcept;
  };

  namespace detail
  {
    template< class T >
    class Node
    {
    public:
      explicit Node(const T& value);

    private:
      T val_;
      Node< T >* next_;
      friend class List< T >;
      friend class LIter< T >;
      friend class LCIter< T >;
    };
  }

  template< class T >
  class List
  {
  public:
    List();
    List(const List& other);
    List(List&& other) noexcept;
    explicit List(const T& value);
    ~List() noexcept;
    List& operator=(const List& other);
    List& operator=(List&& other) noexcept;

    detail::Node< T >* insert(detail::Node< T >* h, const T& value);
    detail::Node< T >* erase(detail::Node< T >* h);
    detail::Node< T >* pushBack(const T& value);
    void swap(List& other) noexcept;
    void clear() noexcept;

    LIter< T > begin();
    LIter< T > end();
    LCIter< T > begin() const;
    LCIter< T > end() const;
    LCIter< T > cbegin() const;
    LCIter< T > cend() const;

    size_t getSize() const noexcept;
    detail::Node< T >* getFake() const noexcept;
    detail::Node< T >* getFirst() const noexcept;

  private:
    detail::Node< T >* fake_node_;
    size_t size_;
  };

  template< class T >
  LIter< T >::LIter(detail::Node< T >* p) noexcept:
    ptr_(p)
  {}

  template< class T >
  T& LIter< T >::operator*() const noexcept
  {
    return ptr_->val_;
  }

  template< class T >
  T* LIter< T >::operator->() const noexcept
  {
    return &(ptr_->val_);
  }

  template< class T >
  LIter< T >& LIter< T >::operator++() noexcept
  {
    ptr_ = ptr_->next_;
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator++(int) noexcept
  {
    LIter< T > tmp = *this;
    ptr_ = ptr_->next_;
    return tmp;
  }

  template< class T >
  bool LIter< T >::operator==(const LIter& other) const noexcept
  {
    return ptr_ == other.ptr_;
  }

  template< class T >
  bool LIter< T >::operator!=(const LIter& other) const noexcept
  {
    return ptr_ != other.ptr_;
  }

  template< class T >
  detail::Node< T >* LIter< T >::getPtr() const noexcept
  {
    return ptr_;
  }

  template< class T >
  LCIter< T >::LCIter(const detail::Node< T >* p) noexcept:
    ptr_(p)
  {}

  template< class T >
  LCIter< T >::LCIter(const LIter< T >& it) noexcept:
    ptr_(it.ptr_)
  {}

  template< class T >
  const T& LCIter< T >::operator*() const noexcept
  {
    return ptr_->val_;
  }

  template< class T >
  const T* LCIter< T >::operator->() const noexcept
  {
    return &(ptr_->val_);
  }

  template< class T >
  LCIter< T >& LCIter< T >::operator++() noexcept
  {
    ptr_ = ptr_->next_;
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator++(int) noexcept
  {
    LCIter< T > tmp = *this;
    ptr_ = ptr_->next_;
    return tmp;
  }

  template< class T >
  bool LCIter< T >::operator==(const LCIter& other) const noexcept
  {
    return ptr_ == other.ptr_;
  }

  template< class T >
  bool LCIter< T >::operator!=(const LCIter& other) const noexcept
  {
    return ptr_ != other.ptr_;
  }

  template< class T >
  const detail::Node< T >* LCIter< T >::getPtr() const noexcept
  {
    return ptr_;
  }

  template< class T >
  detail::Node< T >::Node(const T& value):
    val_(value),
    next_(nullptr)
  {}

  template< class T >
  List< T >::List():
    fake_node_(new detail::Node< T >(T{})),
    size_(0)
  {
    fake_node_->next_ = fake_node_;
  }

  template< class T >
  List< T >::List(const List& other):
    fake_node_(new detail::Node< T >(T{})),
    size_(0)
  {
    fake_node_->next_ = fake_node_;

    for (auto it = other.cbegin(); it != other.cend(); ++it)
    {
      pushBack(*it);
    }
  }

  template< class T >
  List< T >::List(List&& other) noexcept:
    fake_node_(std::exchange(other.fake_node_, nullptr)),
    size_(std::exchange(other.size_, 0))
  {}

  template< class T >
  List< T >::List(const T& value):
    fake_node_(new detail::Node< T >(T{})),
    size_(1)
  {
    try
    {
      detail::Node< T >* head = new detail::Node< T >(value);
      head->next_ = fake_node_;
      fake_node_->next_ = head;
    }
    catch (...)
    {
      delete fake_node_;
      fake_node_ = nullptr;
      throw;
    }
  }

  template< class T >
  List< T >::~List() noexcept
  {
    clear();
    delete fake_node_;
    fake_node_ = nullptr;
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
  void List< T >::clear() noexcept
  {
    if (!fake_node_)
    {
      return;
    }
    if (fake_node_->next_ == fake_node_)
    {
      size_ = 0;
      return;
    }

    detail::Node< T >* current = fake_node_->next_;
    while (current != fake_node_)
    {
      detail::Node< T >* next = current->next_;
      delete current;
      current = next;
    }
    fake_node_->next_ = fake_node_;
    size_ = 0;
  }

  template< class T >
  detail::Node< T >* List< T >::insert(detail::Node< T >* h, const T& value)
  {
    detail::Node< T >* new_node = new detail::Node< T >(value);

    if (fake_node_->next_ == fake_node_)
    {
      new_node->next_ = fake_node_;
      fake_node_->next_ = new_node;
    }
    else
    {
      new_node->next_ = h->next_;
      h->next_ = new_node;
    }
    ++size_;
    return new_node;
  }

  template< class T >
  detail::Node< T >* List< T >::erase(detail::Node< T >* h)
  {
    if (!fake_node_ || h->next_ == fake_node_)
    {
      return fake_node_;
    }
    detail::Node< T >* to_delete = h->next_;
    h->next_ = to_delete->next_;
    delete to_delete;
    --size_;
    return h;
  }

  template< class T >
  detail::Node< T >* List< T >::pushBack(const T& value)
  {
    detail::Node< T >* new_node = new detail::Node< T >(value);
    detail::Node< T >* last = fake_node_;
    while (last->next_ != fake_node_)
    {
      last = last->next_;
    }
    new_node->next_ = fake_node_;
    last->next_ = new_node;
    ++size_;
    return new_node;
  }

  template< class T >
  LIter< T > List< T >::begin()
  {
    return LIter< T >(fake_node_->next_);
  }

  template< class T >
  LIter< T > List< T >::end()
  {
    return LIter< T >(fake_node_);
  }

  template< class T >
  LCIter< T > List< T >::begin() const
  {
    return LCIter< T >(fake_node_->next_);
  }

  template< class T >
  LCIter< T > List< T >::end() const
  {
    return LCIter< T >(fake_node_);
  }

  template< class T >
  LCIter< T > List< T >::cbegin() const
  {
    return LCIter< T >(fake_node_->next_);
  }

  template< class T >
  LCIter< T > List< T >::cend() const
  {
    return LCIter< T >(fake_node_);
  }

  template< class T >
  void List< T >::swap(List& other) noexcept
  {
    std::swap(fake_node_, other.fake_node_);
    std::swap(size_, other.size_);
  }

  template< class T >
  size_t List< T >::getSize() const noexcept
  {
    return size_;
  }

  template< class T >
  detail::Node< T >* List< T >::getFake() const noexcept
  {
    return fake_node_;
  }

  template< class T >
  detail::Node< T >* List< T >::getFirst() const noexcept
  {
    return fake_node_->next_;
  }
}

#endif
