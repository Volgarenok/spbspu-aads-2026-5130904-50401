#ifndef KKHT_CONST_ITERATOR_HPP
#define KKHT_CONST_ITERATOR_HPP

#include "kuku_hash_table.hpp"

namespace petrov
{
  template< class Value, class Key, class Hash, class Equal >
  struct KukuHashTable;

  template< class Value, class Key, class Hash, class Equal >
  struct CKKHTIterator
  {
    using Citer_t = CKKHTIterator< Value,Key, Hash, Equal >;
    using kkht_t = KukuHashTable< Value, Key, Hash, Equal >;
    using node_t = detail::HashNode< Key, Value >;
    friend struct KukuHashTable< Value, Key, Hash, Equal >;

    Citer_t& operator++();
    Citer_t operator++(int);

    bool operator==(const Citer_t& rhs) const;
    bool operator!=(const Citer_t& rhs) const;

    const node_t& operator*() const;
    const node_t* operator->() const;

    private:
      const kkht_t* table_;
      size_t tableNum_;
      size_t idx_;

      CKKHTIterator(const kkht_t* table, size_t tableNum, size_t idx);
      void skipEmpty();
  };

  template< class Value, class Key, class Hash, class Equal >
  CKKHTIterator< Value, Key, Hash, Equal >::CKKHTIterator(const kkht_t* table, size_t tableNum, size_t idx):
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
  CKKHTIterator< Value, Key, Hash, Equal >& CKKHTIterator< Value, Key, Hash, Equal >::operator++()
  {
    ++idx_;
    skipEmpty();
    return *this;
  }

  template< class Value, class Key, class Hash, class Equal >
  CKKHTIterator< Value, Key, Hash, Equal > CKKHTIterator< Value, Key, Hash, Equal >::operator++(int)
  {
    Citer_t tmp = *this;
    ++(*this);
    return tmp;
  }

  template< class Value, class Key, class Hash, class Equal >
  void CKKHTIterator< Value, Key, Hash, Equal >::skipEmpty()
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
  bool CKKHTIterator< Value, Key, Hash, Equal >::operator==(const Citer_t& rhs) const
  {
    return table_ == rhs.table_ && tableNum_ == rhs.tableNum_ && idx_ == rhs.idx_;
  }

  template< class Value, class Key, class Hash, class Equal >
  bool CKKHTIterator< Value, Key, Hash, Equal >::operator!=(const Citer_t& rhs) const
  {
    return !(*this == rhs);
  }

  template< class Value, class Key, class Hash, class Equal >
  const detail::HashNode< Key, Value >&
  CKKHTIterator< Value, Key, Hash, Equal >::operator*() const
  {
    if (tableNum_ == 1)
    {
      return table_->data1_[idx_];
    }
    return table_->data2_[idx_];
  }

  template< class Value, class Key, class Hash, class Equal >
  const detail::HashNode< Key, Value >*
  CKKHTIterator< Value, Key, Hash, Equal >::operator->() const
  {
    return std::addressof(*(*this));
  }
}

#endif
