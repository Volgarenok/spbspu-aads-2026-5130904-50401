#ifndef TRANSPOSITION_TABLE_HPP
#define TRANSPOSITION_TABLE_HPP

#include <cstdint>
#include "position.hpp"
#include "move.hpp"
#include "robinHoodTable.hpp"

namespace chess
{
  enum TTEntryType
  {
    EXACT,
    LOWER_BOUND,
    UPPER_BOUND
  };

  struct TTEntry
  {
    uint64_t key_;
    int eval_;
    int depth_;
    TTEntryType type_;
    bool used_ = 0;
    Move bestMove_;
  };

  struct TTKeyHash
  {
    size_t operator()(uint64_t key) const noexcept
    {
      return static_cast< size_t >(key);
    }
  };

  struct TranspositionTable
  {
  private:
    tarasenko::RobinHoodTable< uint64_t, TTEntry, TTKeyHash > data_;
    TTEntry empty_entry_;

  public:
    TranspositionTable();
    TranspositionTable(uint64_t size);

    void addEntry(TTEntry entry);
    TTEntry& getEntry(uint64_t key);
  };
}

#endif
