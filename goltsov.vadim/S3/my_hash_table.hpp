#ifndef MY_HASH_TABLE_HPP
#define MY_HASH_TABLE_HPP
#include <cstddef>
#include <stdexcept>
#include <mylist.hpp>
#include <boost/uuid/detail/sha1.hpp>
#include <utility>

namespace goltsov
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable;
  template< class Key, class Value, class Hash, class Equal >
  class HashTableConstIterator;
  template< class Key, class Value, class Hash, class Equal >
  class HashTableIterator;
}

namespace goltsov
{
  namespace detail
  {
    template< class Key, class Value >
    struct NodeHashTable
    {
      std::pair< Key, Value > data;
      bool is_valid;
      NodeHashTable();
      NodeHashTable(const Key& k, const Value& v, bool valid);
    };
    template< class Key, class Value >
    struct Bucket
    {
      detail::NodeHashTable< Key, Value >* node;
      size_t capacity;
      Bucket();
      Bucket(size_t capacity);
      ~Bucket();
      void init(size_t capacity);
    };
    template< class Key, class Value, class Hash, class Equal >
    HashTableIterator< Key, Value, Hash, Equal > makeInconstantHashTableIterator(
      HashTableConstIterator< Key, Value, Hash, Equal >);
    template< class T >
    LIter< T > makeInconstantLIter(LCIter< T >);
  }
}

namespace goltsov
{
  template< class T >
  struct Sha1Hasher
  {
    size_t operator()(const T& key) const;
  };
  template <>
  struct Sha1Hasher< std::string >
  {
    size_t operator()(const std::string& key) const;
  };
  template <>
  struct Sha1Hasher< std::pair< std::string, std::string > >
  {
    size_t operator()(const std::pair< std::string, std::string >& key) const;
  };
}

