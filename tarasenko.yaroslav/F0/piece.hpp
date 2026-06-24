#ifndef PIECE_HPP
#define PIECE_HPP

namespace chess
{
  enum Piece
  {
    EMPTY = 0,

    WHITE_PAWN = 1,
    WHITE_KNIGHT = 2,
    WHITE_BISHOP = 3,
    WHITE_ROOK = 4,
    WHITE_QUEEN = 5,
    WHITE_KING = 6,

    BLACK_PAWN = -1,
    BLACK_KNIGHT = -2,
    BLACK_BISHOP = -3,
    BLACK_ROOK = -4,
    BLACK_QUEEN = -5,
    BLACK_KING = -6
  };
  constexpr int weights[7] = {0, 100, 320, 330, 500, 900, 0};

  Piece charToPiece(char c) noexcept;
  char pieceToChar(Piece piece) noexcept;
}

#endif
