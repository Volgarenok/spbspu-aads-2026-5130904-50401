#include "transposition_table.hpp"
#include <cstdint>

namespace chess
{
  TranspositionTable::TranspositionTable():
    TranspositionTable(1ULL << 22)
  {}

  TranspositionTable::TranspositionTable(uint64_t size):
    data_(size)
  {}

  void TranspositionTable::addEntry(TTEntry entry)
  {
    if (!data_.insert({entry.key_, entry}))
    {
      data_.at(entry.key_) = entry;
    }
  }

  TTEntry& TranspositionTable::getEntry(uint64_t key)
  {
    auto it = data_.find(key);
    if (it == data_.end())
    {
      empty_entry_ = TTEntry{};
      return empty_entry_;
    }
    return it->second;
  }
}
