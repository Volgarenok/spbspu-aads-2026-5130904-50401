#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>

namespace chernov {

  template< class K, class V, class H1, class H2, class E, bool IC >
  class CuckooHTIter;

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  class CuckooHT {
  public:
    template< class K, class V, class H1, class H2, class E, bool IC >
    friend class CuckooHTIter;

    using Slot = std::pair< const Key, Value >;
    using iterator = CuckooHTIter< Key, Value, Hash1, Hash2, Equal, false >;
    using const_iterator = CuckooHTIter< Key, Value, Hash1, Hash2, Equal, true >;

    CuckooHT();
    CuckooHT(const CuckooHT & ht);
    CuckooHT(CuckooHT && ht) noexcept;

    explicit CuckooHT(size_t slots);

    ~CuckooHT();

    CuckooHT & operator=(const CuckooHT & ht);
    CuckooHT & operator=(CuckooHT && ht) noexcept;

    void swap(CuckooHT & ht) noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t maxCapacity() const noexcept;

    void clear() noexcept;

    void add(Key k, Value v);
    void remove(Key k);
    bool has(Key k) const;
    void rehash(size_t slots);

    Value & at(const Key & k);
    const Value & at(const Key & k) const;

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;

    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

  private:
    Slot * table1_;
    Slot * table2_;
    bool * occupied1_;
    bool * occupied2_;
    size_t capacity_;
    size_t count_;

    Hash1 hasher1_;
    Hash2 hasher2_;
    Equal equal_;

    CuckooHT(size_t capacity, int);

    size_t hash1(const Key & k) const noexcept;
    size_t hash2(const Key & k) const noexcept;

    std::pair< Slot *, int > findKey(const Key & k) const;
    void insertWithEviction(Key k, Value v);
    void rehashInternal(size_t newSlots);

    size_t firstValidIndex() const noexcept;
    size_t endIndex() const noexcept;
  };

}

#endif
