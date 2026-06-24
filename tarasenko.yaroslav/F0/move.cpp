#include "move.hpp"
#include "move_generator.hpp"
#include "piece.hpp"
#include "position.hpp"
#include <iostream>

namespace chess
{
  MoveArray::MoveArray():
    size_(0)
  {}

  void MoveArray::push(const Move& move) noexcept
  {
    moves_[size_] = move;
    ++size_;
  }

  const Move& MoveArray::get(int index) const noexcept
  {
    return moves_[index];
  }

  void MoveArray::clear() noexcept
  {
    size_ = 0;
  }

  int MoveArray::size() const noexcept
  {
    return size_;
  }

  bool MoveArray::empty() const noexcept
  {
    return !size_;
  }

  void MoveArray::print() const
  {
    if (!empty())
    {
      std::cout << moves_[0];
    }
    for (int i = 1; i < size(); ++i)
    {
      std::cout << " " << moves_[i];
    }
    std::cout << "\n";
  }

  bool operator==(const Move& move1, const Move& move2)
  {
    return (move1.from_ == move2.from_) && (move1.to_ == move2.to_)
    && (move1.promotionPiece_ == move2.promotionPiece_)
    && (move1.isCastling_ == move2.isCastling_)
    && (move1.isEnPassant_ == move2.isEnPassant_);
  }

  bool operator!=(const Move& move1, const Move& move2)
  {
    return !(move1 == move2);
  }

  std::ostream& operator<<(std::ostream& out, const Move& move)
  {
    return out << move.from_ << "-" << move.to_;
  }

  std::ostream& operator<<(std::ostream& out, Square square)
  {
    char col = 'a' + (square % 8);
    char row = '1' + (square / 8);
    return out << col << row;
  }

  std::istream& operator>>(std::istream& in, Move& move)
  {
    char char_from;
    int row_from;
    in >> char_from >> row_from;
    char separator;
    in >> separator;
    char char_to;
    int row_to;
    in >> char_to >> row_to;

    if (!in) return in;

    int col_from = char_from - 'a';
    int col_to = char_to - 'a';
    if (col_from > 7 || col_to > 7 || row_from > 8 || row_to > 8 || separator != '-')
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    move = {static_cast< Square >((row_from - 1) * 8 + col_from),
      static_cast< Square >((row_to - 1) * 8 + col_to)};
    return in;
  }

  bool containsMove(const MoveArray& moves, Move move)
  {
    for (int i = 0; i < moves.size(); ++i)
    {
      if (moves.get(i) == move)
      {
        return true;
      }
    }
    return false;
  }

  bool isEqualArraysUnordered(const MoveArray& moves1, const MoveArray& moves2)
  {
    if (moves1.size() != moves2.size())
    {
      return false;
    }

    for (int i = 0; i < moves1.size(); ++i)
    {
      if (!containsMove(moves2, moves1.get(i)))
      {
        return false;
      }
    }
    return true;
  }

  Move getMove(const MoveArray& moves, Square from, Square to)
  {
    for (int i = 0; i < moves.size(); ++i)
    {
      if (moves.get(i).from_ == from && moves.get(i).to_ == to)
      {
        return moves.get(i);
      }
    }
    return null_move;
  }

  Move getMove(const MoveArray& moves, Move move)
  {
    Move returned = getMove(moves, move.from_, move.to_);
    if (returned != Move{A1, A1})
    {
      return returned;
    }
    throw std::logic_error("Illegal move");
  }

  void printMove(const Move& move, const Position& pos, std::ostream& out)
  {
    int piece = pos.getPiece(move.from_);
    int abs_piece = piece > 0 ? piece : -piece;
    char piece_char = pieceToChar(static_cast< Piece >(abs_piece));
    int row = move.to_ / 8;
    int col = move.to_ % 8;
    if (piece_char == 'P')
    {
      if (pos.getPiece(move.to_) != EMPTY || move.isEnPassant_)
      {
        piece_char = static_cast< char >('a' + (move.from_ % 8));
      }
      else
      {
        piece_char = '\0';
      }
    }
    Position copy = pos;
    UndoInfo undo;
    copy.makeMove(move, undo);
    bool isCheck = MoveGenerator::isCheck(copy);
    bool isMate = false;
    if (isCheck)
    {
      isMate = MoveGenerator::isMate(copy);
    }
    char check = isCheck ? '+' : '\0';
    char mate = isMate ? '#' : '\0';
    if (isMate) check = '\0';

    if (move.isCastling_)
    {
      if (move.to_ - move.from_ == 2)
      {
        out << "0-0";
        if (check) out << check;
        if (mate) out << mate;
        return;
      }
      out << "0-0-0";
      if (check) out << check;
      if (mate) out << mate;
      return;
    }

    char promotion_piece = '\0';
    if (move.promotionPiece_ != EMPTY)
    {
      promotion_piece = move.promotionPiece_ > 0 ? move.promotionPiece_ : -move.promotionPiece_;
    }

    char file_from = '\0';
    char col_from = '\0';
    char hyphen = '\0';
    if (abs_piece != WHITE_PAWN)
    {
      MoveArray legal_moves = MoveGenerator::generateLegalMoves(pos);
      for (int i = 0; i < legal_moves.size(); ++i)
      {
        Move curr_move = legal_moves.get(i);
        if (curr_move.to_ == move.to_ && pos.getPiece(curr_move.from_) == piece)
        {
          if (curr_move.from_ % 8 != move.from_ % 8)
          {
            file_from = static_cast< char >('a' + (move.from_ % 8));
          }
          if (curr_move.from_ / 8 != move.from_ / 8)
          {
            col_from = static_cast< char >('1' + (move.from_ / 8));
          }
        }
      }
      if (file_from || col_from) hyphen = '-';
    }

    char capture = (pos.getPiece(move.to_) != EMPTY || move.isCastling_) ? 'x' : '\0';
    if (piece_char) out << piece_char;
    if (file_from) out << file_from;
    if (col_from) out << col_from;
    if (hyphen) out << hyphen;
    if (capture) out << capture;
    out << static_cast< char >('a' + col) << row + 1;
    if (promotion_piece) out << promotion_piece;
    if (check) out << check;
    if (mate) out << mate;
  }
}
