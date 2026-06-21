#ifndef KKHT_ITERATOR_HPP
#define KKHT_ITERATOR_HPP

#include "kuku_hash_table.hpp"

namespace petrov
{
  template< class Value, class Key, class Hash, class Equal >
  struct KukuHashTable;

  template< class Value, class Key, class Hash, class Equal >
  struct KKHTIterator
  {
    using iter_t = KKHTIterator< Value,Key, Hash, Equal >;
    using kkht_t = KukuHashTable< Value, Key, Hash, Equal >;
    using node_t = detail::HashNode< Key, Value >;
    friend struct KukuHashTable< Value, Key, Hash, Equal >;

    iter_t& operator++();
    iter_t operator++(int);

    bool operator==(const iter_t& rhs) const;
    bool operator!=(const iter_t& rhs) const;

    node_t& operator*() const;
    node_t* operator->() const;

    private:
      kkht_t* table_;
      size_t tableNum_;
      size_t idx_;

      KKHTIterator(kkht_t* table, size_t tableNum, size_t idx);
      void skipEmpty();
  };

  template< class Value, class Key, class Hash, class Equal >
  KKHTIterator< Value, Key, Hash, Equal >::KKHTIterator(kkht_t* table, size_t tableNum, size_t idx):
    table_(table),
    tableNum_(tableNum),
    idx_(idx)
  {
    if (tableNum_ != 1 && tableNum_ != 2)
    {
      throw std::runtime_error("Table number invalid");
    }
  }

  template< class Value, class Key, class Hash, class Equal >
  KKHTIterator< Value, Key, Hash, Equal >& KKHTIterator< Value, Key, Hash, Equal >::operator++()
  {
    ++idx_;
    skipEmpty();
    return *this;
  }

  template< class Value, class Key, class Hash, class Equal >
  KKHTIterator< Value, Key, Hash, Equal > KKHTIterator< Value, Key, Hash, Equal >::operator++(int)
  {
    iter_t tmp = *this;
    ++(*this);
    return tmp;
  }

  template< class Value, class Key, class Hash, class Equal >
  void KKHTIterator< Value, Key, Hash, Equal >::skipEmpty()
  {
    if (tableNum_ == 1)
    {
      for (; idx_ < table_->data1_.getSize(); ++idx_)
      {
        if (table_->data1_[idx_].state_ == detail::OCCUPIED)
        {
          return;
        }
      }
      tableNum_ = 2;
      idx_ = 0;
    }

    if (tableNum_ == 2)
    {
      for (; idx_ < table_->data2_.getSize(); ++idx_)
      {
        if (table_->data2_[idx_].state_ == detail::OCCUPIED)
        {
          return;
        }
      }
    }
  }

  template< class Value, class Key, class Hash, class Equal >
  bool KKHTIterator< Value, Key, Hash, Equal >::operator==(const iter_t& rhs) const
  {
    return table_ == rhs.table_ && tableNum_ == rhs.tableNum_ && idx_ == rhs.idx_;
  }

  template< class Value, class Key, class Hash, class Equal >
  bool KKHTIterator< Value, Key, Hash, Equal >::operator!=(const iter_t& rhs) const
  {
    return !(*this == rhs);
  }

  template< class Value, class Key, class Hash, class Equal >
  detail::HashNode< Key, Value >&
  KKHTIterator< Value, Key, Hash, Equal >::operator*() const
  {
    if (tableNum_ == 1)
    {
      return table_->data1_[idx_];
    }
    return table_->data2_[idx_];
  }

  template< class Value, class Key, class Hash, class Equal >
  detail::HashNode< Key, Value >*
  KKHTIterator< Value, Key, Hash, Equal >::operator->() const
  {
    return std::addressof(*(*this));
  }
}

#endif
