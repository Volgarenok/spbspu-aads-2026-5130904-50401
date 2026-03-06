#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>

namespace khasnulin
{

  template < class T > class List;

  template < class T > class LIter
  {
    friend class List< T >;
  };

  template < class T > class LCIter
  {
    friend class List< T >;
  };

  template < class T > class BiList
  {
  public:
    BiList< T >() noexcept;

    BiList< T >(const BiList< T > &list);
    BiList< T >(BiList< T > &&list) noexcept;

    ~BiList< T >() noexcept;

    BiList< T > &operator=(const BiList< T > &list);
    BiList< T > &operator=(BiList< T > &&list) noexcept;

    LIter< T > begin() noexcept;
    LIter< T > begin() const noexcept;
    LCIter< T > cbegin() const noexcept;

    LIter< T > end() noexcept;
    LIter< T > end() const noexcept;
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

  private:
    List< T > *h_;
    List< T > *t_;
    size_t s_;
  };
}

#endif
