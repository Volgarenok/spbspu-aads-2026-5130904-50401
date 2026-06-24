#ifndef ROBIN_HOOD_TABLE_HPP
#define ROBIN_HOOD_TABLE_HPP

#include <functional>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include "hmac_hash.hpp"
#include "vector.hpp"

namespace tarasenko
{
  template< class Key, class Value, class Hash = HmacHash< Key >, class Equal = std::equal_to< Key > >
  struct RobinHoodTable;

  template< class Key, class Value, class Hash = HmacHash< Key >, class Equal = std::equal_to< Key > >
  struct RobinHoodConstIterator;

  template< class Key, class Value, class Hash = HmacHash< Key >, class Equal = std::equal_to< Key > >
  struct RobinHoodIterator
  {
    RobinHoodIterator() noexcept;

    std::pair< Key, Value >& operator*() const;
    std::pair< Key, Value >* operator->() const;

    RobinHoodIterator& operator++();
    RobinHoodIterator operator++(int);

    bool operator==(const RobinHoodIterator& rhs) const noexcept;
    bool operator!=(const RobinHoodIterator& rhs) const noexcept;

  private:
    RobinHoodTable< Key, Value, Hash, Equal >* table_;
    size_t index_;

    explicit RobinHoodIterator(RobinHoodTable< Key, Value, Hash, Equal >* table, size_t index) noexcept;

    friend struct RobinHoodTable< Key, Value, Hash, Equal >;
    friend struct RobinHoodConstIterator< Key, Value, Hash, Equal >;
  };

  template< class Key, class Value, class Hash, class Equal >
  struct RobinHoodConstIterator
  {
    RobinHoodConstIterator() noexcept;
    RobinHoodConstIterator(const RobinHoodIterator< Key, Value, Hash, Equal >& rhs) noexcept;

    const std::pair< Key, Value >& operator*() const;
    const std::pair< Key, Value >* operator->() const;

    RobinHoodConstIterator& operator++();
    RobinHoodConstIterator operator++(int);

    bool operator==(const RobinHoodConstIterator& rhs) const noexcept;
    bool operator!=(const RobinHoodConstIterator& rhs) const noexcept;

  private:
    const RobinHoodTable< Key, Value, Hash, Equal >* table_;
    size_t index_;

    explicit RobinHoodConstIterator(const RobinHoodTable< Key, Value, Hash, Equal >* table, size_t index) noexcept;

    friend struct RobinHoodTable< Key, Value, Hash, Equal >;
  };

  template< class Key, class Value, class Hash, class Equal >
  struct RobinHoodTable
  {

    RobinHoodTable();
    explicit RobinHoodTable(size_t bucket_count);
    RobinHoodTable(size_t bucket_count, const Hash& hash, const Equal& equal = Equal());

    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t bucket_count() const noexcept;
    double load_factor() const noexcept;
    double max_load_factor() const noexcept;
    void max_load_factor(double ml);

    void clear();
    void reserve(size_t new_capacity);
    void rehash(size_t new_bucket_count);

    bool insert(const std::pair< const Key, Value >& value);
    size_t erase(const Key& key);
    size_t count(const Key& key) const;

    RobinHoodIterator< Key, Value, Hash, Equal > find(const Key& key) noexcept;
    RobinHoodConstIterator< Key, Value, Hash, Equal > find(const Key& key) const noexcept;

    Value& at(const Key& key);
    const Value& at(const Key& key) const;
    Value& operator[](const Key& key);

    RobinHoodIterator< Key, Value, Hash, Equal > begin() noexcept;
    RobinHoodIterator< Key, Value, Hash, Equal > end() noexcept;
    RobinHoodConstIterator< Key, Value, Hash, Equal > begin() const noexcept;
    RobinHoodConstIterator< Key, Value, Hash, Equal > end() const noexcept;
    RobinHoodConstIterator< Key, Value, Hash, Equal > cbegin() const noexcept;
    RobinHoodConstIterator< Key, Value, Hash, Equal > cend() const noexcept;

  private:
    friend struct RobinHoodIterator< Key, Value, Hash, Equal >;
    friend struct RobinHoodConstIterator< Key, Value, Hash, Equal >;

    struct Bucket
    {
      Bucket();
      Bucket(const Key& key, const Value& value, size_t distance);

      std::pair< Key, Value > data;
      size_t distance;
      bool occupied;
    };

    Vector< Bucket > buckets_;
    size_t size_;
    Hash hash_;
    Equal equal_;
    double max_load_factor_;

  };

  #define ht_template template< class Key, class Value, class Hash, class Equal >
  #define ht_type RobinHoodTable< Key, Value, Hash, Equal >
  #define ht_iterator RobinHoodIterator< Key, Value, Hash, Equal >
  #define ht_const_iterator RobinHoodConstIterator< Key, Value, Hash, Equal >

