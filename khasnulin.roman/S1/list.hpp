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
    LIter() noexcept
    {
    }

    LIter< T > &operator++()
    {
      return *this;
    }
    LIter< T > operator++(int)
    {
      return *this;
    }

    LIter< T > &operator--()
    {
      return *this;
    }
    LIter< T > operator--(int)
    {
      return *this;
    }

    bool operator==(const LIter< T > &it) const noexcept
    {
      return false;
    }
    bool operator!=(const LIter< T > &it) const noexcept
    {
      return false;
    }

    T &operator*()
    {
      return *(new T());
    }
    T *operator->()
    {
      return new T();
    }
    friend class BiList< T >;

  private:
    LIter(const LNode< T > *node);

    LNode< T > *curr_;
  };

  template < class T > class LCIter
  {
  public:
    LCIter() noexcept
    {
    }
    LCIter(const LIter< T > &it) noexcept
    {
    }

    LCIter< T > &operator++()
    {
      return *this;
    }
    LCIter< T > operator++(int)
    {
      return *this;
    }

    LCIter< T > &operator--()
    {
      return *this;
    }
    LCIter< T > operator--(int)
    {
      return *this;
    }

    bool operator==(const LCIter< T > &it) const noexcept
    {
      return false;
    }
    bool operator!=(const LCIter< T > &it) const noexcept
    {
      return false;
    }

    const T &operator*()
    {
      return *(new T());
    }
    const T *operator->()
    {
      return new T();
    }

    friend class BiList< T >;

  private:
    LCIter(const LNode< T > *node);

    const LNode< T > *curr_;
  };

  template < class T > class BiList
  {
  public:
    BiList() noexcept
    {
    }

    BiList(const BiList< T > &list)
    {
    }
    BiList(BiList< T > &&list) noexcept
    {
    }

    ~BiList() noexcept
    {
    }

    BiList< T > &operator=(const BiList< T > &list)
    {
      return *(new BiList< T >());
    }
    BiList< T > &operator=(BiList< T > &&list) noexcept
    {
      return *(new BiList< T >());
    }

    LIter< T > begin() noexcept
    {
      return LIter< T >();
    }
    LCIter< T > begin() const noexcept
    {
      return LCIter< T >();
    }
    LCIter< T > cbegin() const noexcept
    {
      return LCIter< T >();
    }

    LIter< T > end() noexcept
    {
      return LIter< T >();
    }
    LCIter< T > end() const noexcept
    {
      return LCIter< T >();
    }
    LCIter< T > cend() noexcept
    {
      return LCIter< T >();
    }

    bool empty() const noexcept
    {
      return false;
    }
    size_t size() const noexcept
    {
      return -1;
    }

    LIter< T > insert(LCIter< T > pos, const T &val)
    {
    }
    LIter< T > insert(LCIter< T > pos, T &&val)
    {
    }

    void push_back(const T &val)
    {
    }
    void push_back(T &&val)
    {
    }

    void clear() noexcept
    {
    }
    void erase(LCIter< T > pos);
    void erase(LCIter< T > begin, LCIter< T > end);

    T &front()
    {
      return *(new T());
    }
    const T &front() const
    {
      return T();
    }

    T &back()
    {
      return *(new T());
    }
    const T &back() const
    {
      return T();
    }

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

  template < typename T > std::ostream &operator<<(std::ostream &os, const LIter< T > &it)
  {
    return os << "LIter";
  }
}

#endif
