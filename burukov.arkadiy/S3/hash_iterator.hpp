#ifndef HASH_ITERATOR_HPP
#define HASH_ITERATOR_HPP

#include <vector.hpp>
#include <list.hpp>
#include <cstddef>
#include <utility>

namespace burukov
{
  template<typename KeyType, typename ValueType, typename HashType, typename EqualType>
  class HashTable;

  template<typename KeyType, typename ValueType, typename HashType, typename EqualType>
  class ConstHashIterator;

  template<typename KeyType, typename ValueType, typename HashType, typename EqualType>
  class HashIterator
  {
    friend class HashTable<KeyType, ValueType, HashType, EqualType>;
    friend class ConstHashIterator<KeyType, ValueType, HashType, EqualType>;
    using EntryType = std::pair<const KeyType, ValueType>;

  public:
    HashIterator() = default;

    HashIterator &operator++()
    {
      ++current_position_;
      if (current_position_ != end_position_)
        return *this;
      ++bucket_index_;
      findValid();
      return *this;
    }

    bool operator==(const HashIterator &other) const
    {
      if (data_ == nullptr && other.data_ == nullptr)
        return true;
      if (data_ == nullptr || other.data_ == nullptr)
        return false;
      if (bucket_index_ >= capacity_ && other.bucket_index_ >= other.capacity_)
        return true;
      return bucket_index_ == other.bucket_index_ && current_position_ == other.current_position_;
    }

    bool operator!=(const HashIterator &other) const
    {
      return !(*this == other);
    }

    EntryType &operator*() const
    {
      LIter<EntryType> it = current_position_;
      return *it;
    }

    EntryType *operator->() const
    {
      LIter<EntryType> it = current_position_;
      return it.operator->();
    }

  private:
    HashIterator(Vector<List<EntryType>> *buckets, size_t capacity, size_t start_index)
      : data_(buckets), capacity_(capacity), bucket_index_(start_index)
    {
      findValid();
    }

    void findValid()
    {
      while (bucket_index_ < capacity_)
      {
        current_position_ = (*data_)[bucket_index_].begin();
        end_position_ = (*data_)[bucket_index_].end();
        if (current_position_ != end_position_)
          return;
        ++bucket_index_;
      }
      data_ = nullptr;
    }

    Vector<List<EntryType>> *data_ = nullptr;
    size_t capacity_ = 0;
    size_t bucket_index_ = 0;
    LIter<EntryType> current_position_;
    LIter<EntryType> end_position_;
  };

  template<typename KeyType, typename ValueType, typename HashType, typename EqualType>
  class ConstHashIterator
  {
    friend class HashTable<KeyType, ValueType, HashType, EqualType>;
    using EntryType = std::pair<const KeyType, ValueType>;

  public:
    ConstHashIterator() = default;

    ConstHashIterator &operator++()
    {
      ++current_position_;
      if (current_position_ != end_position_)
        return *this;
      ++bucket_index_;
      findValid();
      return *this;
    }

    bool operator==(const ConstHashIterator &other) const
    {
      if (data_ == nullptr && other.data_ == nullptr)
        return true;
      if (data_ == nullptr || other.data_ == nullptr)
        return false;
      if (bucket_index_ >= capacity_ && other.bucket_index_ >= other.capacity_)
        return true;
      return bucket_index_ == other.bucket_index_ && current_position_ == other.current_position_;
    }

    bool operator!=(const ConstHashIterator &other) const
    {
      return !(*this == other);
    }

    const EntryType &operator*() const
    {
      return *current_position_;
    }

    const EntryType *operator->() const
    {
      return current_position_.operator->();
    }

  private:
    ConstHashIterator(const Vector<List<EntryType>> *buckets, size_t capacity, size_t start_index)
      : data_(buckets), capacity_(capacity), bucket_index_(start_index)
    {
      findValid();
    }

    void findValid()
    {
      while (bucket_index_ < capacity_)
      {
        current_position_ = (*data_)[bucket_index_].cbegin();
        end_position_ = (*data_)[bucket_index_].cend();
        if (current_position_ != end_position_)
          return;
        ++bucket_index_;
      }
      data_ = nullptr;
    }

    const Vector<List<EntryType>> *data_ = nullptr;
    size_t capacity_ = 0;
    size_t bucket_index_ = 0;
    LCIter<EntryType> current_position_;
    LCIter<EntryType> end_position_;
  };
}

#endif
