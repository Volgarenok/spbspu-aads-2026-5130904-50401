#ifndef MOVE_HPP
#define MOVE_HPP

#include <iosfwd>
#include "position.hpp"
#include "square.hpp"

namespace chess
{
  struct Move
  {
    Square from_;
    Square to_;
    Piece promotionPiece_ = EMPTY;
    bool isEnPassant_ = 0;
    bool isCastling_ = 0;
    int score_ = 0;
  };
  const Move null_move = {A1, A1};

  struct MoveArray
  {
    friend struct Engine;

    MoveArray();
    void push(const Move& move) noexcept;
    const Move& get(int index) const noexcept;

    void clear() noexcept;
    int size() const noexcept;
    bool empty() const noexcept;

    void print() const;

  private:
    Move moves_[256];
    int size_;
  };

  bool operator==(const Move& move1, const Move& move2);
  bool operator!=(const Move& move1, const Move& move2);
  std::ostream& operator<<(std::ostream& out, const Move& move);
  std::ostream& operator<<(std::ostream& out, Square square);
  std::istream& operator>>(std::istream& in, Move& move);

  bool containsMove(const MoveArray& moves, Move move);
  bool isEqualArraysUnordered(const MoveArray& moves1, const MoveArray& moves2);
  Move getMove(const MoveArray& moves, Square from, Square to);
  Move getMove(const MoveArray& moves, Move move);
  void printMove(const Move& move, const Position& pos, std::ostream& out);
}

#endif
