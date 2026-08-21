#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
#include <utility>
#include <memory>

namespace detail
{
  template<class T>
  struct Node
  {
    T val_;
    Node<T>* next_;
    Node<T>* prev_;
    Node(const T& v, Node<T>* n = nullptr, Node<T>* p = nullptr);
    Node(T&& v, Node<T>* n = nullptr, Node<T>* p = nullptr);
  };

  template<class T>
  Node<T>::Node(const T& v, Node<T>* n, Node<T>* p) :
    val_(v),
    next_(n),
    prev_(p)
  {}

  template<class T>
  Node<T>::Node(T&& v, Node<T>* n, Node<T>* p) :
    val_(std::forward<T>(v)),
    next_(n),
    prev_(p)
  {}
};

namespace petrov
{
  template<class T>
  class LIter;

  template<class T>
  class LCIter;

  template<class T>
  class List
  {
  public:
    List() noexcept;
    List(const List<T>& l);
    List(List<T>&& l) noexcept;
    List(size_t s, const T& init);
    ~List();

    List<T>& operator=(List<T>&& l) noexcept;
    List<T>& operator=(const List<T>& l);

    bool empty() const noexcept;
    size_t size() const noexcept;
    LIter<T> begin() noexcept;
    LIter<T> end() noexcept;
    LCIter<T> begin() const noexcept;
    LCIter<T> end() const noexcept;

    void push_back(const T& d);
    void push_back(T&& d);

    void push_front(const T& d);
    void push_front(T&& d);

    void swap(List<T>& d) noexcept;
    void pop_back();
    void pop_front();
    void clear() noexcept;
  private:
    detail::Node<T>* m_h_;
    detail::Node<T>* m_t_;
    size_t m_s_;
  };

  template<class T>
  class LIter
  {
  public:
    LIter() noexcept;
    LIter(detail::Node<T>* n) noexcept;
    bool operator==(const LIter<T>& i) const noexcept;
    bool operator!=(const LIter<T>& i) const noexcept;
    T& operator*() const noexcept;
    LIter<T>& operator++() noexcept;
    LIter<T> operator++(int) noexcept;
    LIter<T>& operator--() noexcept;
    LIter<T> operator--(int) noexcept;
    T* operator->() const noexcept;
  private:
    friend class List<T>;
    detail::Node<T>* m_cur_;
  };

  template<class T>
  LIter<T>::LIter() noexcept :
    m_cur_(nullptr)
  {}

  template<class T>
  LIter<T>::LIter(detail::Node<T>* n) noexcept :
    m_cur_(n)
  {}

  template<class T>
  class LCIter
  {
  public:
    LCIter() noexcept;
    LCIter(const detail::Node<T>* n) noexcept;
    bool operator==(const LCIter<T>& i) const noexcept;
    bool operator!=(const LCIter<T>& i) const noexcept;
    const T& operator*() const noexcept;
    LCIter<T>& operator++() noexcept;
    LCIter<T> operator++(int) noexcept;
    LCIter<T>& operator--() noexcept;
    LCIter<T> operator--(int) noexcept;
    const T* operator->() const noexcept;
  private:
    friend class List<T>;
    const detail::Node<T>* m_cur_;
  };

  template<class T>
  LCIter<T>::LCIter() noexcept :
    m_cur_(nullptr)
  {}

  template<class T>
  LCIter<T>::LCIter(const detail::Node<T>* n) noexcept :
    m_cur_(n)
  {}

  template<class T>
  List<T>::List() noexcept :
    m_h_(nullptr),
    m_t_(nullptr),
    m_s_(0)
  {}

  template<class T>
  void List<T>::clear() noexcept
  {
    while (m_h_ != nullptr)
    {
      detail::Node<T>* next_ = m_h_->next_;
      delete m_h_;
      m_h_ = next_;
    }
    m_t_ = nullptr;
    m_s_ = 0;
  }

  template<class T>
  List<T>::~List()
  {
    clear();
  }

  template<class T>
  void List<T>::pop_front()
  {
    if (!empty())
    {
      detail::Node<T>* new_h_ = m_h_->next_;
      delete m_h_;
      m_h_ = new_h_;
      if (m_h_ == nullptr)
      {
        m_t_ = nullptr;
      }
      else
      {
        m_h_->prev_ = nullptr;
      }
      m_s_--;
    }
  }

  template<class T>
  void List<T>::swap(List<T>& d) noexcept
  {
    std::swap(m_h_, d.m_h_);
    std::swap(m_t_, d.m_t_);
    std::swap(m_s_, d.m_s_);
  }

  template<class T>
  void List<T>::pop_back()
  {
    if (!empty())
    {
      detail::Node<T>* new_t_ = m_t_->prev_;
      delete m_t_;
      m_t_ = new_t_;
      if (m_t_ == nullptr)
      {
        m_h_ = nullptr;
      }
      else
      {
        m_t_->next_ = nullptr;
      }
      m_s_--;
    }
  }

  template<class T>
  bool List<T>::empty() const noexcept
  {
    return m_s_ == 0;
  }

  template<class T>
  size_t List<T>::size() const noexcept
  {
    return m_s_;
  }

  template<class T>
  void List<T>::push_back(const T& d)
  {
    detail::Node<T>* new_node = new detail::Node<T>(d, nullptr, m_t_);
    if (empty())
    {
      m_h_ = new_node;
    }
    else
    {
      m_t_->next_ = new_node;
    }
    m_t_ = new_node;
    m_s_++;
  }

