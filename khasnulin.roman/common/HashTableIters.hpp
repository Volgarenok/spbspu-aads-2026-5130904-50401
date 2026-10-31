#ifndef HASH_TABLE_ITERS_HPP
#define HASH_TABLE_ITERS_HPP

#include "LIter.hpp"
#include "VectorRandomIter.hpp"
namespace khasnulin
{

  template < class Key, class Value, class Hasher, class Equal > class CHIter;

  template < class Key, class Value, class Hash, class Equal > class HashTable;

  template < class Key, class Value, class Hasher, class Equal > class HIter
  {
  public:
    using value_type = typename HashTable< Key, Value, Hasher, Equal >::public_value_type;

    HIter();

    HIter &operator++();
    HIter operator++(int);

    value_type &operator*() const;
    value_type *operator->() const;

    bool operator==(const HIter &rhs) const;

    friend class HashTable< Key, Value, Hasher, Equal >;

    friend class CHIter< Key, Value, Hasher, Equal >;

    bool operator!=(const HIter &rhs) const;

  private:
    using Bucket = typename HashTable< Key, Value, Hasher, Equal >::Bucket;
    using OverflowValue = typename HashTable< Key, Value, Hasher, Equal >::value_type;
    HIter(HashTable< Key, Value, Hasher, Equal > &hashTable, bool isEnd);

    HIter(HashTable< Key, Value, Hasher, Equal > &hashTable, RAIter< Bucket > bucket, size_t slot,
          LIter< OverflowValue > overflow);

    void moveToValid();

    size_t curr_elem_;
    RAIter< Bucket > bucket_elem_;
    RAIter< Bucket > bucket_end_;
    LIter< OverflowValue > overflow_elem_;
    LIter< OverflowValue > overflow_end_;
  };

  template < class Key, class Value, class Hasher, class Equal > class CHIter
  {
  public:
    using value_type = typename HashTable< Key, Value, Hasher, Equal >::public_value_type;

    CHIter() = default;

    CHIter(HIter< Key, Value, Hasher, Equal > iter);

    CHIter &operator++();
    CHIter operator++(int);

    const value_type &operator*() const;
    const value_type *operator->() const;

    bool operator==(const CHIter &rhs) const;

    bool operator!=(const CHIter &rhs) const;

    friend class HashTable< Key, Value, Hasher, Equal >;

  private:
    CHIter(const HashTable< Key, Value, Hasher, Equal > &hashTable, bool isEnd);
    HIter< Key, Value, Hasher, Equal > iter_;
  };

  template < class Key, class Value, class Hasher, class Equal >
  HIter< Key, Value, Hasher, Equal >::HIter():
      curr_elem_(0)
  {
  }

  template < class Key, class Value, class Hasher, class Equal >
  HIter< Key, Value, Hasher, Equal >::HIter(HashTable< Key, Value, Hasher, Equal > &hashTable, bool isEnd):
      curr_elem_(0),
      bucket_elem_(isEnd ? hashTable.buckets_.end() : hashTable.buckets_.begin()),
      bucket_end_(hashTable.buckets_.end()),
      overflow_elem_(isEnd ? hashTable.overflow_.end() : hashTable.overflow_.begin()),
      overflow_end_(hashTable.overflow_.end())
  {
    moveToValid();
  }

  template < class Key, class Value, class Hasher, class Equal >
  HIter< Key, Value, Hasher, Equal >::HIter(HashTable< Key, Value, Hasher, Equal > &hashTable,
                                            RAIter< Bucket > bucket, size_t slot,
                                            LIter< OverflowValue > overflow):
      curr_elem_(slot),
      bucket_elem_(bucket),
      bucket_end_(hashTable.buckets_.end()),
      overflow_elem_(overflow),
      overflow_end_(hashTable.overflow_.end())
  {
    moveToValid();
  }

  // Двигаемся к ближайшему непустому бакету.
  // Проверяем пуст ли текущий бакет в цикле, если пуст -> двигаемся дальше пока не найдем или не дойдем до
  // конца бакетов
  template < class Key, class Value, class Hasher, class Equal >
  void HIter< Key, Value, Hasher, Equal >::moveToValid()
  {
    while (bucket_elem_ != bucket_end_ && curr_elem_ >= (*bucket_elem_).count)
    {
      ++bucket_elem_;
      curr_elem_ = 0;
    }
  }

