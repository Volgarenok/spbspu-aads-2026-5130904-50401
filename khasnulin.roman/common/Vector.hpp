#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <utility>

#include "VectorRandomIter.hpp"

namespace khasnulin
{

  template < class T > struct Vector
  {
  public:
    ~Vector();
    Vector() noexcept;

    Vector(const Vector< T > &rhs);
    Vector(Vector< T > &&rhs) noexcept;
    Vector(size_t size, const T &init);
    explicit Vector(std::initializer_list< T > il);

    Vector(size_t size);

    Vector(const Vector< T > &rhs, size_t capacity);

    Vector< T > &operator=(const Vector< T > &rhs);
    Vector< T > &operator=(Vector< T > &&rhs) noexcept;

    T &operator[](size_t index) noexcept;
    const T &operator[](size_t index) const noexcept;

    T &at(size_t id);
    const T &at(size_t id) const;

    bool isEmpty() const noexcept;
    size_t getSize() const noexcept;
    size_t getCapacity() const noexcept;

    void popBack();

    void insert(size_t i, const T &val);
    void insert(size_t i, const Vector< T > &rhs, size_t from, size_t to);

    void insert(RAIter< T > pos, size_t count, const T &value);
    void insert(RAIter< T > pos, RAIter< T > begin, RAIter< T > end);
    void insert(RAIter< T > pos, std::initializer_list< T > il);

    // template < class VectorIterator, class FwdIterator >
    // void insert(VectorIterator pos, FwdIterator begin, FwdIterator end);

    void erase(size_t i);
    void erase(size_t from, size_t to);
    RAIter< T > erase(RAIter< T > first, RAIter< T > last);
    RAIter< T > erase(RAIter< T > pos);
    template < class K > void eraseIf(RAIter< T > first, RAIter< T > last, K k);

    void swap(Vector< T > &rhs) noexcept;

    void changeVectorInSomeWay();

    void pushBack(const T &val);
    void pushBack(T &&val);

    void pushBackCount(size_t k, const T &val);
    template < class Val > void unsafePushBack(size_t k, Val &&val);

    template < class IT > void pushBackRange(IT start, size_t count);

    void reserve(size_t new_cap);
    void shrinkToFit();
    void shrinkToFit(size_t size);

    RAIter< T > begin();
    RAIter< T > end();

  private:
    static T *copyTo(T *start, const T *end, T *to);
    static T *createCopy(T *start, const T *end, size_t cap);
    void rangeInsertion(size_t i, const T *rhs, size_t from, size_t to);
    void rangeErasing(size_t from, size_t to);
    size_t calcCapacity(size_t diff);
    static T *fill(T *start, const T *end, const T &val);
    void assignWithLocal(T *data, size_t new_size, size_t new_cap);

    T *data_;
    size_t size_, cap_;
  };

  template < class T > Vector< T >::~Vector()
  {
    delete[] data_;
  }

  template < class T >
  Vector< T >::Vector() noexcept:
      data_(nullptr),
      size_(0),
      cap_(0)
  {
  }

  template < class T >
  Vector< T >::Vector(Vector< T > &&rhs) noexcept:
      data_(rhs.data_),
      size_(rhs.size_),
      cap_(rhs.cap_)
  {
    rhs.data_ = nullptr;
    rhs.size_ = 0;
  }

  template < class T >
  Vector< T >::Vector(const Vector< T > &rhs):
      Vector(rhs.size_)
  {
    for (size_t i = 0; i < rhs.getSize(); ++i)
    {
      data_[i] = rhs[i];
    }
  }

  template < class T >
  Vector< T >::Vector(size_t size, const T &init):
      Vector(size)
  {
    for (size_t i = 0; i < size; ++i)
    {
      data_[i] = init;
    }
  }

  template < class T >
  Vector< T >::Vector(size_t size):
      data_(size ? new T[size] : nullptr),
      size_(size),
      cap_(size)
  {
  }

  template < class T >
  Vector< T >::Vector(const Vector< T > &rhs, size_t capacity):
      Vector(capacity)
  {
    size_ = 0;
    for (size_t i = 0, end = rhs.getSize(); i < end; ++i)
    {
      data_[size_++] = rhs[i];
    }
  }

  template < class T >
  Vector< T >::Vector(std::initializer_list< T > il):
      Vector(il.size())
  {
    size_t i = 0;
    for (auto it = il.begin(); it < il.end(); ++it)
    {
      data_[i++] = *it;
    }
  }

