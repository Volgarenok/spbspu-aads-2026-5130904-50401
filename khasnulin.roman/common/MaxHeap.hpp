#ifndef MAX_HEAP_HPP
#define MAX_HEAP_HPP

#include "Vector.hpp"
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace khasnulin
{
  template < class T, class Compare > class MaxHeap
  {
  public:
    using value_compare = Compare;
    using value_type = T;
    using size_type = size_t;
    using reference = T &;
    using const_reference = const T &;

    // конструктор по умолчанию
    MaxHeap();

    explicit MaxHeap(const Compare &compare);

    value_compare valueComp() const;

    // положить в кучу элемент.
    void push(const_reference value);
    void push(value_type &&value);

    // Убрать элемент из сверху кучи. Если куча пуста -- проброс std::out_of_range
    void pop();

    // Реализуем swap для ADL
    void swap(MaxHeap &rhs) noexcept;

    // Взять элемент с верха кучи, строго по константной ссылке. Если куча пуста -- проброс std::out_of_range
    const_reference top() const;

    // Проверить, пуста ли куча
    bool empty() const noexcept;

    // Вернуть размер кучи
    size_type size() const noexcept;

  private:
    static size_type parent(size_type node) noexcept;
    static size_type leftChild(size_type node) noexcept;
    static size_type rightChild(size_type node) noexcept;

    void siftUp(size_type node);
    void siftDown(size_type node);

    Compare compare_;
    Vector< value_type > heap_;
  };

  template < class T, class Compare >
  MaxHeap< T, Compare >::MaxHeap():
      compare_(),
      heap_()
  {
  }

  template < class T, class Compare >
  MaxHeap< T, Compare >::MaxHeap(const Compare &compare):
      compare_(compare),
      heap_()
  {
  }

  template < class T, class Compare >
  typename MaxHeap< T, Compare >::size_type MaxHeap< T, Compare >::parent(size_type node) noexcept
  {
    if (node != 0)
    {
      return (node - 1) / 2;
    }
    return 0;
  }

  template < class T, class Compare >
  typename MaxHeap< T, Compare >::size_type MaxHeap< T, Compare >::leftChild(size_type node) noexcept
  {
    return (node * 2) + 1;
  }

  template < class T, class Compare >
  typename MaxHeap< T, Compare >::size_type MaxHeap< T, Compare >::rightChild(size_type node) noexcept
  {
    return (node * 2) + 2;
  }

  template < class T, class Compare > void MaxHeap< T, Compare >::siftUp(size_type node)
  {
    size_type par = parent(node);
    while (node > 0 && compare_(heap_[par], heap_[node]))
    {
      std::swap(heap_[par], heap_[node]);
      node = par;
      par = parent(node);
    }
  }

  template < class T, class Compare > void MaxHeap< T, Compare >::push(const_reference value)
  {
    heap_.pushBack(value);
    siftUp(size() - 1);
  }

  template < class T, class Compare > void MaxHeap< T, Compare >::push(value_type &&value)
  {
    heap_.pushBack(std::move(value));
    siftUp(size() - 1);
  }

  template < class T, class Compare >
  typename MaxHeap< T, Compare >::const_reference MaxHeap< T, Compare >::top() const
  {
    if (empty())
    {
      throw std::out_of_range("heap is empty");
    }
    return heap_[0];
  }

  template < class T, class Compare > void MaxHeap< T, Compare >::siftDown(size_type node)
  {
    size_type leftC = leftChild(node);
    size_type rightC = rightChild(node);
    size_type s = size();
    bool swapped = true;
    while (leftC < s && swapped)
    {
      swapped = false;
      size_type maxC = node;
      if (compare_(heap_[node], heap_[leftC]))
      {
        maxC = leftC;
      }

      if (rightC < s && compare_(heap_[maxC], heap_[rightC]))
      {
        maxC = rightC;
      }

      if (maxC != node)
      {
        std::swap(heap_[node], heap_[maxC]);
        swapped = true;
        node = maxC;
        leftC = leftChild(node);
        rightC = rightChild(node);
      }
    }
  }

  template < class T, class Compare > void MaxHeap< T, Compare >::pop()
  {
    if (empty())
    {
      throw std::out_of_range("Can't pop from empty heap");
    }
    std::swap(heap_[0], heap_[size() - 1]);
    heap_.popBack();
    siftDown(0);
  }

  template < class T, class Compare > bool MaxHeap< T, Compare >::empty() const noexcept
  {
    return heap_.isEmpty();
  }

  template < class T, class Compare >
  typename MaxHeap< T, Compare >::size_type MaxHeap< T, Compare >::size() const noexcept
  {
    return heap_.getSize();
  }

  template < class T, class Compare > void MaxHeap< T, Compare >::swap(MaxHeap &rhs) noexcept
  {
    std::swap(compare_, rhs.compare_);
    std::swap(heap_, rhs.heap_);
  }
  template < class T, class Compare >
  typename MaxHeap< T, Compare >::value_compare MaxHeap< T, Compare >::valueComp() const
  {
    return compare_;
  }
}

#endif