  // Forwarding iter
  // Двигаемся вперед: если ещё не прошли все бакеты, то двигаемся по текущему бакету дальше, пока не выйдем
  // за count Если прошли все бакеты, то двигаемся итератором по списку overflow
  template < class Key, class Value, class Hasher, class Equal >
  HIter< Key, Value, Hasher, Equal > &HIter< Key, Value, Hasher, Equal >::operator++()
  {
    // текущий бакет ещё не дошел до конца
    if (bucket_elem_ != bucket_end_)
    {
      // Увеличиваем curr_elem и внутри move to valid проверяем, можно не достигли ли мы лимита, если достигли
      // -> ищем ближайший непустой бакет
      ++curr_elem_;
      moveToValid();
      return (*this);
    }
    // бакеты закончились, идем по overflow
    ++overflow_elem_;
    return (*this);
  }

  template < class Key, class Value, class Hasher, class Equal >
  HIter< Key, Value, Hasher, Equal > HIter< Key, Value, Hasher, Equal >::operator++(int)
  {
    HIter< Key, Value, Hasher, Equal > cp(*this);
    this->operator++();
    return cp;
  }

  template < class Key, class Value, class Hasher, class Equal >
  typename HIter< Key, Value, Hasher, Equal >::value_type &
  HIter< Key, Value, Hasher, Equal >::operator*() const
  {
    // Бакеты не обошли ещё -> возвращаем из бакета ссылку
    if (bucket_elem_ != bucket_end_)
    {
      return reinterpret_cast< value_type & >((*bucket_elem_).data[curr_elem_]);
    }
    // Мы сейчас идем по overflow, возвращаем оттуда ссылку
    return reinterpret_cast< value_type & >(
        *const_cast< LIter< typename HashTable< Key, Value, Hasher, Equal >::value_type > & >(
            overflow_elem_));
  }

  template < class Key, class Value, class Hasher, class Equal >
  typename HIter< Key, Value, Hasher, Equal >::value_type *
  HIter< Key, Value, Hasher, Equal >::operator->() const
  {
    return &(this->operator*());
  }

  template < class Key, class Value, class Hasher, class Equal >
  bool HIter< Key, Value, Hasher, Equal >::operator==(const HIter< Key, Value, Hasher, Equal > &rhs) const
  {
    // Проверяем, что у обоих итератов один и тот же итератор на бакет
    if (bucket_elem_ != rhs.bucket_elem_)
    {
      return false;
    }

    // Если бакет один и тот же и это не конец вектора, проверяем номера элементов
    if (bucket_elem_ != bucket_end_ && curr_elem_ == rhs.curr_elem_)
    {
      return true;
    }

    // Достигнут конец бакета, тогда смотрим, совпадают ли overflow итераторы
    if (bucket_elem_ == bucket_end_)
    {
      return overflow_elem_ == rhs.overflow_elem_;
    }

    return false;
  }

  template < class Key, class Value, class Hasher, class Equal >
  bool HIter< Key, Value, Hasher, Equal >::operator!=(const HIter &rhs) const
  {
    return !(*this == rhs);
  }

  template < class Key, class Value, class Hasher, class Equal >
  CHIter< Key, Value, Hasher, Equal >::CHIter(const HashTable< Key, Value, Hasher, Equal > &hashTable,
                                              bool isEnd):
      iter_(const_cast< HashTable< Key, Value, Hasher, Equal > & >(hashTable), isEnd)
  {
  }

  template < class Key, class Value, class Hasher, class Equal >
  CHIter< Key, Value, Hasher, Equal >::CHIter(HIter< Key, Value, Hasher, Equal > iter):
      iter_(iter)
  {
  }

  template < class Key, class Value, class Hasher, class Equal >
  CHIter< Key, Value, Hasher, Equal > &CHIter< Key, Value, Hasher, Equal >::operator++()
  {
    ++iter_;
    return (*this);
  }

  template < class Key, class Value, class Hasher, class Equal >
  CHIter< Key, Value, Hasher, Equal > CHIter< Key, Value, Hasher, Equal >::operator++(int)
  {
    CHIter< Key, Value, Hasher, Equal > cp(*this);
    ++iter_;
    return cp;
  }
  template < class Key, class Value, class Hasher, class Equal >
  const typename CHIter< Key, Value, Hasher, Equal >::value_type &
  CHIter< Key, Value, Hasher, Equal >::operator*() const
  {
    return static_cast< const value_type & >(*iter_);
  }

  template < class Key, class Value, class Hasher, class Equal >
  const typename CHIter< Key, Value, Hasher, Equal >::value_type *
  CHIter< Key, Value, Hasher, Equal >::operator->() const
  {
    return &(this->operator*());
  }

  template < class Key, class Value, class Hasher, class Equal >
  bool CHIter< Key, Value, Hasher, Equal >::operator==(const CHIter &rhs) const
  {
    return iter_ == rhs.iter_;
  }

  template < class Key, class Value, class Hasher, class Equal >
  bool CHIter< Key, Value, Hasher, Equal >::operator!=(const CHIter &rhs) const
  {
    return !(iter_ == rhs.iter_);
  }
}

#endif
