#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>

namespace khasnulin
{

  template < class T > class BiList;

  template < class T > struct LNode;

  template < class T > class LIter
  {
  public:
    LIter< T >() noexcept;

    LIter< T > &operator++();
    LIter< T > operator++(int);

    LIter< T > &operator--();
    LIter< T > operator--(int);

    bool operator==(const LIter< T > &it) const noexcept;
    bool operator!=(const LIter< T > &it) const noexcept;

    T &operator*();
    T *operator->();
    friend class BiList< T >;

  private:
    LIter< T >(const LNode< T > *node);

    LNode< T > *curr_;
  };

  template < class T > class LCIter
  {
  public:
    LCIter< T >() noexcept;
    explicit LCIter< T >(LIter< T > it) noexcept;

    LCIter< T > &operator++();
    LCIter< T > operator++(int);

    LCIter< T > &operator--();
    LCIter< T > operator--(int);

    bool operator==(const LCIter< T > &it) const noexcept;
    bool operator!=(const LCIter< T > &it) const noexcept;

    const T &operator*();
    const T *operator->();

    friend class BiList< T >;

  private:
    LCIter< T >(const LNode< T > *node);

    const LNode< T > *curr_;
  };

  template < class T > class BiList
  {
  public:
    BiList< T >() noexcept;

    BiList< T >(const BiList< T > &list);
    BiList< T >(BiList< T > &&list) noexcept;

    ~BiList() noexcept;

    BiList< T > &operator=(const BiList< T > &list);
    BiList< T > &operator=(BiList< T > &&list) noexcept;

    LIter< T > begin() noexcept;
    LCIter< T > begin() const noexcept;
    LCIter< T > cbegin() const noexcept;

    LIter< T > end() noexcept;
    LCIter< T > end() const noexcept;
    LCIter< T > cend() noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

    LIter< T > insert(LCIter< T > pos, const T &val);
    LIter< T > insert(LCIter< T > pos, T &&val);

    void push_back(const T &val);
    void push_back(T &&val);

    void clear() noexcept;
    void erase(LCIter< T > pos);
    void erase(LCIter< T > begin, LCIter< T > end);

    T &front();
    const T &front() const;

    T &back();
    const T &back() const;

    // TODO: to the future version
    template < class... Args > LIter< T > emplace(LCIter< T > pos, Args &&...args);

    friend class LIter< T >;
    friend class LCIter< T >;

  private:
    template < class K > struct LNode
    {
      K val;
      LNode< K > *next;
      LNode< K > *prev;
    };
    LNode< T > *h_;
    LNode< T > *t_;
    size_t s_;
  };
}

#endif
