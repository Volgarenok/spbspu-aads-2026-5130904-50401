#include "position.hpp"
#include "move.hpp"
#include "piece.hpp"

#include <sstream>
#include <stdexcept>

namespace chess
{
  bool Position::isPieceChar(char c) noexcept
  {
    return c == 'p' || c == 'n' || c == 'b' || c == 'r' || c == 'q' || c == 'k'
      || c == 'P' || c == 'N' || c == 'B' || c == 'R' || c == 'Q' || c == 'K';
  }

  bool Position::isValidFen(const std::string& fen)
  {
    std::stringstream stream(fen);
    std::string board;
    std::string color;
    std::string castling;
    std::string enPassant;
    stream >> board >> color >> castling >> enPassant;
    if (!stream)
    {
      return false;
    }

    int row_count = 1;
    int row_sum = 0;
    int white_king = 0;
    int black_king = 0;
    for (size_t i = 0; i < board.size(); ++i)
    {
      char c = board[i];
      if (c >= '1' && c <= '8')
      {
        row_sum += c - '0';
      }
      else if (isPieceChar(c))
      {
        ++row_sum;
        if (c == 'K')
        {
          ++white_king;
        }
        if (c == 'k')
        {
          ++black_king;
        }
      }
      else if (c == '/')
      {
        if (row_sum != 8)
        {
          return false;
        }
        row_sum = 0;
        ++row_count;
      }
      else
      {
        return false;
      }
      if (row_sum > 8 || row_count > 8)
      {
        return false;
      }
    }
    if (row_count != 8 || row_sum != 8 || white_king != 1 || black_king != 1)
    {
      return false;
    }
    if (color != "w" && color != "b")
    {
      return false;
    }
    if (castling != "-")
    {
      for (size_t i = 0; i < castling.size(); ++i)
      {
        char c = castling[i];
        if (c != 'K' && c != 'Q' && c != 'k' && c != 'q')
        {
          return false;
        }
      }
    }
    if (enPassant != "-")
    {
      if (enPassant.size() != 2 || enPassant[0] < 'a' || enPassant[0] > 'h'
        || enPassant[1] < '1' || enPassant[1] > '8')
      {
        return false;
      }
    }
    return true;
  }

  Position::Position()
  {
    clear();
  }

  Position::Position(std::initializer_list< std::pair< Square, Piece > > pieces, bool whiteToMove,
    bool wkc, bool wqc, bool bkc, bool bqc)
  {
    clear();
    for (auto it = pieces.begin(); it != pieces.end(); ++it)
    {
      Square square = it->first;
      Piece piece = it->second;
      if (piece == WHITE_KING)
      {
        whiteKingSquare_ = square;
      }
      if (piece == BLACK_KING)
      {
        blackKingSquare_ = square;
      }

      board_[square] = piece;
      if (piece == WHITE_KING)
      {
        whiteKingSquare_ = square;
      }
      else if (piece == BLACK_KING)
      {
        blackKingSquare_ = square;
      }
    }
    whiteToMove_ = whiteToMove;

    whiteKingCastling_ = wkc;
    whiteQueenCastling_ = wqc;
    blackKingCastling_ = bkc;
    blackQueenCastling_ = bqc;
  }

  Position::Position(const char* FEN)
  {
    if (!isValidFen(FEN))
    {
      throw std::logic_error("invalid FEN");
    }
    clear();
    std::stringstream stream(FEN);
    std::string section_board;
    stream >> section_board;
    int cur_square = 56;
    for (size_t i = 0; i < section_board.size(); ++i)
    {
      if (isdigit(section_board[i]))
      {
        cur_square += section_board[i] - '0';
      }
      else if (isalpha(section_board[i]))
      {
        board_[cur_square] = charToPiece(section_board[i]);
        if (section_board[i] == 'K') whiteKingSquare_ = cur_square;
        if (section_board[i] == 'k') blackKingSquare_ = cur_square;
        ++cur_square;
      }
      else
      {
        cur_square = ((cur_square / 8) - 2) * 8;
      }
    }

    char color;
    stream >> color;
    color == 'w' ? whiteToMove_ = 1 : whiteToMove_ = 0;

    std::string castlings;
    stream >> castlings;
    for (size_t i = 0; i < castlings.size(); ++i)
    {
      switch (castlings[i])
      {
        case 'K': whiteKingCastling_ = 1; break;
        case 'Q': whiteQueenCastling_ = 1; break;
        case 'k': blackKingCastling_ = 1; break;
        case 'q': blackQueenCastling_ = 1; break;
      }
    }

    std::string enPassantSquare;
    stream >> enPassantSquare;
    if (enPassantSquare != "-")
    {
      int col = enPassantSquare[0] - 'a';
      int row = enPassantSquare[1] - '1';
      enPassantSquare_ = row * 8 + col;
    }
    else
    {
      enPassantSquare_ = -1;
    }
  }