  ht_template
  ht_iterator::RobinHoodIterator() noexcept:
    table_(nullptr),
    index_(0)
  {}

  ht_template
  ht_iterator::RobinHoodIterator(ht_type* table, size_t index) noexcept:
    table_(table),
    index_(index)
  {
    while (table_ && index_ < table_->buckets_.getSize() && !table_->buckets_[index_].occupied)
    {
      ++index_;
    }
  }

  ht_template
  std::pair< Key, Value >& ht_iterator::operator*() const
  {
    return table_->buckets_[index_].data;
  }

  ht_template
  std::pair< Key, Value >* ht_iterator::operator->() const
  {
    return std::addressof(table_->buckets_[index_].data);
  }

  ht_template
  ht_iterator& ht_iterator::operator++()
  {
    ++index_;
    while (index_ < table_->buckets_.getSize() && !table_->buckets_[index_].occupied)
    {
      ++index_;
    }
    return *this;
  }

  ht_template
  ht_iterator ht_iterator::operator++(int)
  {
    ht_iterator copy(*this);
    ++(*this);
    return copy;
  }

  ht_template
  bool ht_iterator::operator==(const ht_iterator& rhs) const noexcept
  {
    return table_ == rhs.table_ && index_ == rhs.index_;
  }

  ht_template
  bool ht_iterator::operator!=(const ht_iterator& rhs) const noexcept
  {
    return !(*this == rhs);
  }

  ht_template
  ht_const_iterator::RobinHoodConstIterator() noexcept:
    table_(nullptr),
    index_(0)
  {}

  ht_template
  ht_const_iterator::RobinHoodConstIterator(const ht_iterator& rhs) noexcept:
    table_(rhs.table_),
    index_(rhs.index_)
  {}

  ht_template
  ht_const_iterator::RobinHoodConstIterator(const ht_type* table, size_t index) noexcept:
    table_(table),
    index_(index)
  {
    while (table_ && index_ < table_->buckets_.getSize() && !table_->buckets_[index_].occupied)
    {
      ++index_;
    }
  }

  ht_template
  const std::pair< Key, Value >& ht_const_iterator::operator*() const
  {
    return table_->buckets_[index_].data;
  }

  ht_template
  const std::pair< Key, Value >* ht_const_iterator::operator->() const
  {
    return std::addressof(table_->buckets_[index_].data);
  }

  ht_template
  ht_const_iterator& ht_const_iterator::operator++()
  {
    ++index_;
    while (index_ < table_->buckets_.getSize() && !table_->buckets_[index_].occupied)
    {
      ++index_;
    }
    return *this;
  }

  ht_template
  ht_const_iterator ht_const_iterator::operator++(int)
  {
    ht_const_iterator copy(*this);
    ++(*this);
    return copy;
  }

  ht_template
  bool ht_const_iterator::operator==(const ht_const_iterator& rhs) const noexcept
  {
    return table_ == rhs.table_ && index_ == rhs.index_;
  }

  ht_template
  bool ht_const_iterator::operator!=(const ht_const_iterator& rhs) const noexcept
  {
    return !(*this == rhs);
  }

  ht_template
  ht_type::Bucket::Bucket():
    data(),
    distance(0),
    occupied(false)
  {}

  ht_template
  ht_type::Bucket::Bucket(const Key& key, const Value& value, size_t distance):
    data(key, value),
    distance(distance),
    occupied(true)
  {}

  ht_template
  ht_type::RobinHoodTable():
    RobinHoodTable(8)
  {}

  ht_template
  ht_type::RobinHoodTable(size_t bucket_count):
    RobinHoodTable(bucket_count, Hash(), Equal())
  {}

  ht_template
  ht_type::RobinHoodTable(size_t bucket_count, const Hash& hash, const Equal& equal):
    buckets_(bucket_count ? bucket_count : 1, Bucket()),
    size_(0),
    hash_(hash),
    equal_(equal),
    max_load_factor_(0.75)
  {}

  ht_template
  bool ht_type::empty() const noexcept
  {
    return size_ == 0;
  }

  ht_template
  size_t ht_type::size() const noexcept
  {
    return size_;
  }

  ht_template
  size_t ht_type::bucket_count() const noexcept
  {
    return buckets_.getSize();
  }

  ht_template
  double ht_type::load_factor() const noexcept
  {
    return static_cast< double >(size_) / bucket_count();
  }

  ht_template
  double ht_type::max_load_factor() const noexcept
  {
    return max_load_factor_;
  }

  ht_template
  void ht_type::max_load_factor(double ml)
  {
    if (ml <= 0)
    {
      throw std::invalid_argument("bad load factor");
    }
    max_load_factor_ = ml;
  }

  ht_template
  void ht_type::clear()
  {
    for (size_t i = 0; i < buckets_.getSize(); ++i)
    {
      buckets_[i].occupied = false;
      buckets_[i].distance = 0;
    }
    size_ = 0;
  }