  template < class T > bool Vector< T >::isEmpty() const noexcept
  {
    return size_ == 0;
  }

  template < class T > size_t Vector< T >::getSize() const noexcept
  {
    return size_;
  }

  template < class T > size_t Vector< T >::getCapacity() const noexcept
  {
    return cap_;
  }

  template < class T > void Vector< T >::swap(Vector< T > &rhs) noexcept
  {
    std::swap(data_, rhs.data_);
    std::swap(size_, rhs.size_);
    std::swap(cap_, rhs.cap_);
  }

  template < class T > Vector< T > &Vector< T >::operator=(const Vector< T > &rhs)
  {
    if (this == std::addressof(rhs))
    {
      return *this;
    }
    Vector< T > cpy{rhs};
    swap(cpy);
    return *this;
  }

  template < class T > Vector< T > &Vector< T >::operator=(Vector< T > &&rhs) noexcept
  {
    if (this == std::addressof(rhs))
    {
      return *this;
    }
    // cpy тут для того, чтобы гарантированно удалить то, что было в this
    Vector< T > cpy{std::move(rhs)};
    swap(cpy);
    return *this;
  }

  // Пример использования copy and swap
  template < class T > void Vector< T >::changeVectorInSomeWay()
  {
    Vector< T > cpy(*this);
    cpy.pushBack(1);
    cpy.pushBack(1);
    cpy.pushBack(1);
    cpy.pushBack(1);
    cpy.pushBack(1);
    cpy.pushBack(1);
    cpy.pushBack(1);
    swap(cpy); //  SWAAAAAP NOOOOEXXCEEPT!!!!!!
  }

  template < class T > T *Vector< T >::copyTo(T *start, const T *end, T *to)
  {
    for (; start != end; ++start, ++to)
    {

      *to = *start;
    }
    return to;
  }

  template < class T > T *Vector< T >::fill(T *start, const T *end, const T &val)
  {
    for (; start != end; ++start)
    {
      *start = val;
    }
    return start;
  }

  template < class T > T *Vector< T >::createCopy(T *start, const T *end, size_t cap)
  {
    T *new_data = new T[cap];
    try
    {
      copyTo(start, end, new_data);
    }
    catch (...)
    {
      delete[] new_data;
      throw;
    }
    return new_data;
  }

  template < class T > void Vector< T >::popBack()
  {
    if (!size_)
    {
      throw std::runtime_error("can't pop back, vector size is already zero");
    }
    --size_;
  }

  template < class T > void Vector< T >::insert(size_t i, const T &val)
  {
    if (i > size_)
    {
      throw std::runtime_error("cant't insert element out of array size");
    }
    rangeInsertion(i, &val, 0, 1);
  }

  template < class T > size_t Vector< T >::calcCapacity(size_t diff)
  {
    return size_ + diff < cap_ ? cap_ : std::max((size_ + diff) * 2, cap_ * 2);
  }

  template < class T > void Vector< T >::assignWithLocal(T *data, size_t new_size, size_t new_cap)
  {
    delete[] data_;
    data_ = data;
    size_ = new_size;
    cap_ = new_cap;
  }

  template < class T > void Vector< T >::rangeInsertion(size_t i, const T *rhs, size_t from, size_t to)
  {

    size_t diff = to - from;
    size_t newCapacity = calcCapacity(diff);
    T *new_data = createCopy(data_, data_ + i, newCapacity);
    try
    {
      copyTo(const_cast< T * >(rhs + from), rhs + to, new_data + i);
      copyTo(data_ + i, data_ + size_, new_data + i + diff);
      assignWithLocal(new_data, size_ + diff, newCapacity);
    }
    catch (...)
    {
      delete[] new_data;
      throw;
    }
  }

  template < class T > void Vector< T >::insert(size_t i, const Vector< T > &rhs, size_t from, size_t to)
  {
    if (i > size_)
    {
      throw std::runtime_error("can't insert range to pos bigger than array size");
    }
    if (from > to)
    {
      throw std::runtime_error("the lhs of range is bigger than rhs");
    }
    if (from >= rhs.size_ || to > rhs.size_)
    {
      throw std::runtime_error("can't insert elements out of range in rhs array");
    }
    rangeInsertion(i, rhs.data_, from, to);
  }

  template < class VectorIterator, class FwdIterator >
  void insert(VectorIterator pos, FwdIterator begin, FwdIterator end)
  {
  }