  std::string Position::toFEN() const
  {
    std::string fen;
    for (int row = 7; row >= 0; --row)
    {
      int empty = 0;
      for (int col = 0; col < 8; ++col)
      {
        Piece piece = static_cast< Piece >(getPiece(row * 8 + col));
        if (piece == EMPTY)
        {
          ++empty;
        }
        else
        {
          if (empty)
          {
            fen += static_cast< char >('0' + empty);
            empty = 0;
          }
          fen += pieceToChar(piece);
        }
      }
      if (empty)
      {
        fen += static_cast< char >('0' + empty);
      }
      if (row)
      {
        fen += '/';
      }
    }

    fen += isWhiteToMove() ? " w " : " b ";
    std::string castling;
    if (whiteKingCastling_) castling += 'K';
    if (whiteQueenCastling_) castling += 'Q';
    if (blackKingCastling_) castling += 'k';
    if (blackQueenCastling_) castling += 'q';
    fen += castling.empty() ? "-" : castling;
    fen += ' ';

    if (enPassantSquare_ == -1)
    {
      fen += '-';
    }
    else
    {
      fen += static_cast< char >('a' + enPassantSquare_ % 8);
      fen += static_cast< char >('1' + enPassantSquare_ / 8);
    }
    fen += " 0 1";
    return fen;
  }

  bool Position::operator==(const Position& another) const noexcept
  {
    for (int i = A1; i <= H8; ++i)
    {
      if (board_[i] != another.board_[i])
      {
        return false;
      }
    }
    if (whiteToMove_ != another.whiteToMove_) return false;

    if (whiteKingCastling_ != another.whiteKingCastling_) return false;
    if (whiteQueenCastling_ != another.whiteQueenCastling_) return false;
    if (blackKingCastling_ != another.blackKingCastling_) return false;
    if (blackQueenCastling_ != another.blackQueenCastling_) return false;

    if (enPassantSquare_ != another.enPassantSquare_) return false;

    if (whiteKingSquare_ != another.whiteKingSquare_) return false;
    if (blackKingSquare_ != another.blackKingSquare_) return false;

    return true;
  }

  void Position::clear() noexcept
  {
    for (size_t i = 0; i < 64; ++i)
    {
      board_[i] = EMPTY;
    }

    whiteToMove_ = true;

    whiteKingCastling_ = false;
    whiteQueenCastling_ = false;
    blackKingCastling_ = false;
    blackQueenCastling_ = false;

    enPassantSquare_ = -1;

    whiteKingSquare_ = -1;
    blackKingSquare_ = -1;
  }