  ht_template
  void ht_type::reserve(size_t new_capacity)
  {
    size_t required = static_cast< size_t >(new_capacity / max_load_factor_) + 1;
    if (required > bucket_count())
    {
      rehash(required);
    }
  }

  ht_template
  void ht_type::rehash(size_t new_bucket_count)
  {
    if (new_bucket_count == 0)
    {
      new_bucket_count = 1;
    }

    Vector< Bucket > old_buckets(buckets_);
    buckets_ = Vector< Bucket >(new_bucket_count, Bucket());
    size_ = 0;
    for (size_t i = 0; i < old_buckets.getSize(); ++i)
    {
      if (old_buckets[i].occupied)
      {
        insert({old_buckets[i].data.first, old_buckets[i].data.second});
      }
    }
  }

  ht_template
  bool ht_type::insert(const std::pair< const Key, Value >& value)
  {
    if (find(value.first) != end())
    {
      return false;
    }
    if (static_cast< double >(size_ + 1) / bucket_count() > max_load_factor_)
    {
      rehash(bucket_count() * 2);
    }

    size_t index = hash_(value.first) % bucket_count();
    Bucket bucket(value.first, value.second, 0);
    while (true)
    {
      Bucket& current = buckets_[index];
      if (!current.occupied)
      {
        current = bucket;
        ++size_;
        return true;
      }
      if (equal_(current.data.first, bucket.data.first))
      {
        return false;
      }
      if (current.distance < bucket.distance)
      {
        std::swap(current, bucket);
      }
      index = (index + 1) % bucket_count();
      ++bucket.distance;
    }
  }

  ht_template
  size_t ht_type::erase(const Key& key)
  {
    ht_iterator it = find(key);
    if (it == end())
    {
      return 0;
    }

    size_t index = it.index_;
    size_t next = (index + 1) % bucket_count();
    while (buckets_[next].occupied && buckets_[next].distance > 0)
    {
      buckets_[index] = buckets_[next];
      --buckets_[index].distance;
      index = next;
      next = (next + 1) % bucket_count();
    }
    buckets_[index].occupied = false;
    buckets_[index].distance = 0;
    --size_;
    return 1;
  }

  ht_template
  ht_iterator ht_type::find(const Key& key) noexcept
  {
    if (bucket_count() == 0)
    {
      return end();
    }
    size_t index = hash_(key) % bucket_count();
    size_t distance = 0;
    while (buckets_[index].occupied && buckets_[index].distance >= distance)
    {
      if (equal_(buckets_[index].data.first, key))
      {
        return ht_iterator(this, index);
      }
      index = (index + 1) % bucket_count();
      ++distance;
    }
    return end();
  }

  ht_template
  ht_const_iterator ht_type::find(const Key& key) const noexcept
  {
    if (bucket_count() == 0)
    {
      return end();
    }
    size_t index = hash_(key) % bucket_count();
    size_t distance = 0;
    while (buckets_[index].occupied && buckets_[index].distance >= distance)
    {
      if (equal_(buckets_[index].data.first, key))
      {
        return ht_const_iterator(this, index);
      }
      index = (index + 1) % bucket_count();
      ++distance;
    }
    return end();
  }

  ht_template
  size_t ht_type::count(const Key& key) const
  {
    return find(key) == end() ? 0 : 1;
  }

  ht_template
  Value& ht_type::at(const Key& key)
  {
    ht_iterator it = find(key);
    if (it == end())
    {
      throw std::out_of_range("key not found");
    }
    return it->second;
  }

  ht_template
  const Value& ht_type::at(const Key& key) const
  {
    ht_const_iterator it = find(key);
    if (it == end())
    {
      throw std::out_of_range("key not found");
    }
    return it->second;
  }

  ht_template
  Value& ht_type::operator[](const Key& key)
  {
    ht_iterator it = find(key);
    if (it == end())
    {
      insert({key, Value()});
      it = find(key);
    }
    return it->second;
  }

  ht_template
  ht_iterator ht_type::begin() noexcept
  {
    return ht_iterator(this, 0);
  }

  ht_template
  ht_iterator ht_type::end() noexcept
  {
    return ht_iterator(this, buckets_.getSize());
  }

  ht_template
  ht_const_iterator ht_type::begin() const noexcept
  {
    return ht_const_iterator(this, 0);
  }

  ht_template
  ht_const_iterator ht_type::end() const noexcept
  {
    return ht_const_iterator(this, buckets_.getSize());
  }

  ht_template
  ht_const_iterator ht_type::cbegin() const noexcept
  {
    return begin();
  }

  ht_template
  ht_const_iterator ht_type::cend() const noexcept
  {
    return end();
  }

  #undef ht_template
  #undef ht_type
  #undef ht_iterator
  #undef ht_const_iterator
}

#endif
