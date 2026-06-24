#ifndef ZOBRIST_HPP
#define ZOBRIST_HPP

#include <cstdint>
#include "position.hpp"
#include "move.hpp"

namespace chess
{
  void initZobristHash();
  uint64_t zobristHash(const Position& pos);
  uint64_t incrementZobristHash(uint64_t hash, const Position& pos, const Move& move);

  int pieceIndex(int piece);
}

#endif
