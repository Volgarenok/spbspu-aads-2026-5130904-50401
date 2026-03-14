#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>

namespace hvostov {
  template< class T >
  struct Node {
    T val_;
    Node< T >* next_;
  };

  template< class T > class List;

  template< class T >
  class Liter {
  public:
    Liter() noexcept;
    Liter(Node< T >* curr) noexcept;
    Liter(const Liter< T >& liter) noexcept;
    Liter(Liter< T >&& liter) noexcept;
    Liter< T >& operator=(const Liter< T >& liter) noexcept;
    Liter< T >& operator=(Liter< T >&& liter) noexcept;
    Liter< T >& operator++() noexcept;
    Liter< T > operator++(int) noexcept;
    T& operator*() const noexcept;
    T* operator->() const noexcept;
    bool operator==(const Liter< T >& liter) const noexcept;
    bool operator!=(const Liter< T >& liter) const noexcept;

  private:
    friend class List< T >;
    Node< T >* curr_;
  };

  template< class T >
  class LCiter {
  public:
    LCiter() noexcept;
    LCiter(Node< T >* curr) noexcept;
    LCiter(const LCiter< T >& lciter) noexcept;
    LCiter(LCiter< T >&& lciter) noexcept;
    LCiter< T >& operator=(const LCiter< T >& lciter) noexcept;
    LCiter< T >& operator=(LCiter< T >&& lciter) noexcept;
    LCiter< T >& operator++() noexcept;
    LCiter< T > operator++(int) noexcept;
    const T& operator*() const noexcept;
    const T* operator->() const noexcept;
    bool operator==(const LCiter< T >& lciter) const noexcept;
    bool operator!=(const LCiter< T >& lciter) const noexcept;

  private:
    friend class List< T >;
    Node< T >* curr_;
  };

  template< class T >
  class List {
    public:
      List();
      List(const List< T >& list);
      List(List< T >&& list) noexcept;
      ~List();
      List< T >& operator=(const List< T >& list);
      List< T >& operator=(List< T >&& list) noexcept;

      Liter< T > begin() const noexcept;
      Liter< T > end() const noexcept;
      LCiter< T > cend() const noexcept;
      LCiter< T > cbegin() const noexcept;

      Liter< T > insertAfter(const Liter< T > it, const T& val);
      Liter< T > pushBack(const T&val);
      void eraseAfter(const Liter< T > it) noexcept;
      void clear() noexcept;
      bool empty() const noexcept;
    private:
      Node< T >* fake_;
      Node< T >* createFake();
      void rmFake() noexcept;
  };
}

template< class T >
hvostov::Node< T >* hvostov::List< T >::createFake()
{
  fake_ = new Node< T >();
  return fake_;
}

template< class T >
void hvostov::List< T >::rmFake() noexcept
{
  delete fake_;
}

template< class T >
hvostov::List< T >::List():
  fake_(nullptr)
{
  createFake();
  fake_->next_ = fake_;
}

template< class T >
hvostov::List< T >::List(const List< T >& list):
  fake_(nullptr)
{
  createFake();
  fake_->next_ = fake_;
  Liter< T > i = begin();
  for (Liter< T > it = list.begin(); it != list.end(); it++) {
    try {
      i = insertAfter(i, *it);
    } catch (...){
      clear();
      rmFake();
      throw;
    }
  }
}

template< class T >
hvostov::List< T >::List(List< T >&& list) noexcept:
  fake_(list.fake_)
{
  list.fake_ = nullptr;
}

template< class T >
hvostov::List< T >::~List()
{
  clear();
  rmFake();
}

template< class T >
hvostov::List< T >& hvostov::List< T >::operator=(const List< T >& list)
{
  if (this == &list) {
    return *this;
  }
  clear();
  Liter< T > i = begin();
  for (Liter< T > it = list.begin(); it != list.end(); it++) {
    i = insertAfter(i, *it);
  }
  return *this;
}

template< class T >
hvostov::List< T >& hvostov::List< T >::operator=(List< T >&& list) noexcept
{
  if (this == &list) {
    return *this;
  }
  clear();
  rmFake();
  fake_ = list.fake_;
  list.fake_ = nullptr;
  return *this;
}

template< class T >
hvostov::Liter< T > hvostov::List< T >::begin() const noexcept
{
  return { fake_->next_ };
}

template< class T >
hvostov::Liter< T > hvostov::List< T >::end() const noexcept
{
  return { fake_ };
}

template< class T >
hvostov::Liter< T > hvostov::List< T >::insertAfter(const Liter< T > it, const T& val)
{
  Node< T >* n = new Node< T >{val, it.curr_->next_};
  it.curr_->next_ = n;
  return { n };
}