  void Position::setInitial() noexcept
  {
    clear();

    for (size_t i = A2; i <= H2; ++i)
    {
      board_[i] = WHITE_PAWN;
    }

    for (size_t i = A7; i <= H7; ++i)
    {
      board_[i] = BLACK_PAWN;
    }

    board_[A1] = WHITE_ROOK;
    board_[B1] = WHITE_KNIGHT;
    board_[C1] = WHITE_BISHOP;
    board_[D1] = WHITE_QUEEN;
    board_[E1] = WHITE_KING;
    board_[F1] = WHITE_BISHOP;
    board_[G1] = WHITE_KNIGHT;
    board_[H1] = WHITE_ROOK;

    board_[A8] = BLACK_ROOK;
    board_[B8] = BLACK_KNIGHT;
    board_[C8] = BLACK_BISHOP;
    board_[D8] = BLACK_QUEEN;
    board_[E8] = BLACK_KING;
    board_[F8] = BLACK_BISHOP;
    board_[G8] = BLACK_KNIGHT;
    board_[H8] = BLACK_ROOK;

    whiteToMove_ = true;

    whiteKingCastling_ = true;
    whiteQueenCastling_ = true;
    blackKingCastling_ = true;
    blackQueenCastling_ = true;

    enPassantSquare_ = -1;

    whiteKingSquare_ = E1;
    blackKingSquare_ = E8;
  }

  int Position::getPiece(int square) const
  {
    return board_[square];
  }

  bool Position::isWhiteToMove() const noexcept
  {
    return whiteToMove_;
  }

  void Position::print(std::ostream& out, bool flipped) const
  {
    if (!flipped)
    {
      out << *this;
      return;
    }
    for (int row = 0; row < 8; ++row)
    {
      out << row + 1;
      for (int col = 7; col >= 0; --col)
      {
        out << " " << pieceToChar(static_cast< Piece >(getPiece(8 * row + col)));
      }
      out << "\n";
    }
    out << "  h g f e d c b a" << "\n";
    isWhiteToMove() ? out << "White " : out << "Black ";
    out << "to move\n";
  }

  std::ostream& operator<<(std::ostream& out, const Position& pos)
  {
    for (int row = 7; row >= 0; --row)
    {
      out << row + 1;
      for (int col = 0; col < 8; ++col)
      {
        out << " " << pieceToChar(static_cast< Piece >(pos.getPiece(8 * row + col)));
      }
      out << "\n";
    }
    out << "  a b c d e f g h" << "\n";
    pos.isWhiteToMove() ? out << "White " : out << "Black ";
    out << "to move\n";
    return out;
  }

  void Position::makeMove(const Move& move, UndoInfo& undo) noexcept
  {
    const int is_white_piece = isWhiteToMove() ? 1 : -1;
    if (getPiece(move.from_) == WHITE_KING * is_white_piece)
    {
      is_white_piece == 1 ? whiteKingSquare_ = move.to_ : blackKingSquare_ = move.to_;
    }

    undo.capturedPiece_ = board_[move.to_];
    board_[move.to_] = board_[move.from_];
    board_[move.from_] = EMPTY;
    whiteToMove_ = !whiteToMove_;

    undo.enPassantSquare_ = enPassantSquare_;
    enPassantSquare_ = -1;

    if (move.to_ == move.from_ + (16 * is_white_piece)
    && getPiece(move.to_) == WHITE_PAWN * is_white_piece)
    {
      enPassantSquare_ = move.from_ + (8 * is_white_piece);
    }

    if (move.promotionPiece_ != EMPTY)
    {
      board_[move.to_] = move.promotionPiece_;
    }

    if (move.isEnPassant_)
    {
      board_[move.to_ - (8 * is_white_piece)] = EMPTY;
    }

    undo.whiteKingCastling_ = whiteKingCastling_;
    undo.whiteQueenCastling_ = whiteQueenCastling_;
    undo.blackKingCastling_ = blackKingCastling_;
    undo.blackQueenCastling_ = blackQueenCastling_;

    if (move.isCastling_)
    {
      if (move.to_ - move.from_ == 2)
      {
        board_[move.to_ - 1] = static_cast< Piece >(WHITE_ROOK * is_white_piece);
        board_[move.to_ + 1] = EMPTY;
      }
      else if (move.from_ - move.to_ == 2)
      {
        board_[move.to_ + 1] = static_cast< Piece >(WHITE_ROOK * is_white_piece);
        board_[move.to_ - 2] = EMPTY;
      }
    }

    if (move.to_ == H1 || move.from_ == H1)
    {
      whiteKingCastling_ = 0;
    }
    if (move.to_ == A1 || move.from_ == A1)
    {
      whiteQueenCastling_ = 0;
    }
    if (move.to_ == H8 || move.from_ == H8)
    {
      blackKingCastling_ = 0;
    }
    if (move.to_ == A8 || move.from_ == A8)
    {
      blackQueenCastling_ = 0;
    }
    if (move.from_ == E1)
    {
      whiteKingCastling_ = 0;
      whiteQueenCastling_ = 0;
    }
    if (move.from_ == E8)
    {
      blackKingCastling_ = 0;
      blackQueenCastling_ = 0;
    }
  }

