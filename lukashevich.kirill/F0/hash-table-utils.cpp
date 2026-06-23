#include "hash-table-utils.hpp"

namespace lukashevich {
  namespace detail {
    bool isPrimeCapacity(size_t number);
  }
}

bool lukashevich::detail::isPrimeCapacity(size_t number)
{
  if (number < 2) {
    return false;
  }
  if ((number % 2) == 0) {
    return number == 2;
  }

  for (size_t divisor = 3; divisor <= (number / divisor); divisor += 2) {
    if ((number % divisor) == 0) {
      return false;
    }
  }

  return true;
}

size_t lukashevich::detail::getNextPrimeCapacity(size_t current)
{
  const size_t capacities[] = {
    11,
    23,
    47,
    97,
    197,
    397,
    797,
    1597,
    3203,
    6421,
    12853,
    25717,
    51437,
    102877,
    205759,
    411527,
    823117,
    1646237
  };

  const size_t count = sizeof(capacities) / sizeof(capacities[0]);
  for (size_t i = 0; i < count; ++i) {
    if (capacities[i] > current) {
      return capacities[i];
    }
  }

  size_t candidate = current + 1;
  if ((candidate % 2) == 0) {
    ++candidate;
  }

  while (!isPrimeCapacity(candidate)) {
    candidate += 2;
  }

  return candidate;
}