template< class T >
void hvostov::List< T >::eraseAfter(const Liter< T > it) noexcept
{
  if (it.curr_->next_ != fake_) {
    Node< T >* d = it.curr_->next_;
    it.curr_->next_ = d->next_;
    delete d;
  }
}

template< class T >
void hvostov::List< T >::clear() noexcept
{
  Node< T >* h = fake_->next_;
  while (h != fake_) {
    Node< T >* temp = h->next_;
    delete h;
    h = temp;
  }
  fake_->next_ = fake_;
}

template< class T >
bool hvostov::List< T >::empty() const noexcept
{
  return fake_->next_ == fake_;
}

template< class T >
hvostov::Liter< T > hvostov::List< T >::pushBack(const T& val)
{
  Liter< T > it = begin();
  it = insertAfter(it, val);
  return it;
}

template< class T >
hvostov::Liter< T >::Liter() noexcept:
  curr_(nullptr)
{}

template< class T >
hvostov::Liter< T >::Liter(Node< T >* curr) noexcept:
  curr_(curr)
{}

template< class T >
hvostov::Liter< T >::Liter(const Liter< T >& liter) noexcept:
  curr_(liter.curr_)
{}

template< class T >
hvostov::Liter< T >::Liter(Liter< T >&& liter) noexcept:
  curr_(liter.curr_)
{
  liter.curr_ = nullptr;
}

template< class T >
hvostov::Liter< T >& hvostov::Liter< T >::operator=(const Liter< T >& liter) noexcept
{
  if (this != &liter) {
    curr_ = liter.curr_;
  }
  return *this;
}

template< class T >
hvostov::Liter< T >& hvostov::Liter< T >::operator=(Liter< T >&& liter) noexcept
{
  if (this != &liter) {
    curr_ = liter.curr_;
    liter.curr_ = nullptr;
  }
  return *this;
}

template< class T >
hvostov::Liter< T >& hvostov::Liter< T >::operator++() noexcept
{
  curr_ = curr_->next_;
  return *this;
}

template< class T >
hvostov::Liter< T > hvostov::Liter< T >::operator++(int) noexcept
{
  Liter< T > temp = *this;
  ++(*this);
  return temp;
}

template< class T >
T& hvostov::Liter< T >::operator*() const noexcept
{
  return curr_->val_;
}

template< class T >
T* hvostov::Liter< T >::operator->() const noexcept
{
  return &(curr_->val_);
}

template< class T >
bool hvostov::Liter< T >::operator==(const Liter< T >& liter) const noexcept
{
  return curr_ == liter.curr_;
}

template< class T >
bool hvostov::Liter< T >::operator!=(const Liter< T >& liter) const noexcept
{
  return curr_ != liter.curr_;
}

template< class T >
hvostov::LCiter< T >::LCiter() noexcept:
  curr_(nullptr)
{}

template< class T >
hvostov::LCiter< T >::LCiter(Node< T >* curr) noexcept:
  curr_(curr)
{}

template< class T >
hvostov::LCiter< T >::LCiter(const LCiter< T >& lciter) noexcept:
  curr_(lciter.curr_)
{}

template< class T >
hvostov::LCiter< T >::LCiter(LCiter< T >&& lciter) noexcept:
  curr_(lciter.curr_)
{
  lciter.curr_ = nullptr;
}

template< class T >
hvostov::LCiter< T >& hvostov::LCiter< T >::operator=(const LCiter< T >& lciter) noexcept
{
  if (this != &lciter) {
    curr_ = lciter.curr_;
  }
  return *this;
}

template< class T >
hvostov::LCiter< T >& hvostov::LCiter< T >::operator=(LCiter< T >&& lciter) noexcept
{
  if (this != &lciter) {
    curr_ = lciter.curr_;
    lciter.curr_ = nullptr;
  }
  return *this;
}

template< class T >
hvostov::LCiter< T >& hvostov::LCiter< T >::operator++() noexcept
{
  curr_ = curr_->next_;
  return *this;
}

template< class T >
hvostov::LCiter< T > hvostov::LCiter< T >::operator++(int) noexcept
{
  LCiter< T > temp = *this;
  ++(*this);
  return temp;
}

template< class T >
const T& hvostov::LCiter< T >::operator*() const noexcept
{
  return curr_->val_;
}

template< class T >
const T* hvostov::LCiter< T >::operator->() const noexcept
{
  return &(curr_->val_);
}

template< class T >
bool hvostov::LCiter< T >::operator==(const LCiter< T >& lciter) const noexcept
{
  return curr_ == lciter.curr_;
}

template< class T >
bool hvostov::LCiter< T >::operator!=(const LCiter< T >& lciter) const noexcept
{
  return curr_ != lciter.curr_;
}

#endif
