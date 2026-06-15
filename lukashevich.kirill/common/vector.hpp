#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cassert>
#include <cstddef>
#include <new>
#include <stdexcept>
#include <utility>

namespace lukashevich {
  template< class T >
  class Vector {
  public:
    using iterator = T *;
    using const_iterator = const T *;

    Vector();
    explicit Vector(size_t count);
    Vector(size_t count, const T & value);
    Vector(const Vector & rhs);
    Vector(Vector && rhs) noexcept;
    ~Vector();

    Vector & operator=(const Vector & rhs);
    Vector & operator=(Vector && rhs) noexcept;

    void pushBack(const T & value);
    void popBack();
    void erase(size_t index);
    void clear() noexcept;
    void reserve(size_t capacity);
    void swap(Vector & rhs) noexcept;

    T & operator[](size_t index);
    const T & operator[](size_t index) const;

    size_t getSize() const noexcept;
    size_t getCapacity() const noexcept;
    bool isEmpty() const noexcept;

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

  private:
    static T * allocate(size_t capacity);
    static void deallocate(T * data) noexcept;
    static void destroyRange(T * data, size_t begin, size_t end) noexcept;
    static size_t nextCapacity(size_t needed);

    T * data_;
    size_t size_;
    size_t capacity_;
  };
}

#endif
