#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <utility>

namespace petrov {
  template< typename T >
  struct Node {
    T data_;
    Node< T > * next_;

    Node(const T & data, Node< T > * next = nullptr):
      data_(data),
      next_(next)
    {}
  };

  template< typename T >
  class List {
  public:
    class Iterator {
    public:
      Iterator(Node< T > * node);

      T & operator*();
      T * operator->();
      Iterator & operator++();
      bool operator==(const Iterator & other) const;
      bool operator!=(const Iterator & other) const;

    private:
      Node< T > * current_;
    };

    class ConstIterator {
    public:
      ConstIterator(const Node< T > * node);

      const T & operator*() const;
      const T * operator->() const;
      ConstIterator & operator++();
      bool operator==(const ConstIterator & other) const;
      bool operator!=(const ConstIterator & other) const;

    private:
      const Node< T > * current_;
    };

    List();
    List(const List< T > & other);
    ~List();

    List< T > & operator=(List< T > other);

    void pushBack(const T & value);
    void clear();

    T & getBack();
    const T & getBack() const;

    Iterator begin();
    Iterator end();
    ConstIterator begin() const;
    ConstIterator end() const;

    bool isEmpty() const;
    size_t getSize() const;

  private:
    Node< T > * head_;
    Node< T > * tail_;
    size_t size_;
  };
}
#include "list.cpp"
#endif