  template < class T > void Vector< T >::insert(RAIter< T > pos, size_t count, const T &value)
  {
    if (pos.getPtr() != this)
    {
      throw std::runtime_error(
          "can't insert count in pos of another vector, pos must refer to the current vector");
    }
    if (!count)
    {
      return;
    }
    size_t new_cap = calcCapacity(count);
    T *new_data = createCopy(data_, data_ + pos.i_, new_cap);
    try
    {
      fill(new_data + pos.i_, new_data + pos.i_ + count, value);
      copyTo(data_ + pos.i_, data_ + size_, new_data + pos.i_ + count);

      assignWithLocal(new_data, size_ + count, new_cap);
    }
    catch (...)
    {
      delete[] new_data;
      throw;
    }
  }

  template < class T > void Vector< T >::insert(RAIter< T > pos, RAIter< T > begin_it, RAIter< T > end_it)
  {
    if (begin_it.getPtr() != end_it.getPtr())
    {
      throw std::runtime_error("can't intsert range from diff vector begin and end iters");
    }
    if (begin_it > end_it)
    {
      throw std::runtime_error("can't insert elems when begin iter more than end iter");
    }
    if (begin_it == end_it)
    {
      return;
    }
    size_t from = begin_it - begin_it.getPtr()->begin();
    size_t to = end_it - begin_it.getPtr()->begin();
    rangeInsertion(pos.i_, begin_it.getPtr()->data_, from, to);
  }

  template < class T > void Vector< T >::insert(RAIter< T > pos, std::initializer_list< T > il)
  {
    if (!il.size())
    {
      return;
    }
    size_t newCap = calcCapacity(il.size());
    T *new_d = createCopy(data_, data_ + pos.i_, newCap);
    try
    {
      size_t i = pos.i_;
      for (const auto &item : il)
      {
        new_d[i++] = item;
      }
      copyTo(data_ + pos.i_, data_ + size_, new_d + i);
      assignWithLocal(new_d, size_ + il.size(), newCap);
    }
    catch (...)
    {
      delete[] new_d;
      throw;
    }
  }

  template < class T > void Vector< T >::rangeErasing(size_t from, size_t to)
  {
    size_t diff = to - from;
    T *new_data = createCopy(data_, data_ + from, cap_);
    try
    {
      copyTo(data_ + to, data_ + size_, new_data + from);
      delete[] data_;
      data_ = new_data;
      size_ -= diff;
    }
    catch (...)
    {
      delete[] new_data;
      throw;
    }
  }

  template < class T > void Vector< T >::erase(size_t i)
  {
    if (i >= size_)
    {
      throw std::runtime_error("can't erase element out of array size");
    }
    rangeErasing(i, i + 1);
  }

  template < class T > void Vector< T >::erase(size_t from, size_t to)
  {
    if (from >= size_ || to > size_)
    {
      throw std::runtime_error("can't erase elements out of range in rhs array");
    }
    if (from > to)
    {
      throw std::runtime_error("can't erase elements range: lhs more than rhs");
    }
    rangeErasing(from, to);
  }

  template < class T > RAIter< T > Vector< T >::erase(RAIter< T > first, RAIter< T > last)
  {
    if (first.getPtr() != last.getPtr())
    {
      throw std::runtime_error("can't intsert range from diff vector begin and end iters");
    }
    if (first > last)
    {
      throw std::runtime_error("can't insert elems when begin iter more than end iter");
    }
    if (first == last)
    {
      return RAIter< T >(this, last.i_);
    }
    size_t last_pos = last.i_ - (first.i_ - last.i_);
    rangeErasing(first.i_, last.i_);
    return RAIter< T >(this, last_pos);
  }

  template < class T > RAIter< T > Vector< T >::erase(RAIter< T > pos)
  {
    if (pos.getPtr() != this)
    {
      throw std::runtime_error("can't erase: iterator refers to another vector");
    }
    size_t last_pos = pos.i_;
    erase(pos.i_);
    return RAIter< T >(this, last_pos);
  }