  template<class T>
  void List<T>::push_front(const T& d)
  {
    detail::Node<T>* new_node = new detail::Node<T>(d, m_h_, nullptr);
    if (empty())
    {
      m_t_ = new_node;
    }
    else
    {
      m_h_->prev_ = new_node;
    }
    m_h_ = new_node;
    m_s_++;
  }

  template<class T>
  void List<T>::push_back(T&& d)
  {
    detail::Node<T>* new_node = new detail::Node<T>(std::forward<T>(d), nullptr, m_t_);
    if (empty())
    {
      m_h_ = new_node;
    }
    else
    {
      m_t_->next_ = new_node;
    }
    m_t_ = new_node;
    m_s_++;
  }

  template<class T>
  void List<T>::push_front(T&& d)
  {
    detail::Node<T>* new_node = new detail::Node<T>(std::forward<T>(d), m_h_, nullptr);
    if (empty())
    {
      m_t_ = new_node;
    }
    else
    {
      m_h_->prev_ = new_node;
    }
    m_h_ = new_node;
    m_s_++;
  }

  template<class T>
  List<T>::List(const List<T>& l) :
    m_h_(nullptr),
    m_t_(nullptr),
    m_s_(0)
  {
    detail::Node<T>* nod_ = l.m_h_;
    while (nod_ != nullptr)
    {
      try
      {
        push_back(nod_->val_);
      }
      catch (...)
      {
        clear();
        throw;
      }
      nod_ = nod_->next_;
    }
  }

  template<class T>
  List<T>& List<T>::operator=(const List<T>& l)
  {
    if (this != &l)
    {
      List<T> temp(l);
      swap(temp);
    }
    return *this;
  }

  template<class T>
  List<T>::List(List<T>&& l) noexcept :
    m_h_(std::exchange(l.m_h_, nullptr)),
    m_t_(std::exchange(l.m_t_, nullptr)),
    m_s_(std::exchange(l.m_s_, 0))
  {}

  template<class T>
  List<T>& List<T>::operator=(List<T>&& l) noexcept
  {
    swap(l);
    return *this;
  }

  template<class T>
  List<T>::List(size_t s, const T& init) :
    m_h_(nullptr),
    m_t_(nullptr),
    m_s_(0)
  {
    for (size_t i_ = 0; i_ < s; ++i_)
    {
      try
      {
        push_back(init);
      }
      catch (...)
      {
        clear();
        throw;
      }
    }
  }

  template<class T>
  LIter<T> List<T>::begin() noexcept
  {
    return LIter<T>(m_h_);
  }

  template<class T>
  LIter<T> List<T>::end() noexcept
  {
    return LIter<T>(nullptr);
  }

  template<class T>
  LIter<T>& LIter<T>::operator++() noexcept
  {
    if (m_cur_)
    {
      m_cur_ = m_cur_->next_;
    }
    return *this;
  }

  template<class T>
  LIter<T> LIter<T>::operator++(int) noexcept
  {
    LIter<T> d_ = *this;
    ++(*this);
    return d_;
  }

  template<class T>
  LIter<T> LIter<T>::operator--(int) noexcept
  {
    LIter<T> d_ = *this;
    --(*this);
    return d_;
  }

  template<class T>
  LIter<T>& LIter<T>::operator--() noexcept
  {
    if (m_cur_)
    {
      m_cur_ = m_cur_->prev_;
    }
    return *this;
  }

  template<class T>
  T& LIter<T>::operator*() const noexcept
  {
    return m_cur_->val_;
  }

  template<class T>
  T* LIter<T>::operator->() const noexcept
  {
    return &(m_cur_->val_);
  }

  template<class T>
  bool LIter<T>::operator==(const LIter<T>& i) const noexcept
  {
    return m_cur_ == i.m_cur_;
  }

  template<class T>
  bool LIter<T>::operator!=(const LIter<T>& i) const noexcept
  {
    return m_cur_ != i.m_cur_;
  }

  template<class T>
  bool LCIter<T>::operator==(const LCIter<T>& i) const noexcept
  {
    return m_cur_ == i.m_cur_;
  }

  template<class T>
  bool LCIter<T>::operator!=(const LCIter<T>& i) const noexcept
  {
    return m_cur_ != i.m_cur_;
  }

  template<class T>
  const T& LCIter<T>::operator*() const noexcept
  {
    return m_cur_->val_;
  }

  template<class T>
  const T* LCIter<T>::operator->() const noexcept
  {
    return std::addressof(m_cur_->val_);
  }

  template<class T>
  LCIter<T>& LCIter<T>::operator++() noexcept
  {
    if (m_cur_)
    {
      m_cur_ = m_cur_->next_;
    }
    return *this;
  }

  template<class T>
  LCIter<T> LCIter<T>::operator++(int) noexcept
  {
    LCIter<T> d_ = *this;
    ++(*this);
    return d_;
  }

  template<class T>
  LCIter<T>& LCIter<T>::operator--() noexcept
  {
    if (m_cur_)
    {
      m_cur_ = m_cur_->prev_;
    }
    return *this;
  }

  template<class T>
  LCIter<T> LCIter<T>::operator--(int) noexcept
  {
    LCIter<T> d_ = *this;
    --(*this);
    return d_;
  }

  template<class T>
  LCIter<T> List<T>::begin() const noexcept
  {
    return LCIter<T>(m_h_);
  }

  template<class T>
  LCIter<T> List<T>::end() const noexcept
  {
    return LCIter<T>(nullptr);
  }
};

#endif
