#ifndef POSITION_HPP
#define POSITION_HPP

#include <cstddef>
#include <iostream>
#include <ostream>
#include <string>

#include "square.hpp"
#include "piece.hpp"

namespace chess
{
  struct Move;

  char pieceToChar(Piece piece) noexcept;

  struct UndoInfo
  {
    Piece capturedPiece_;

    bool whiteKingCastling_ = 0;
    bool whiteQueenCastling_ = 0;
    bool blackKingCastling_ = 0;
    bool blackQueenCastling_ = 0;

    int enPassantSquare_ = -1;
  };

  struct Castling
  {
    bool king_;
    bool queen_;
  };

  struct Position
  {
  private:
    static bool isPieceChar(char c) noexcept;
    static bool isValidFen(const std::string& fen);

    Piece board_[64];
    bool whiteToMove_;

    bool whiteKingCastling_ = 0;
    bool whiteQueenCastling_ = 0;
    bool blackKingCastling_ = 0;
    bool blackQueenCastling_ = 0;

    int enPassantSquare_ = -1;

    int whiteKingSquare_ = -1;
    int blackKingSquare_ = -1;

  public:
    Position();
    Position(std::initializer_list< std::pair< Square, Piece > > pieces,
      bool whiteToMove = true, bool wkc = 0, bool wqc = 0, bool bkc = 0, bool bqc = 0);
    Position(const char* FEN);

    bool operator==(const Position& another) const noexcept;

    void setInitial() noexcept;
    void clear() noexcept;

    int getPiece(int square) const;
    int getEnPassantSquare() const;
    int getOppositeColourKingSquare() const;
    int getCurentColourKingSquare() const;
    int getWhiteKingSquare() const;
    int getBlackKingSquare() const;
    Castling getCastling() const;
    int getCastlingRights() const;
    bool isWhiteToMove() const noexcept;

    void makeMove(const Move& move, UndoInfo& undo) noexcept;
    void undoMove(const Move& move, const UndoInfo& undo) noexcept;

    Position getToggledSideToMovePosition() const;

    std::string toFEN() const;
    void print(std::ostream& out = std::cout, bool flipped = false) const;

    // для тестов
    void placePiece(int square, Piece piece);
    void removePiece(int square);
    void setEnPassantSquare(int square);
  };

  std::ostream& operator<<(std::ostream& out, const Position& pos);
}

#endif
