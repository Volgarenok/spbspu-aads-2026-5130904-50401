#ifndef AUTO_HASH_TABLE_HPP
#define AUTO_HASH_TABLE_HPP

#include "HashTable.hpp"
#include <stdexcept>
#include <utility>

namespace khasnulin
{
  template < class Key, class Value, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  class AutoHashTable
  {
  public:
    using key_type = Key;

    using mapped_type = Value;

    using public_value_type = std::pair< const Key, Value >;

    using size_type = std::size_t;

    using iterator = HIter< Key, Value, Hash, Equal >;
    using const_iterator = CHIter< Key, Value, Hash, Equal >;

    explicit AutoHashTable(size_type initial_buckets = 16);

    // default rule of 5 == rule of zero
    AutoHashTable(const AutoHashTable &rhs) = default;
    AutoHashTable(AutoHashTable &&rhs) noexcept = default;

    AutoHashTable &operator=(const AutoHashTable &rhs) = default;
    AutoHashTable &operator=(AutoHashTable &&rhs) noexcept = default;

    ~AutoHashTable() noexcept = default;

    // Вставить элемент, если уже существует -- перезаписать
    template < class V > void add(const Key &key, V &&value);

    // Получить элемент по ключу. Если элемента нет -- создать Value{} и вернуть
    Value &operator[](const Key &key);

    // Найти по ключу и вернуть указатель на значение. Если нет ключа -- возврат nullptr. (Колхоз, но для
    // быстрого решения подойдет)
    Value *find(const Key &key);
    const Value *find(const Key &key) const;

    bool has(const Key &key) const;

    // Если удалось удалить -- возвращаем true, иначе false. Без исключений
    bool erase(const Key &key);

    size_type size() const noexcept;
    bool empty() const noexcept;

    size_type bucketSize() const noexcept;

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    void clear();

    void swap(AutoHashTable &rhs) noexcept;

  private:
    static constexpr size_type MAX_GROW_ATTEMPTS = 8;
    bool needGrow() const noexcept;
    void grow();

    HashTable< Key, Value, Hash, Equal > hashTable_;
  };

  template < class Key, class Value, class Hash, class Equal >
  AutoHashTable< Key, Value, Hash, Equal >::AutoHashTable(size_type initial_buckets):
      hashTable_(initial_buckets)
  {
  }

  template < class Key, class Value, class Hash, class Equal >
  bool AutoHashTable< Key, Value, Hash, Equal >::needGrow() const noexcept
  {
    return hashTable_.average_bucket_size() >= 3.0;
  }

  template < class Key, class Value, class Hash, class Equal >
  void AutoHashTable< Key, Value, Hash, Equal >::grow()
  {
    size_type currBucketsCount = bucketSize();
    // нет проверки на overflow осознанно, не рассчитываю на большой Max Grow Attempts
    size_type multiplier = 2;
    bool caught = true;
    for (size_type i = 0; i < MAX_GROW_ATTEMPTS && caught; ++i)
    {
      try
      {
        hashTable_.rehash(currBucketsCount * multiplier);
        caught = false;
      }
      catch (const std::overflow_error &)
      {
        caught = true;
        multiplier *= 2;
      }
    }
    if (caught)
    {
      throw std::overflow_error("auto hash table can't grow after all grow attempts");
    }
  }

  template < class Key, class Value, class Hash, class Equal >
  template < class V >
  void AutoHashTable< Key, Value, Hash, Equal >::add(const Key &key, V &&value)
  {
    try
    {
      if (find(key) == nullptr && needGrow())
      {
        grow();
      }
    }
    catch (const std::overflow_error &)
    {
    }
    try
    {
      hashTable_.add(key, std::forward< V >(value));
    }
    catch (const std::overflow_error &)
    {
      grow();
      hashTable_.add(key, std::forward< V >(value));
    }
  }

