#ifndef UTILS_RL
#define UTILS_RL

#include "hashtable.hpp"
#include <experimental/optional>

namespace rl {
template <typename Key, typename Value>
using Map = levkin::HashTable<Key, Value>;


} // namespace rl
#endif