  void Position::undoMove(const Move& move, const UndoInfo& undo) noexcept
  {
    const int is_white_piece = isWhiteToMove() ? -1 : 1;
    if (getPiece(move.to_) == WHITE_KING * is_white_piece)
    {
      is_white_piece == 1 ? whiteKingSquare_ = move.from_ : blackKingSquare_ = move.from_;
    }

    board_[move.from_] = board_[move.to_];
    board_[move.to_] = undo.capturedPiece_;
    whiteToMove_ = !whiteToMove_;

    enPassantSquare_ = undo.enPassantSquare_;

    if (move.promotionPiece_ != EMPTY)
    {
      board_[move.from_] = static_cast< Piece >(WHITE_PAWN * is_white_piece);
    }

    if (move.isEnPassant_)
    {
      board_[move.to_ - (8 * is_white_piece)] = static_cast< Piece >(WHITE_PAWN * -is_white_piece);
    }

    whiteKingCastling_ = undo.whiteKingCastling_;
    whiteQueenCastling_ = undo.whiteQueenCastling_;
    blackKingCastling_ = undo.blackKingCastling_;
    blackQueenCastling_ = undo.blackQueenCastling_;

    if (move.isCastling_)
    {
      if (move.to_ - move.from_ == 2)
      {
        board_[move.to_ - 1] = EMPTY;
        board_[move.to_ + 1] = static_cast< Piece >(WHITE_ROOK * is_white_piece);
      }
      else if (move.from_ - move.to_ == 2)
      {
        board_[move.to_ + 1] = EMPTY;
        board_[move.to_ - 2] = static_cast< Piece >(WHITE_ROOK * is_white_piece);
      }
    }
  }

  void Position::placePiece(int square, Piece piece)
  {
    board_[square] = piece;
    const int is_white_piece = piece > 0 ? 1 : -1;
    if (piece == WHITE_KING * is_white_piece)
    {
      is_white_piece == 1 ? whiteKingSquare_ = square : blackKingSquare_ = square;
    }
  }

  void Position::removePiece(int square)
  {
    if (board_[square] == WHITE_KING)
    {
      whiteKingSquare_ = -1;
    }
    else if (board_[square] == BLACK_KING)
    {
      blackKingSquare_ = -1;
    }
    board_[square] = EMPTY;
  }

  int Position::getEnPassantSquare() const
  {
    return enPassantSquare_;
  }

  void Position::setEnPassantSquare(int square)
  {
    enPassantSquare_ = square;
  }

  int Position::getOppositeColourKingSquare() const
  {
    return isWhiteToMove() ? blackKingSquare_ : whiteKingSquare_;
  }

  int Position::getCurentColourKingSquare() const
  {
    return isWhiteToMove() ? whiteKingSquare_ : blackKingSquare_;
  }

  Castling Position::getCastling() const
  {
    if (whiteToMove_)
    {
      return Castling{whiteKingCastling_, whiteQueenCastling_};
    }
    return Castling{blackKingCastling_, blackQueenCastling_};
  }

  Position Position::getToggledSideToMovePosition() const
  {
    Position new_pos = *this;
    new_pos.whiteToMove_ = !new_pos.whiteToMove_;
    return new_pos;
  }

  int Position::getCastlingRights() const
  {
    return( whiteKingCastling_ * 8) + (whiteQueenCastling_ * 4) + (blackKingCastling_ * 2) + blackQueenCastling_;
  }

  int Position::getWhiteKingSquare() const
  {
    return whiteKingSquare_;
  }

  int Position::getBlackKingSquare() const
  {
    return blackKingSquare_;
  }
}
