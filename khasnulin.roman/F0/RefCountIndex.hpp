#ifndef REF_COUNT_INDEX_HPP
#define REF_COUNT_INDEX_HPP

#include "BSTree.hpp"
#include <cassert>
#include <cstddef>
#include <utility>

namespace khasnulin
{
  template < class Key > class RefCountIndex
  {
  public:
    using count_type = std::size_t;

    using tree_type = BSTree< Key, count_type >;

    using size_type = typename tree_type::size_type;

    RefCountIndex() = default;

    // true, если ключ новый; строгая гарантия, бросает только bad_alloc;
    bool acquire(const Key &key);
    // предусловие count(key) > 0, проверяется assert; узел удаляется при нуле;
    void release(const Key &key);

    count_type count(const Key &key) const;
    size_type size() const noexcept;
    bool empty() const noexcept;

    void swap(RefCountIndex &rhs) noexcept;
    void clear();

    // по возрастанию ключа, f(key, count), f не должен менять индекс.
    template < class F > F traverse(F f) const;

  private:
    tree_type refs_;
  };

  template < class Key > bool RefCountIndex< Key >::acquire(const Key &key)
  {
    size_type oldSize = refs_.size();
    typename tree_type::iter it = refs_.insert({key, 1});
    if (oldSize < refs_.size())
    {
      return true;
    }
    it->second += 1;
    return false;
  }

  template < class Key > void RefCountIndex< Key >::release(const Key &key)
  {

    typename tree_type::iter it = refs_.find(key);
    assert(it != refs_.end());
    if (it->second == 1)
    {
      refs_.erase(it);
    }
    else
    {
      it->second -= 1;
    }
  }

  template < class Key >
  typename RefCountIndex< Key >::count_type RefCountIndex< Key >::count(const Key &key) const
  {
    typename tree_type::const_iter it = refs_.find(key);
    if (it != refs_.end())
    {
      return it->second;
    }
    return 0;
  }

  template < class Key > typename RefCountIndex< Key >::size_type RefCountIndex< Key >::size() const noexcept
  {
    return refs_.size();
  }

  template < class Key > bool RefCountIndex< Key >::empty() const noexcept
  {
    return refs_.empty();
  }

  template < class Key > void RefCountIndex< Key >::swap(RefCountIndex &rhs) noexcept
  {
    khasnulin::swap(refs_, rhs.refs_);
  }

  template < class Key > void RefCountIndex< Key >::clear()
  {
    refs_.clear();
  }

  template < class Key > template < class F > F RefCountIndex< Key >::traverse(F f) const
  {
    refs_.traverseLNR([&f](const typename tree_type::value_type &val) { return f(val.first, val.second); });
    return f;
  }
}

#endif
