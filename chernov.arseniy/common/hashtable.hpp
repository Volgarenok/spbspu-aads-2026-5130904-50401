#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

namespace chernov {
  template< class Key, class Value, class Hash, class Equal >
  class HashTable {
  public:
    HashTable();
    ~HashTable();
    void add(Key k, Value v);
    Value drop(Key k);
    bool has(Key k);
    void rehash(size_t slots);
  private:
    T * _data;
    size_t * _bucket_sizes;
    size_t _num_buckets;
    size_t _bucker_cap;
    size_t _overflow_cap;
  };
}

#endif