namespace goltsov
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTableIterator
  {
  public:
    HashTableIterator() noexcept;
    HashTableIterator(HashTable < Key, Value, Hash, Equal >*, size_t, size_t,
      LIter< detail::NodeHashTable< Key, Value > >) noexcept;
    std::pair< Key, Value >* operator->() noexcept;
    std::pair< Key, Value >& operator*() noexcept;
    HashTableIterator< Key, Value, Hash, Equal >& operator++();
    HashTableIterator< Key, Value, Hash, Equal > operator++(int);
    operator HashTableConstIterator< Key, Value, Hash, Equal >() const noexcept;
  private:
    HashTable < Key, Value, Hash, Equal >* hash_table_;
    size_t ind_;
    size_t ind_Bucket_;
    LIter< detail::NodeHashTable< Key, Value > > overflow_iterator_;
    HashTableIterator< Key, Value, Hash, Equal > next() const;
    bool hasNext() const noexcept;
    template< class K, class V, class H, class E >
    friend bool operator==(const HashTableIterator< K, V, H, E >&, const HashTableIterator< K, V, H, E >&);
    template< class K, class V, class H, class E >
    friend bool operator!=(const HashTableIterator< K, V, H, E >&, const HashTableIterator< K, V, H, E >&);
    template< class K, class V, class H, class E >
    friend bool operator==(const HashTableIterator< K, V, H, E >&, const HashTableConstIterator< K, V, H, E >&);
    template <class K, class V, class H, class E >
    friend bool operator!=(const HashTableIterator< K, V, H, E >&, const HashTableConstIterator< K, V, H, E >&);
    template< class K, class V, class H, class E >
    friend bool operator==(const HashTableConstIterator< K, V, H, E >&, const HashTableIterator< K, V, H, E >&);
    template< class K, class V, class H, class E >
    friend bool operator!=(const HashTableConstIterator< K, V, H, E >&, const HashTableIterator< K, V, H, E >&);
    friend HashTableIterator< Key, Value, Hash, Equal >
      detail::makeInconstantHashTableIterator< Key, Value, Hash, Equal >(
      HashTableConstIterator< Key, Value, Hash, Equal > it);
  };

  template< class Key, class Value, class Hash, class Equal >
  class HashTableConstIterator
  {
  public:
    HashTableConstIterator() noexcept;
    HashTableConstIterator(const HashTable < Key, Value, Hash, Equal >*, size_t, size_t,
      LCIter< detail::NodeHashTable< Key, Value > >) noexcept;
    const std::pair< Key, Value >* operator->() noexcept;
    const std::pair< Key, Value >& operator*() noexcept;
    HashTableConstIterator< Key, Value, Hash, Equal >& operator++();
    HashTableConstIterator< Key, Value, Hash, Equal > operator++(int);
  private:
    const HashTable< Key, Value, Hash, Equal >* hash_table_;
    size_t ind_;
    size_t ind_Bucket_;
    mutable LCIter< detail::NodeHashTable< Key, Value > > overflow_iterator_;
    HashTableConstIterator< Key, Value, Hash, Equal > next() const;
    bool hasNext() const noexcept;
    friend class HashTableIterator< Key, Value, Hash, Equal >;
    template< class K, class V, class H, class E >
    friend bool operator==(const HashTableConstIterator< K, V, H, E >&, const HashTableConstIterator< K, V, H, E >&);
    template< class K, class V, class H, class E >
    friend bool operator!=(const HashTableConstIterator< K, V, H, E >&, const HashTableConstIterator< K, V, H, E >&);
    template< class K, class V, class H, class E >
    friend bool operator==(const HashTableIterator< K, V, H, E >&, const HashTableConstIterator< K, V, H, E >&);
    template< class K, class V, class H, class E >
    friend bool operator!=(const HashTableIterator< K, V, H, E >&, const HashTableConstIterator< K, V, H, E >&);
    template< class K, class V, class H, class E >
    friend bool operator==(const HashTableConstIterator< K, V, H, E >&, const HashTableIterator< K, V, H, E >&);
    template< class K, class V, class H, class E >
    friend bool operator!=(const HashTableConstIterator< K, V, H, E >&, const HashTableIterator< K, V, H, E >&);
    friend HashTableIterator< Key, Value, Hash, Equal >
      detail::makeInconstantHashTableIterator< Key, Value, Hash, Equal >(
      HashTableConstIterator< Key, Value, Hash, Equal >);
  };

  template< class Key, class Value, class Hash, class Equal >
  class HashTable
  {
  public:
    HashTable();
    HashTable(const HashTable< Key, Value, Hash, Equal >&);
    HashTable(HashTable< Key, Value, Hash, Equal >&&);
    HashTable(const size_t&, const size_t&);
    ~HashTable();
    HashTable< Key, Value, Hash, Equal >& operator=(const HashTable< Key, Value, Hash, Equal >&);
    HashTable< Key, Value, Hash, Equal >& operator=(HashTable< Key, Value, Hash, Equal >&&);
    void swap(HashTable< Key, Value, Hash, Equal >&) noexcept;
    template< class TypeKey, class TypeValue >
    std::pair< HashTableIterator< Key, Value, Hash, Equal >, bool > insert(std::pair< TypeKey, TypeValue >&&);
    size_t erase(const Key&);
    void rehash(const size_t&, const size_t&);
    void clear();
    size_t size() const noexcept;
    size_t count() const noexcept;
    size_t countBuckets() const noexcept;
    size_t capacity() const noexcept;
    bool contains(const Key&) const noexcept;
    Value& at(const Key&);
    const Value& at(const Key&) const;
    HashTableIterator< Key, Value, Hash, Equal > find(const Key&);
    HashTableConstIterator< Key, Value, Hash, Equal > find(const Key&) const;
    Value& operator[](const Key&);
    const Value& operator[](const Key&) const;
    HashTableIterator< Key, Value, Hash, Equal > begin();
    HashTableIterator< Key, Value, Hash, Equal > end();
    HashTableConstIterator< Key, Value, Hash, Equal > cbegin() const;
    HashTableConstIterator< Key, Value, Hash, Equal > cend() const;

    size_t getSizeOverflowBucket() const noexcept;
    double getAvgCountInBucket() const noexcept;

    void setMaxSizeOverflow(const size_t&) noexcept;
    void setMaxAvgCountInBucket(const double&) noexcept;
  private:
    friend class HashTableConstIterator< Key, Value, Hash, Equal >;
    friend class HashTableIterator< Key, Value, Hash, Equal >;
    size_t count_valid_;
    size_t count_buckets_;
    size_t size_bucket_;
    size_t max_size_overflow_;
    double max_avg_count_in_bucket_;
    detail::Bucket< Key, Value >* data_;
    List< detail::NodeHashTable< Key, Value > > overflow_;
    bool checkAndRebuild();
  };

  inline size_t upd_buckets(const size_t&);
  inline size_t upd_bucket_size(const size_t&);

  template< class Key, class Value, class Hash, class Equal >
  bool operator==(const HashTableIterator< Key, Value, Hash, Equal >& lhs,
    const HashTableIterator< Key, Value, Hash, Equal >& rhs);
  template< class Key, class Value, class Hash, class Equal >
  bool operator!=(const HashTableIterator< Key, Value, Hash, Equal >& lhs,
    const HashTableIterator< Key, Value, Hash, Equal >& rhs);
  template< class Key, class Value, class Hash, class Equal >
  bool operator==(const HashTableConstIterator< Key, Value, Hash, Equal >& lhs,
    const HashTableConstIterator< Key, Value, Hash, Equal >& rhs);
  template< class Key, class Value, class Hash, class Equal >
  bool operator!=(const HashTableConstIterator< Key, Value, Hash, Equal >& lhs,
    const HashTableConstIterator< Key, Value, Hash, Equal >& rhs);
  template< class Key, class Value, class Hash, class Equal >
  bool operator==(const HashTableIterator< Key, Value, Hash, Equal >& lhs,
    const HashTableConstIterator< Key, Value, Hash, Equal >& rhs);
  template< class Key, class Value, class Hash, class Equal >
  bool operator!=(const HashTableIterator< Key, Value, Hash, Equal >& lhs,
    const HashTableConstIterator< Key, Value, Hash, Equal >& rhs);
  template< class Key, class Value, class Hash, class Equal >
  bool operator==(const HashTableConstIterator< Key, Value, Hash, Equal >& lhs,
    const HashTableIterator< Key, Value, Hash, Equal >& rhs);
  template< class Key, class Value, class Hash, class Equal >
  bool operator!=(const HashTableConstIterator< Key, Value, Hash, Equal >& lhs,
    const HashTableIterator< Key, Value, Hash, Equal >& rhs);
}

