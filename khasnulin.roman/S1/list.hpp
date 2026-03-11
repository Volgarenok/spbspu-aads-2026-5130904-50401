#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <iostream>

namespace khasnulin
{

  template < class T > class BiList;

  template < class T > struct LNode;

  template < class T > class LIter
  {
  public:
    LIter() noexcept;

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
    LIter(const LNode< T > *node);

    LNode< T > *curr_;
  };

  template < class T > class LCIter
  {
  public:
    LCIter() noexcept;
    LCIter(const LIter< T > &it) noexcept;

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
    LCIter(const LNode< T > *node);

    const LNode< T > *curr_;
  };

  template < class T > class BiList
  {
  public:
    BiList() noexcept;

    BiList(const BiList< T > &list);
    BiList(BiList< T > &&list) noexcept;

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
    size_t s_;
  };

  template < typename T > std::ostream &operator<<(std::ostream &os, const LIter< T > &it)
  {
    return os << "LIter";
  }

  template < class T > BiList< T >::BiList() noexcept
  {
  }

  template < class T > BiList< T >::BiList(const BiList< T > &list)
  {
  }
  template < class T > BiList< T >::BiList(BiList< T > &&list) noexcept
  {
  }

  template < class T > BiList< T >::~BiList() noexcept
  {
  }

  template < class T > BiList< T > &BiList< T >::operator=(const BiList< T > &list)
  {
    return *(new BiList< T >());
  }

  template < class T > BiList< T > &BiList< T >::operator=(BiList< T > &&list) noexcept
  {
    return *(new BiList< T >());
  }

  template < class T > LIter< T > BiList< T >::begin() noexcept
  {
    return LIter< T >();
  }
  template < class T > LCIter< T > BiList< T >::begin() const noexcept
  {
    return LCIter< T >();
  }
  template < class T > LCIter< T > BiList< T >::cbegin() const noexcept
  {
    return LCIter< T >();
  }

  template < class T > LIter< T > BiList< T >::end() noexcept
  {
    return LIter< T >();
  }
  template < class T > LCIter< T > BiList< T >::end() const noexcept
  {
    return LCIter< T >();
  }
  template < class T > LCIter< T > BiList< T >::cend() noexcept
  {
    return LCIter< T >();
  }

  template < class T > bool BiList< T >::empty() const noexcept
  {
    return false;
  }
  template < class T > size_t BiList< T >::size() const noexcept
  {
    return -1;
  }

  template < class T > LIter< T > BiList< T >::insert(LCIter< T > pos, const T &val)
  {
  }
  template < class T > LIter< T > BiList< T >::insert(LCIter< T > pos, T &&val)
  {
  }

  template < class T > void BiList< T >::push_back(const T &val)
  {
  }
  template < class T > void BiList< T >::push_back(T &&val)
  {
  }

  template < class T > void BiList< T >::clear() noexcept
  {
  }
  template < class T > void BiList< T >::erase(LCIter< T > pos)
  {
  }
  template < class T > void BiList< T >::erase(LCIter< T > begin, LCIter< T > end)
  {
  }

  template < class T > T &BiList< T >::front()
  {
    return *(new T());
  }
  template < class T > const T &BiList< T >::front() const
  {
    return T();
  }

  template < class T > T &BiList< T >::back()
  {
    return *(new T());
  }
  template < class T > const T &BiList< T >::back() const
  {
    return T();
  }

  template < class T > LCIter< T >::LCIter() noexcept
  {
  }

  template < class T > LCIter< T >::LCIter(const LIter< T > &it) noexcept
  {
  }

  template < class T > LCIter< T > &LCIter< T >::operator++()
  {
    return *this;
  }
  template < class T > LCIter< T > LCIter< T >::operator++(int)
  {
    return *this;
  }

  template < class T > LCIter< T > &LCIter< T >::operator--()
  {
    return *this;
  }

  template < class T > LCIter< T > LCIter< T >::operator--(int)
  {
    return *this;
  }

  template < class T > bool LCIter< T >::operator==(const LCIter< T > &it) const noexcept
  {
    return false;
  }
  template < class T > bool LCIter< T >::operator!=(const LCIter< T > &it) const noexcept
  {
    return false;
  }

  template < class T > const T &LCIter< T >::operator*()
  {
    return *(new T());
  }
  template < class T > const T *LCIter< T >::operator->()
  {
    return new T();
  }

  template < class T > LIter< T >::LIter() noexcept
  {
  }

  template < class T > LIter< T > &LIter< T >::operator++()
  {
    return *this;
  }
  template < class T > LIter< T > LIter< T >::operator++(int)
  {
    return *this;
  }

  template < class T > LIter< T > &LIter< T >::operator--()
  {
    return *this;
  }
  template < class T > LIter< T > LIter< T >::operator--(int)
  {
    return *this;
  }

  template < class T > bool LIter< T >::operator==(const LIter< T > &it) const noexcept
  {
    return false;
  }
  template < class T > bool LIter< T >::operator!=(const LIter< T > &it) const noexcept
  {
    return false;
  }

  template < class T > T &LIter< T >::operator*()
  {
    return *(new T());
  }
  template < class T > T *LIter< T >::operator->()
  {
    return new T();
  }
}

#endif