  template < class Key, class Value, class Hash, class Equal >
  Value &AutoHashTable< Key, Value, Hash, Equal >::operator[](const Key &key)
  {
    Value *result = find(key);
    if (result)
    {
      return *result;
    }
    add(key, Value{});
    return *find(key);
  }

  template < class Key, class Value, class Hash, class Equal >
  Value *AutoHashTable< Key, Value, Hash, Equal >::find(const Key &key)
  {
    iterator it = hashTable_.find(key);
    return it == hashTable_.end() ? nullptr : &(*it).second;
  }

  template < class Key, class Value, class Hash, class Equal >
  const Value *AutoHashTable< Key, Value, Hash, Equal >::find(const Key &key) const
  {
    const_iterator it = hashTable_.find(key);
    return it == hashTable_.end() ? nullptr : &(*it).second;
  }

  template < class Key, class Value, class Hash, class Equal >
  bool AutoHashTable< Key, Value, Hash, Equal >::has(const Key &key) const
  {
    return hashTable_.has(key);
  }

  template < class Key, class Value, class Hash, class Equal >
  bool AutoHashTable< Key, Value, Hash, Equal >::erase(const Key &key)
  {
    if (find(key) == nullptr)
    {
      return false;
    }
    hashTable_.drop(key);
    return true;
  }

  template < class Key, class Value, class Hash, class Equal >
  typename AutoHashTable< Key, Value, Hash, Equal >::size_type
  AutoHashTable< Key, Value, Hash, Equal >::size() const noexcept
  {
    return hashTable_.size();
  }

  template < class Key, class Value, class Hash, class Equal >
  bool AutoHashTable< Key, Value, Hash, Equal >::empty() const noexcept
  {
    return hashTable_.empty();
  }

  template < class Key, class Value, class Hash, class Equal >
  typename AutoHashTable< Key, Value, Hash, Equal >::size_type
  AutoHashTable< Key, Value, Hash, Equal >::bucketSize() const noexcept
  {
    return hashTable_.bucket_size();
  }

  template < class Key, class Value, class Hash, class Equal >
  typename AutoHashTable< Key, Value, Hash, Equal >::iterator
  AutoHashTable< Key, Value, Hash, Equal >::begin() noexcept
  {
    return hashTable_.begin();
  }

  template < class Key, class Value, class Hash, class Equal >
  typename AutoHashTable< Key, Value, Hash, Equal >::iterator
  AutoHashTable< Key, Value, Hash, Equal >::end() noexcept
  {
    return hashTable_.end();
  }

  template < class Key, class Value, class Hash, class Equal >
  typename AutoHashTable< Key, Value, Hash, Equal >::const_iterator
  AutoHashTable< Key, Value, Hash, Equal >::begin() const noexcept
  {
    return hashTable_.cbegin();
  }

  template < class Key, class Value, class Hash, class Equal >
  typename AutoHashTable< Key, Value, Hash, Equal >::const_iterator
  AutoHashTable< Key, Value, Hash, Equal >::end() const noexcept
  {
    return hashTable_.cend();
  }

  template < class Key, class Value, class Hash, class Equal >
  typename AutoHashTable< Key, Value, Hash, Equal >::const_iterator
  AutoHashTable< Key, Value, Hash, Equal >::cbegin() const noexcept
  {
    return hashTable_.cbegin();
  }

  template < class Key, class Value, class Hash, class Equal >
  typename AutoHashTable< Key, Value, Hash, Equal >::const_iterator
  AutoHashTable< Key, Value, Hash, Equal >::cend() const noexcept
  {
    return hashTable_.cend();
  }

  template < class Key, class Value, class Hash, class Equal >
  void AutoHashTable< Key, Value, Hash, Equal >::clear()
  {
    hashTable_ = HashTable< Key, Value, Hash, Equal >(hashTable_.bucket_size());
  }

  template < class Key, class Value, class Hash, class Equal >
  void AutoHashTable< Key, Value, Hash, Equal >::swap(AutoHashTable &rhs) noexcept
  {
    khasnulin::swap(hashTable_, rhs.hashTable_);
  }

}

#endif
