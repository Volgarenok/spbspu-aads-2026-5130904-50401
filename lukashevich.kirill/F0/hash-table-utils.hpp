#ifndef HASH_TABLE_UTILS_HPP
#define HASH_TABLE_UTILS_HPP

#include <cstddef>

namespace lukashevich {
  namespace detail {
    size_t getNextPrimeCapacity(size_t current);
  }
}

#endif