template< class T >
size_t goltsov::Sha1Hasher< T >::operator()(const T& key) const
{
  boost::uuids::detail::sha1 sha1;
  sha1.process_bytes(&key, sizeof(T));
  unsigned char digest[20];
  sha1.get_digest(digest);
  size_t hash = 0;
  for (size_t i = 0; i < sizeof(size_t); ++i)
  {
    hash = (hash << 8) | digest[i];
  }
  return hash;
}
inline size_t goltsov::Sha1Hasher< std::string >::operator()(const std::string& key) const
{
  boost::uuids::detail::sha1 sha1;
  sha1.process_bytes(key.data(), key.size());
  unsigned char digest[20];
  sha1.get_digest(digest);
  size_t hash = 0;
  for (size_t i = 0; i < sizeof(size_t); ++i)
  {
    hash = (hash << 8) | digest[i];
  }
  return hash;
}
inline size_t goltsov::Sha1Hasher< std::pair< std::string, std::string > >::operator()(
  const std::pair< std::string, std::string >& key) const
{
  boost::uuids::detail::sha1 sha1;
  sha1.process_bytes(key.first.data(), key.first.size());
  char separator = '\0';
  sha1.process_bytes(&separator, 1);
  sha1.process_bytes(key.second.data(), key.second.size());
  unsigned char digest[20];
  sha1.get_digest(digest);
  size_t hash = 0;
  for (size_t i = 0; i < sizeof(size_t); ++i)
  {
    hash = (hash << 8) | digest[i];
  }
  return hash;
}
template< class Key, class Value >
goltsov::detail::NodeHashTable< Key, Value >::NodeHashTable():
  is_valid(false)
{}
template< class Key, class Value >
goltsov::detail::NodeHashTable< Key, Value >::NodeHashTable(const Key& k, const Value& v, bool valid):
  data({k, v}),
  is_valid(valid)
{}
template< class Key, class Value >
goltsov::detail::Bucket< Key, Value >::Bucket():
  node(nullptr),
  capacity(0)
{}
template< class Key, class Value >
goltsov::detail::Bucket< Key, Value >::Bucket(size_t other_capacity):
  node(new detail::NodeHashTable< Key, Value >[other_capacity]),
  capacity(other_capacity)
{}
template< class Key, class Value >
void goltsov::detail::Bucket< Key, Value >::init(size_t other_capacity)
{
  delete[] node;
  capacity = 0;
  node = new detail::NodeHashTable< Key, Value >[other_capacity];
  capacity = other_capacity;
}
template< class Key, class Value >
goltsov::detail::Bucket< Key, Value >::~Bucket()
{
  delete[] node;
  node = nullptr;
  capacity = 0;
}
template< class Key, class Value, class Hash, class Equal >
goltsov::HashTable< Key, Value, Hash, Equal >::HashTable()
try:
  count_valid_(0),
  count_buckets_(0),
  size_bucket_(0),
  max_size_overflow_(0),
  max_avg_count_in_bucket_(0.0),
  data_(nullptr),
  overflow_(List< detail::NodeHashTable< Key, Value > >())
{
  data_ = new detail::Bucket< Key, Value >[1];
  count_buckets_ = 1;
  data_[0].init(1);
  size_bucket_ = 1;
  max_size_overflow_ = count_buckets_;
  max_avg_count_in_bucket_ = 0.75 * static_cast< double >(size_bucket_);
}
catch (...)
{
  delete[] data_;
  data_ = nullptr;
  throw;
}
template< class Key, class Value, class Hash, class Equal >
goltsov::HashTable< Key, Value, Hash, Equal >::~HashTable()
{
  delete[] data_;
  data_ = nullptr;
}
template< class Key, class Value, class Hash, class Equal >
goltsov::HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable< Key, Value, Hash, Equal >& other)
try:
  count_valid_(0),
  count_buckets_(0),
  size_bucket_(0),
  max_size_overflow_(0),
  max_avg_count_in_bucket_(0.0),
  data_(nullptr),
  overflow_(List< detail::NodeHashTable< Key, Value > >())
{
  HashTable< Key, Value, Hash, Equal > new_table(other.count_buckets_, other.size_bucket_);
  for (HashTableConstIterator< Key, Value, Hash, Equal > it = other.cbegin(); it != other.cend(); ++it)
  {
    new_table.insert(std::pair< Key, Value >{it->first, it->second});
  }
  swap(new_table);
}
catch (...)
{
  delete[] data_;
  data_ = nullptr;
  throw;
}
template< class Key, class Value, class Hash, class Equal >
goltsov::HashTable< Key, Value, Hash, Equal >::HashTable(HashTable< Key, Value, Hash, Equal >&& other)
try:
  count_valid_(0),
  count_buckets_(0),
  size_bucket_(0),
  max_size_overflow_(0),
  max_avg_count_in_bucket_(0.0),
  data_(nullptr),
  overflow_(List< detail::NodeHashTable< Key, Value > >())
{
  swap(other);
}
catch (...)
{
  delete[] data_;
  data_ = nullptr;
  throw;
}
template< class Key, class Value, class Hash, class Equal >
goltsov::HashTable< Key, Value, Hash, Equal >&
  goltsov::HashTable< Key, Value, Hash, Equal >::operator=(const HashTable< Key, Value, Hash, Equal >& other)
{
  HashTable< Key, Value, Hash, Equal > new_table(other);
  swap(new_table);
  return *this;
}
template< class Key, class Value, class Hash, class Equal >
goltsov::HashTable< Key, Value, Hash, Equal >&
  goltsov::HashTable< Key, Value, Hash, Equal >::operator=(HashTable< Key, Value, Hash, Equal >&& other)
{
  HashTable< Key, Value, Hash, Equal > new_table(std::move(other));
  swap(new_table);
  return *this;
}
template< class Key, class Value, class Hash, class Equal >
goltsov::HashTable< Key, Value, Hash, Equal >::HashTable(const size_t& count_buckets, const size_t& size_bucket)
try:
  count_valid_(0),
  count_buckets_(0),
  size_bucket_(0),
  max_size_overflow_(0),
  max_avg_count_in_bucket_(0.0),
  data_(nullptr),
  overflow_(List< detail::NodeHashTable< Key, Value > >())
{
  data_ = new detail::Bucket< Key, Value >[count_buckets];
  count_buckets_ = count_buckets;
  for (size_t i = 0; i < count_buckets; ++i)
  {
    data_[i].init(size_bucket);
  }
  size_bucket_ = size_bucket;
  max_size_overflow_ = count_buckets;
  max_avg_count_in_bucket_ = 0.75 * static_cast< double >(size_bucket);
}
catch (...)
{
  delete[] data_;
  data_ = nullptr;
  throw;
}
template< class Key, class Value, class Hash, class Equal >
void goltsov::HashTable< Key, Value, Hash, Equal >::swap(HashTable< Key, Value, Hash, Equal >& other) noexcept
{
  if (this != &other)
  {
    std::swap(count_valid_, other.count_valid_);
    std::swap(count_buckets_, other.count_buckets_);
    std::swap(size_bucket_, other.size_bucket_);
    std::swap(data_, other.data_);
    std::swap(max_size_overflow_, other.max_size_overflow_);
    std::swap(max_avg_count_in_bucket_, other.max_avg_count_in_bucket_);
    overflow_.swap(other.overflow_);
  }
}
template< class Key, class Value, class Hash, class Equal >
template< class TypeKey, class TypeValue >
std::pair< goltsov::HashTableIterator< Key, Value, Hash, Equal >, bool >
  goltsov::HashTable< Key, Value, Hash, Equal >::insert(std::pair< TypeKey, TypeValue >&& data)
{
  Hash hasher;
  Equal e;
  size_t ind = hasher(data.first) % count_buckets_;
  size_t i_in_bucket = size_bucket_;
  for (size_t i = 0; i < size_bucket_; ++i)
  {
    if (data_[ind].node[i].is_valid && e(data_[ind].node[i].data.first, data.first))
    {
      return {HashTableIterator< Key, Value, Hash, Equal >(this, ind, i,
        LIter< detail::NodeHashTable< Key, Value > >()), false};
    }
    else if (!data_[ind].node[i].is_valid && i_in_bucket == size_bucket_)
    {
      i_in_bucket = i;
    }
  }
  LIter< detail::NodeHashTable< Key, Value > > it_now = overflow_.begin();
  LIter< detail::NodeHashTable< Key, Value > > it_now_prev = it_now;
  LIter< detail::NodeHashTable< Key, Value > > i_in_list = overflow_.end();
  while (it_now != overflow_.end())
  {
    if ((*it_now).is_valid && e(it_now->data.first, data.first))
    {
      return {HashTableIterator< Key, Value, Hash, Equal >(this, countBuckets(), 0, it_now), false};
    }
    if (!(*it_now).is_valid && i_in_list == overflow_.end())
    {
      i_in_list = it_now;
    }
    ++it_now;
    if (it_now != overflow_.end())
    {
      it_now_prev = it_now;
    }
  }
  if (i_in_bucket != size_bucket_)
  {
    data_[ind].node[i_in_bucket].data = std::forward< std::pair< TypeKey, TypeValue > >(data);
    data_[ind].node[i_in_bucket].is_valid = true;
    count_valid_++;
    if (!checkAndRebuild())
    {
      return {HashTableIterator< Key, Value, Hash, Equal >(this, ind, i_in_bucket,
        LIter< detail::NodeHashTable< Key, Value > >()), true};
    }
    else
    {
      return {find(data.first), true};
    }
  }
  else if (i_in_list != overflow_.end())
  {
    (*i_in_list).data = std::forward< std::pair< TypeKey, TypeValue > >(data);
    (*i_in_list).is_valid = true;
    count_valid_++;
    if (!checkAndRebuild())
    {
      return {HashTableIterator< Key, Value, Hash, Equal >(this, countBuckets(), 0, i_in_list), true};
    }
    else
    {
      return {find(data.first), true};
    }
  }
  else
  {
    if (it_now_prev == overflow_.end())
    {
      it_now_prev = overflow_.beforeBegin();
    }
    it_now = overflow_.insertAfter(it_now_prev, detail::NodeHashTable< Key, Value >(
      std::forward< std::pair< TypeKey, TypeValue > >(data).first,
      std::forward< std::pair< TypeKey, TypeValue > >(data).second, true));
    count_valid_++;
    if (!checkAndRebuild())
    {
      return {HashTableIterator< Key, Value, Hash, Equal >(this, countBuckets(), 0, it_now), true};
    }
    else
    {
      return {find(data.first), true};
    }
  }
}
template< class Key, class Value, class Hash, class Equal >
size_t goltsov::HashTable< Key, Value, Hash, Equal >::erase(const Key& key)
{
  Hash hasher;
  Equal e;
  size_t ind = hasher(key) % count_buckets_;
  for (size_t i = 0; i < size_bucket_; ++i)
  {
    if (data_[ind].node[i].is_valid && e(data_[ind].node[i].data.first, key))
    {
      data_[ind].node[i].is_valid = false;
      count_valid_--;
      return 1;
    }
  }
  LIter< detail::NodeHashTable< Key, Value > > it_now = overflow_.begin();
  while (it_now != overflow_.end())
  {
    if ((*it_now).is_valid && e((*it_now).data.first, key))
    {
      (*it_now).is_valid = false;
      count_valid_--;
      return 1;
    }
    ++it_now;
  }
  return 0;
}
template< class Key, class Value, class Hash, class Equal >
void goltsov::HashTable< Key, Value, Hash, Equal >::rehash(const size_t& new_count_buckets,
  const size_t& new_size_bucket)
{
  HashTable< Key, Value, Hash, Equal > new_table(new_count_buckets, new_size_bucket);
  for (HashTableIterator< Key, Value, Hash, Equal > it = begin(); it != end(); ++it)
  {
    new_table.insert(std::pair< Key, Value >{it->first, it->second});
  }
  this->swap(new_table);
}
template< class Key, class Value, class Hash, class Equal >
void goltsov::HashTable< Key, Value, Hash, Equal >::clear()
{
  if (count_buckets_ == 0)
  {
    count_buckets_ = 1;
  }
  delete[] data_;
  data_ = new detail::Bucket< Key, Value >[count_buckets_];
  for (size_t i = 0; i < count_buckets_; ++i)
  {
    data_[i].init(size_bucket_);
  }
  overflow_.clear();
  count_valid_ = 0;
}
template< class Key, class Value, class Hash, class Equal >
size_t goltsov::HashTable< Key, Value, Hash, Equal >::size() const noexcept
{
  return count_buckets_ * size_bucket_;
}
template< class Key, class Value, class Hash, class Equal >
size_t goltsov::HashTable< Key, Value, Hash, Equal >::count() const noexcept
{
  return count_valid_;
}
template< class Key, class Value, class Hash, class Equal >
size_t goltsov::HashTable< Key, Value, Hash, Equal >::countBuckets() const noexcept
{
  return count_buckets_;
}
template< class Key, class Value, class Hash, class Equal >
size_t goltsov::HashTable< Key, Value, Hash, Equal >::capacity() const noexcept
{
  return size_bucket_;
}
template< class Key, class Value, class Hash, class Equal >
bool goltsov::HashTable< Key, Value, Hash, Equal >::contains(const Key& key) const noexcept
{
  Hash hasher;
  Equal e;
  if (count_buckets_ == 0)
  {
    return false;
  }
  size_t ind = hasher(key) % count_buckets_;
  for (size_t i = 0; i < size_bucket_; ++i)
  {
    if (data_[ind].node[i].is_valid && e(key, data_[ind].node[i].data.first))
    {
      return true;
    }
  }
  LCIter< detail::NodeHashTable< Key, Value > > it = overflow_.cbegin();
  while (it != overflow_.cend())
  {
    if ((*it).is_valid && e(key, (*it).data.first))
    {
      return true;
    }
    ++it;
  }
  return false;
}
template< class Key, class Value, class Hash, class Equal >
Value& goltsov::HashTable< Key, Value, Hash, Equal >::at(const Key& key)
{
  return const_cast< Value& >(static_cast< const HashTable& >(*this).at(key));;
}
template< class Key, class Value, class Hash, class Equal >
const Value& goltsov::HashTable< Key, Value, Hash, Equal >::at(const Key& key) const
{
  HashTableConstIterator< Key, Value, Hash, Equal > i = find(key);
  if (i != cend())
  {
    return i->second;
  }
  else
  {
    throw std::logic_error("No key in table");
  }
}
template< class Key, class Value, class Hash, class Equal >
goltsov::HashTableIterator< Key, Value, Hash, Equal >
  goltsov::HashTable< Key, Value, Hash, Equal >::find(const Key& key)
{
  return detail::makeInconstantHashTableIterator(
    (static_cast< const HashTable< Key, Value, Hash, Equal > >(*this)).find(key)
  );
}
template< class Key, class Value, class Hash, class Equal >
goltsov::HashTableConstIterator< Key, Value, Hash, Equal >
  goltsov::HashTable< Key, Value, Hash, Equal >::find(const Key& key) const
{
  Hash hasher;
  Equal e;
  if (count_buckets_ == 0)
  {
    return cend();
  }
  size_t ind = hasher(key) % count_buckets_;
  for (size_t i = 0; i < size_bucket_; ++i)
  {
    if (data_[ind].node[i].is_valid && e(key, data_[ind].node[i].data.first))
    {
      return HashTableConstIterator< Key, Value, Hash, Equal >(this,
        ind, i, LCIter< detail::NodeHashTable< Key, Value > >());
    }
  }
  LCIter< detail::NodeHashTable< Key, Value > > it = overflow_.cbegin();
  while (it != overflow_.cend())
  {
    if ((*it).is_valid && e(key, it->data.first))
    {
      return HashTableConstIterator< Key, Value, Hash, Equal >(this,
        count_buckets_, 0, it);
    }
    ++it;
  }
  return cend();
}
template< class Key, class Value, class Hash, class Equal >
Value& goltsov::HashTable< Key, Value, Hash, Equal >::operator[](const Key& key)
{
  if (contains(key))
  {
    return at(key);
  }
  else
  {
    insert(std::pair< Key, Value >{key, Value{}});
    return at(key);
  }
}
template< class Key, class Value, class Hash, class Equal >
const Value& goltsov::HashTable< Key, Value, Hash, Equal >::operator[](const Key& key) const
{
  return at(key);
}
template< class Key, class Value, class Hash, class Equal >
goltsov::HashTableIterator< Key, Value, Hash, Equal > goltsov::HashTable< Key, Value, Hash, Equal >::begin()
{
  return detail::makeInconstantHashTableIterator(cbegin());
}
template< class Key, class Value, class Hash, class Equal >
goltsov::HashTableIterator< Key, Value, Hash, Equal > goltsov::HashTable< Key, Value, Hash, Equal >::end()
{
  return HashTableIterator< Key, Value, Hash, Equal >(this, count_buckets_, 0, overflow_.end());
}
template< class Key, class Value, class Hash, class Equal >
goltsov::HashTableConstIterator< Key, Value, Hash, Equal >
  goltsov::HashTable< Key, Value, Hash, Equal >::cbegin() const
{
  for (size_t i = 0; i < count_buckets_; ++i)
  {
    for (size_t j = 0; j < size_bucket_; ++j)
    {
      if (data_[i].node[j].is_valid)
      {
        return HashTableConstIterator< Key, Value, Hash, Equal >(this, i, j,
          LCIter< detail::NodeHashTable< Key, Value > >());
      }
    }
  }
  LCIter< detail::NodeHashTable< Key, Value > > overflow_now = overflow_.cbegin();
  while (overflow_now != overflow_.cend())
  {
    if ((*overflow_now).is_valid)
    {
      return HashTableConstIterator< Key, Value, Hash, Equal >(this, count_buckets_, 0, overflow_now);
    }
    ++overflow_now;
  }
  return cend();
}
template< class Key, class Value, class Hash, class Equal >
goltsov::HashTableConstIterator< Key, Value, Hash, Equal >
  goltsov::HashTable< Key, Value, Hash, Equal >::cend() const
{
  return HashTableConstIterator< Key, Value, Hash, Equal >(this, count_buckets_, 0, overflow_.cend());
}
template< class Key, class Value, class Hash, class Equal >
goltsov::HashTableIterator< Key, Value, Hash, Equal >::HashTableIterator() noexcept:
  hash_table_(nullptr),
  ind_(0),
  ind_Bucket_(0),
  overflow_iterator_(LIter< detail::NodeHashTable< Key, Value > >())
{}
template< class Key, class Value, class Hash, class Equal >
goltsov::HashTableIterator< Key, Value, Hash, Equal >::HashTableIterator(
  HashTable< Key, Value, Hash, Equal >* hash_table,
  size_t ind, size_t ind_Bucket, LIter< detail::NodeHashTable< Key, Value > > overflow_it) noexcept:
  hash_table_(hash_table),
  ind_(ind),
  ind_Bucket_(ind_Bucket),
  overflow_iterator_(overflow_it)
{}
template< class Key, class Value, class Hash, class Equal >
goltsov::HashTableIterator< Key, Value, Hash, Equal >
  goltsov::HashTableIterator< Key, Value, Hash, Equal >::next() const
{
  if (overflow_iterator_ != hash_table_->overflow_.end())
  {
    LIter< detail::NodeHashTable< Key, Value > > it = overflow_iterator_;
    while (it != hash_table_->overflow_.end())
    {
      ++it;
      if (it != hash_table_->overflow_.end() && (*it).is_valid)
      {
        return HashTableIterator(hash_table_, hash_table_->countBuckets(), 0, it);
      }
    }
    return hash_table_->end();
  }
  else
  {
    for (size_t j = ind_Bucket_ + 1; j < hash_table_->size_bucket_; ++j)
    {
      if (hash_table_->data_[ind_].node[j].is_valid)
      {
        return HashTableIterator(hash_table_, ind_, j, LIter< detail::NodeHashTable< Key, Value > >());
      }
    }
    for (size_t i = ind_ + 1; i < hash_table_->count_buckets_; ++i)
    {
      for (size_t j = 0; j < hash_table_->size_bucket_; ++j)
      {
        if (hash_table_->data_[i].node[j].is_valid)
        {
          return HashTableIterator(hash_table_, i, j, LIter< detail::NodeHashTable< Key, Value > >());
        }
      }
    }
    LIter< detail::NodeHashTable< Key, Value > > it = hash_table_->overflow_.begin();
    while (it != hash_table_->overflow_.end())
    {
      if ((*it).is_valid)
      {
        return HashTableIterator(hash_table_, hash_table_->count_buckets_, 0, it);
      }
      ++it;
    }
    return hash_table_->end();
  }
}
template< class Key, class Value, class Hash, class Equal >
bool goltsov::HashTableIterator< Key, Value, Hash, Equal >::hasNext() const noexcept
{
  if ((*this) == hash_table_->end())
  {
    return false;
  }
  else
  {
    return true;
  }
}
template< class Key, class Value, class Hash, class Equal >
goltsov::HashTableIterator< Key, Value, Hash, Equal >&
  goltsov::HashTableIterator< Key, Value, Hash, Equal >::operator++()
{
  (*this) = next();
  return (*this);
}
template< class Key, class Value, class Hash, class Equal >
goltsov::HashTableIterator< Key, Value, Hash, Equal >
  goltsov::HashTableIterator< Key, Value, Hash, Equal >::operator++(int)
{
  HashTableIterator< Key, Value, Hash, Equal > temp = (*this);
  ++(*this);
  return temp;
}
template< class Key, class Value, class Hash, class Equal >
std::pair< Key, Value >*
  goltsov::HashTableIterator< Key, Value, Hash, Equal >::operator->() noexcept
{
  if (overflow_iterator_ != hash_table_->overflow_.end())
  {
    return &(overflow_iterator_->data);
  }
  else
  {
    return &(hash_table_->data_[ind_].node[ind_Bucket_].data);
  }
}
template< class Key, class Value, class Hash, class Equal >
std::pair< Key, Value >&
  goltsov::HashTableIterator< Key, Value, Hash, Equal >::operator*() noexcept
{
  if (overflow_iterator_ != hash_table_->overflow_.end())
  {
    return (overflow_iterator_->data);
  }
  else
  {
    return hash_table_->data_[ind_].node[ind_Bucket_].data;
  }
}
template< class Key, class Value, class Hash, class Equal >
goltsov::HashTableIterator< Key, Value, Hash, Equal >::operator
  HashTableConstIterator< Key, Value, Hash, Equal >() const noexcept
{
  return HashTableConstIterator< Key, Value, Hash, Equal >(hash_table_, ind_, ind_Bucket_, overflow_iterator_);
}
template< class Key, class Value, class Hash, class Equal >
goltsov::HashTableConstIterator< Key, Value, Hash, Equal >::HashTableConstIterator() noexcept:
  hash_table_(nullptr),
  ind_(0),
  ind_Bucket_(0),
  overflow_iterator_(LCIter< detail::NodeHashTable< Key, Value > >())
{}
template< class Key, class Value, class Hash, class Equal >
goltsov::HashTableConstIterator< Key, Value, Hash, Equal >::HashTableConstIterator(
  const HashTable< Key, Value,Hash, Equal >* hash_table, size_t ind, size_t ind_Bucket,
  LCIter< detail::NodeHashTable< Key, Value > > overflow_it) noexcept:
  hash_table_(hash_table),
  ind_(ind),
  ind_Bucket_(ind_Bucket),
  overflow_iterator_(overflow_it)
{}
template< class Key, class Value, class Hash, class Equal >
goltsov::HashTableConstIterator< Key, Value, Hash, Equal >
  goltsov::HashTableConstIterator< Key, Value, Hash, Equal >::next() const
{
  if (overflow_iterator_ != hash_table_->overflow_.cend())
  {
    LCIter< detail::NodeHashTable< Key, Value > > it = overflow_iterator_;
    while (it != hash_table_->overflow_.cend())
    {
      ++it;
      if (it != hash_table_->overflow_.cend() && (*it).is_valid)
      {
        return HashTableConstIterator(hash_table_, hash_table_->countBuckets(), 0, it);
      }
    }
    return hash_table_->cend();
  }
  else
  {
    for (size_t j = ind_Bucket_ + 1; j < hash_table_->size_bucket_; ++j)
    {
      if (hash_table_->data_[ind_].node[j].is_valid)
      {
        return HashTableConstIterator(hash_table_, ind_, j, LCIter< detail::NodeHashTable< Key, Value > >());
      }
    }
    for (size_t i = ind_ + 1; i < hash_table_->count_buckets_; ++i)
    {
      for (size_t j = 0; j < hash_table_->size_bucket_; ++j)
      {
        if (hash_table_->data_[i].node[j].is_valid)
        {
          return HashTableConstIterator(hash_table_, i, j, LCIter< detail::NodeHashTable< Key, Value > >());
        }
      }
    }
    LCIter< detail::NodeHashTable< Key, Value > > it = hash_table_->overflow_.cbegin();
    while (it != hash_table_->overflow_.cend())
    {
      if ((*it).is_valid)
      {
        return HashTableConstIterator(hash_table_, hash_table_->countBuckets(), 0, it);
      }
      ++it;
    }
    return hash_table_->cend();
  }
}
template< class Key, class Value, class Hash, class Equal >
bool goltsov::HashTableConstIterator< Key, Value, Hash, Equal >::hasNext() const noexcept
{
  if ((*this) == hash_table_->end())
  {
    return false;
  }
  else
  {
    return true;
  }
}
template< class Key, class Value, class Hash, class Equal >
const std::pair< Key, Value >* goltsov::HashTableConstIterator< Key, Value, Hash, Equal >::operator->() noexcept
{
  if (overflow_iterator_ != hash_table_->overflow_.cend())
  {
    return &(overflow_iterator_->data);
  }
  else
  {
    return &(hash_table_->data_[ind_].node[ind_Bucket_].data);
  }
}
template< class Key, class Value, class Hash, class Equal >
const std::pair< Key, Value >& goltsov::HashTableConstIterator< Key, Value, Hash, Equal >::operator*() noexcept
{
  if (overflow_iterator_ != hash_table_->overflow_.cend())
  {
    return (overflow_iterator_->data);
  }
  else
  {
    return hash_table_->data_[ind_].node[ind_Bucket_].data;
  }
}
template< class Key, class Value, class Hash, class Equal >
goltsov::HashTableConstIterator< Key, Value, Hash, Equal >&
  goltsov::HashTableConstIterator< Key, Value, Hash, Equal >::operator++()
{
  (*this) = next();
  return (*this);
}
template< class Key, class Value, class Hash, class Equal >
goltsov::HashTableConstIterator< Key, Value, Hash, Equal >
  goltsov::HashTableConstIterator< Key, Value, Hash, Equal >::operator++(int)
{
  HashTableConstIterator< Key, Value, Hash, Equal > temp = (*this);
  ++(*this);
  return temp;
}
template< class Key, class Value, class Hash, class Equal >
bool goltsov::operator==(const HashTableIterator< Key, Value, Hash, Equal >& lhs,
  const HashTableIterator< Key, Value, Hash, Equal >& rhs)
{
  return (lhs.hash_table_ == rhs.hash_table_ && lhs.ind_ == rhs.ind_
    && lhs.ind_Bucket_ == rhs.ind_Bucket_ && lhs.overflow_iterator_ == rhs.overflow_iterator_);
}
template< class Key, class Value, class Hash, class Equal >
bool goltsov::operator!=(const HashTableIterator< Key, Value, Hash, Equal >& lhs,
  const HashTableIterator< Key, Value, Hash, Equal >& rhs)
{
  return !(rhs == lhs);
}
template< class Key, class Value, class Hash, class Equal >
bool goltsov::operator==(const HashTableConstIterator< Key, Value, Hash, Equal >& lhs,
  const HashTableConstIterator< Key, Value, Hash, Equal >& rhs)
{
  return (lhs.hash_table_ == rhs.hash_table_ && lhs.ind_ == rhs.ind_
    && lhs.ind_Bucket_ == rhs.ind_Bucket_ && lhs.overflow_iterator_ == rhs.overflow_iterator_);
}
template< class Key, class Value, class Hash, class Equal >
bool goltsov::operator!=(const HashTableConstIterator< Key, Value, Hash, Equal >& lhs,
  const HashTableConstIterator< Key, Value, Hash, Equal >& rhs)
{
  return !(rhs == lhs);
}
template< class Key, class Value, class Hash, class Equal >
bool goltsov::operator==(const HashTableIterator< Key, Value, Hash, Equal >& lhs,
  const HashTableConstIterator< Key, Value, Hash, Equal >& rhs)
{
  return (lhs.hash_table_ == const_cast< HashTable< Key, Value, Hash, Equal >* >(rhs.hash_table_)
    && lhs.ind_ == rhs.ind_ && lhs.ind_Bucket_ == rhs.ind_Bucket_
    && lhs.overflow_iterator_ == rhs.overflow_iterator_);
}
template< class Key, class Value, class Hash, class Equal >
bool goltsov::operator!=(const HashTableIterator< Key, Value, Hash, Equal >& lhs,
  const HashTableConstIterator< Key, Value, Hash, Equal >& rhs)
{
  return !(rhs == lhs);
}
template< class Key, class Value, class Hash, class Equal >
bool goltsov::operator==(const HashTableConstIterator< Key, Value, Hash, Equal >& lhs,
  const HashTableIterator< Key, Value, Hash, Equal >& rhs)
{
  return (lhs.hash_table_ == const_cast< const HashTable< Key, Value, Hash, Equal >* >(rhs.hash_table_)
    && lhs.ind_ == rhs.ind_ && lhs.ind_Bucket_ == rhs.ind_Bucket_
    && lhs.overflow_iterator_ == rhs.overflow_iterator_);
}
template< class Key, class Value, class Hash, class Equal >
bool goltsov::operator!=(const HashTableConstIterator< Key, Value, Hash, Equal >& lhs,
  const HashTableIterator< Key, Value, Hash, Equal >& rhs)
{
  return !(rhs == lhs);
}
template< class Key, class Value, class Hash, class Equal >
goltsov::HashTableIterator< Key, Value, Hash, Equal > goltsov::detail::makeInconstantHashTableIterator(
  HashTableConstIterator< Key, Value, Hash, Equal > it)
{
  return HashTableIterator< Key, Value, Hash, Equal >{
    const_cast< HashTable< Key, Value, Hash, Equal >* >(it.hash_table_), it.ind_, it.ind_Bucket_,
    makeInconstantLIter(it.overflow_iterator_)};
}
template< class Key, class Value, class Hash, class Equal >
size_t goltsov::HashTable< Key, Value, Hash, Equal >::getSizeOverflowBucket() const noexcept
{
  return overflow_.size();
}
template< class Key, class Value, class Hash, class Equal >
double goltsov::HashTable< Key, Value, Hash, Equal >::getAvgCountInBucket() const noexcept
{
  return (static_cast< double >(count_valid_)) / (static_cast< double >(count_buckets_));
}
template< class Key, class Value, class Hash, class Equal >
void goltsov::HashTable< Key, Value, Hash, Equal >::setMaxSizeOverflow(const size_t& new_val) noexcept
{
  if (new_val == 0)
  {
    throw std::runtime_error("MaxAvgCount must be greater than 0");
  }
  max_size_overflow_ = new_val;
}
template< class Key, class Value, class Hash, class Equal >
void goltsov::HashTable< Key, Value, Hash, Equal >::setMaxAvgCountInBucket(const double& new_val) noexcept
{
  if (new_val <= 0)
  {
    throw std::runtime_error("MaxAvgCount must be greater than 0");
  }
  max_avg_count_in_bucket_ = new_val;
}
inline size_t goltsov::upd_buckets(const size_t& old_count_buckets)
{
  return old_count_buckets ? old_count_buckets * 2 : 1;
}
inline size_t goltsov::upd_bucket_size(const size_t& old_bucket_size)
{
  return old_bucket_size ? old_bucket_size * 2 : 1;
}
template< class Key, class Value, class Hash, class Equal >
bool goltsov::HashTable< Key, Value, Hash, Equal >::checkAndRebuild()
{
  bool needRehash = false;
  size_t new_count_buckets = count_buckets_;
  size_t new_size_bucket = size_bucket_;
  if (getSizeOverflowBucket() >= max_size_overflow_)
  {
    needRehash = true;
    new_count_buckets = upd_buckets(count_buckets_);
  }
  if (getAvgCountInBucket() >= max_avg_count_in_bucket_)
  {
    needRehash = true;
    new_size_bucket = upd_bucket_size(size_bucket_);
    new_count_buckets = upd_buckets(count_buckets_);
  }
  if (needRehash)
  {
    rehash(new_count_buckets, new_size_bucket);
  }
  return needRehash;
}

#endif
