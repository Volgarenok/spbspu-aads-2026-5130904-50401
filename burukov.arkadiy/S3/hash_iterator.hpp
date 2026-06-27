#ifndef HASH_ITERATOR_HPP
#define HASH_ITERATOR_HPP

#include <cstddef>
#include <utility>
#include <memory>

#include <vector.hpp>
#include <list.hpp>

namespace burukov
{
  template< class KeyType, class ValueType, class HashType, class EqualType >
  class HashTable;

  template< class KeyType, class ValueType, class HashType, class EqualType >
  class ConstHashIterator;

  template< class KeyType, class ValueType, class HashType, class EqualType >
  class HashIterator
  {
    friend class HashTable< KeyType, ValueType, HashType, EqualType >;
    friend class ConstHashIterator< KeyType, ValueType, HashType, EqualType >;

  public:
    using EntryType = std::pair< const KeyType, ValueType >;

    HashIterator() = default;

    HashIterator &operator++();

    bool operator==(const HashIterator &other) const;
    bool operator!=(const HashIterator &other) const;

    EntryType &operator*();
    EntryType *operator->();

  private:
    Vector< List< EntryType > > *data_ = nullptr;
    size_t capacity_ = 0;
    size_t bucket_index_ = 0;
    LIter< EntryType > current_position_;
    LIter< EntryType > end_position_;

    HashIterator(Vector< List< EntryType > > *buckets, size_t capacity, size_t startIndex);

    void findValid();
  };

  template< class KeyType, class ValueType, class HashType, class EqualType >
  HashIterator< KeyType, ValueType, HashType, EqualType >::HashIterator(Vector< List< EntryType > > *buckets,
    size_t capacity, size_t startIndex):
    data_(buckets),
    capacity_(capacity),
    bucket_index_(startIndex)
  {
    findValid();
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  void HashIterator< KeyType, ValueType, HashType, EqualType >::findValid()
  {
    while (bucket_index_ < capacity_)
    {
      current_position_ = (*data_)[bucket_index_].begin();
      end_position_ = (*data_)[bucket_index_].end();
      if (current_position_ != end_position_)
      {
        return;
      }
      ++bucket_index_;
    }
    data_ = nullptr;
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  HashIterator< KeyType, ValueType, HashType, EqualType > &
  HashIterator< KeyType, ValueType, HashType, EqualType >::operator++()
  {
    ++current_position_;
    if (current_position_ != end_position_)
    {
      return *this;
    }
    ++bucket_index_;
    findValid();
    return *this;
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  bool HashIterator< KeyType, ValueType, HashType, EqualType >::operator==(const HashIterator &other) const
  {
    if (data_ == nullptr && other.data_ == nullptr)
    {
      return true;
    }
    if (data_ == nullptr || other.data_ == nullptr)
    {
      return false;
    }
    if (bucket_index_ >= capacity_ && other.bucket_index_ >= other.capacity_)
    {
      return true;
    }
    return bucket_index_ == other.bucket_index_ && current_position_ == other.current_position_;
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  bool HashIterator< KeyType, ValueType, HashType, EqualType >::operator!=(const HashIterator &other) const
  {
    return !(*this == other);
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  typename HashIterator< KeyType, ValueType, HashType, EqualType >::EntryType &
  HashIterator< KeyType, ValueType, HashType, EqualType >::operator*()
  {
    return *current_position_;
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  typename HashIterator< KeyType, ValueType, HashType, EqualType >::EntryType *
  HashIterator< KeyType, ValueType, HashType, EqualType >::operator->()
  {
    return std::addressof(*current_position_);
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  class ConstHashIterator
  {
    friend class HashTable< KeyType, ValueType, HashType, EqualType >;

  public:
    using EntryType = std::pair< const KeyType, ValueType >;

    ConstHashIterator() = default;

    ConstHashIterator &operator++();

    bool operator==(const ConstHashIterator &other) const;
    bool operator!=(const ConstHashIterator &other) const;

    const EntryType &operator*() const;
    const EntryType *operator->() const;

  private:
    const Vector< List< EntryType > > *data_ = nullptr;
    size_t capacity_ = 0;
    size_t bucket_index_ = 0;
    LCIter< EntryType > current_position_;
    LCIter< EntryType > end_position_;

    ConstHashIterator(const Vector< List< EntryType > > *buckets, size_t capacity, size_t startIndex);

    void findValid();
  };

  template< class KeyType, class ValueType, class HashType, class EqualType >
  ConstHashIterator< KeyType, ValueType, HashType, EqualType >::ConstHashIterator(
    const Vector< List< EntryType > > *buckets, size_t capacity, size_t startIndex):
    data_(buckets),
    capacity_(capacity),
    bucket_index_(startIndex)
  {
    findValid();
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  void ConstHashIterator< KeyType, ValueType, HashType, EqualType >::findValid()
  {
    while (bucket_index_ < capacity_)
    {
      current_position_ = (*data_)[bucket_index_].cbegin();
      end_position_ = (*data_)[bucket_index_].cend();
      if (current_position_ != end_position_)
      {
        return;
      }
      ++bucket_index_;
    }
    data_ = nullptr;
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  ConstHashIterator< KeyType, ValueType, HashType, EqualType > &
  ConstHashIterator< KeyType, ValueType, HashType, EqualType >::operator++()
  {
    ++current_position_;
    if (current_position_ != end_position_)
    {
      return *this;
    }
    ++bucket_index_;
    findValid();
    return *this;
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  bool ConstHashIterator< KeyType, ValueType, HashType, EqualType >::operator==(
    const ConstHashIterator &other) const
  {
    if (data_ == nullptr && other.data_ == nullptr)
    {
      return true;
    }
    if (data_ == nullptr || other.data_ == nullptr)
    {
      return false;
    }
    if (bucket_index_ >= capacity_ && other.bucket_index_ >= other.capacity_)
    {
      return true;
    }
    return bucket_index_ == other.bucket_index_ && current_position_ == other.current_position_;
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  bool ConstHashIterator< KeyType, ValueType, HashType, EqualType >::operator!=(const ConstHashIterator &other) const
  {
    return !(*this == other);
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  const typename ConstHashIterator< KeyType, ValueType, HashType, EqualType >::EntryType &
  ConstHashIterator< KeyType, ValueType, HashType, EqualType >::operator*() const
  {
    return *current_position_;
  }

  template< class KeyType, class ValueType, class HashType, class EqualType >
  const typename ConstHashIterator< KeyType, ValueType, HashType, EqualType >::EntryType *
  ConstHashIterator< KeyType, ValueType, HashType, EqualType >::operator->() const
  {
    return std::addressof(*current_position_);
  }
}

#endif