  template < class T >
  template < class K >
  void Vector< T >::eraseIf(RAIter< T > first, RAIter< T > last, K k)
  {
    if (first.getPtr() != this || last.getPtr() != this)
    {
      throw std::runtime_error("can't erase if: first or last iter refers to another vector");
    }
    if (first > last)
    {
      throw std::runtime_error("can't erase if:begin iter more than end iter");
    }
    if (first == last)
    {
      return;
    }
    size_t start = first.i_;
    size_t end = last.i_;
    T *new_data = createCopy(data_, data_ + start, cap_);
    try
    {
      size_t i = start;
      size_t diff = (last - first);
      size_t count = 0;
      for (; first != last; ++first)
      {
        if (!k(*first))
        {
          new_data[i++] = *first;
          count++;
        }
      }
      diff = diff - count;
      copyTo(data_ + end, data_ + size_, new_data + i);
      assignWithLocal(new_data, size_ - diff, cap_);
    }
    catch (...)
    {
      delete[] new_data;
      throw;
    }
  }

  template < class T > T &Vector< T >::operator[](size_t index) noexcept
  {
    const Vector< T > *cthis = this;
    return const_cast< T & >((*cthis)[index]);
  }

  template < class T > const T &Vector< T >::operator[](size_t index) const noexcept
  {

    return data_[index];
  }

  template < class T > T &Vector< T >::at(size_t id)
  {
    const Vector< T > *cthis = this;
    return const_cast< T & >(cthis->at(id));
  }

  template < class T > const T &Vector< T >::at(size_t id) const
  {
    if (id < getSize())
    {
      return data_[id];
    }
    throw std::out_of_range("can't get element out of range");
  }

  template < class T > bool operator==(const Vector< T > &lhs, const Vector< T > &rhs) noexcept
  {
    bool isEqual = lhs.getSize() == rhs.getSize();
    for (size_t i = 0; i < lhs.getSize() && (isEqual = isEqual && lhs[i] == rhs[i]); ++i)
    {
    }
    return isEqual;
  }

  template < class T > bool operator!=(const Vector< T > &lhs, const Vector< T > &rhs) noexcept
  {
    return !(lhs == rhs);
  }

  template < class T > void Vector< T >::pushBack(const T &val)
  {
    if (size_ >= cap_)
    {
      Vector< T > new_vec{*this, cap_ ? cap_ * 2 : 1};
      new_vec.unsafePushBack(1, val);
      swap(new_vec);
    }
    else
    {
      data_[size_] = val;
      size_++;
    }
  }

  template < class T > void Vector< T >::pushBack(T &&val)
  {
    if (size_ >= cap_)
    {
      Vector< T > new_vec{*this, cap_ ? cap_ * 2 : 1};
      new_vec.unsafePushBack(1, std::move(val));
      swap(new_vec);
    }
    else
    {
      data_[size_] = std::move(val);
      size_++;
    }
  }

  template < class T > void Vector< T >::pushBackCount(size_t k, const T &val)
  {
    if (!k)
    {
      throw std::logic_error("can't push back zero elements");
    }

    Vector< T > new_vec{*this, calcCapacity(k)};
    new_vec.unsafePushBack(k, val);
    swap(new_vec);
  }

  template < class T > template < class Val > void Vector< T >::unsafePushBack(size_t k, Val &&val)
  {
    for (size_t i = 0; i < k; ++i)
    {
      data_[i + size_] = std::forward< Val && >(val);
    }
    size_ += k;
  }

  template < class T > template < class IT > void Vector< T >::pushBackRange(IT start, size_t c)
  {
    if (!c)
    {
      throw std::logic_error("can't push back zero elements range");
    }
    Vector< T > new_vec{*this, calcCapacity(c)};

    size_t i = 0;
    size_t size = new_vec.getSize();
    for (auto it = start; i < c; ++it, ++i)
    {
      new_vec[size + i] = *it;
    }
    new_vec.size_ += c;
    swap(new_vec);
  }

  template < class T > void Vector< T >::reserve(size_t new_cap)
  {
    if (cap_ < new_cap)
    {
      T *new_data = createCopy(data_, data_ + size_, new_cap);
      delete[] data_;
      data_ = new_data;
      cap_ = new_cap;
    }
  }

  template < class T > void Vector< T >::shrinkToFit()
  {
    if (cap_ > size_)
    {
      T *new_data = createCopy(data_, data_ + size_, cap_);
      delete[] data_;
      data_ = new_data;
      cap_ = size_;
    }
  }

  template < class T > RAIter< T > Vector< T >::begin()
  {
    return RAIter< T >{this, 0};
  }

  template < class T > RAIter< T > Vector< T >::end()
  {
    return RAIter< T >{this, size_};
  }
}

#endif
