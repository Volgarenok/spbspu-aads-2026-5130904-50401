#include "zobrist.hpp"
#include "piece.hpp"
#include <cstdint>
#include <random>
#include <sys/types.h>

namespace chess
{
  uint64_t zobrist_board[64 * 12];
  uint64_t zobrist_side;
  uint64_t zobrist_castling[16];
  uint64_t zobrist_enpassant[64];

  void initZobristHash()
  {
    std::mt19937_64 rng(67);
    for (int i = 0; i < 64; i++)
    {
      for (int j = 0; j < 12; j++)
      {
        zobrist_board[i * 12 + j] = rng();
      }
    }
    zobrist_side = rng();
    for (int i = 0; i < 16; i++)
    {
      zobrist_castling[i] = rng();
    }
    for (int i = 0; i < 64; i++)
    {
      zobrist_enpassant[i] = rng();
    }
  }

  uint64_t zobristHash(const Position &pos)
  {
    uint64_t hash = 0;
    for (int i = 0; i < 64; i++)
    {
      int cur = pos.getPiece(i);
      if (cur != 0)
      {
        hash ^= zobrist_board[i * 12 + pieceIndex(cur)];
      }
    }
    if (pos.isWhiteToMove())
    {
      hash ^= zobrist_side;
    }
    hash ^= zobrist_castling[pos.getCastlingRights()];
    int enPassant = pos.getEnPassantSquare();
    if (enPassant != -1)
    {
      hash ^= zobrist_enpassant[enPassant];
    }
    return hash;
  }

  uint64_t incrementZobristHash(uint64_t hash, const Position &pos, const Move &move)
  {
    const int moving_piece = pos.getPiece(move.from_);
    const int is_white_piece = pos.isWhiteToMove() ? 1 : -1;
    const int old_en_passant = pos.getEnPassantSquare();
    const int old_castling_rights = pos.getCastlingRights();

    hash ^= zobrist_board[move.from_ * 12 + pieceIndex(moving_piece)];

    if (move.isEnPassant_)
    {
      const int captured_square = move.to_ - (8 * is_white_piece);
      const int captured_piece = WHITE_PAWN * -is_white_piece;
      hash ^= zobrist_board[captured_square * 12 + pieceIndex(captured_piece)];
    }
    else
    {
      const int captured_piece = pos.getPiece(move.to_);
      if (captured_piece != EMPTY)
      {
        hash ^= zobrist_board[move.to_ * 12 + pieceIndex(captured_piece)];
      }
    }

    const int placed_piece = move.promotionPiece_ != EMPTY ? move.promotionPiece_ : moving_piece;
    hash ^= zobrist_board[move.to_ * 12 + pieceIndex(placed_piece)];

    if (move.isCastling_)
    {
      const int rook_piece = WHITE_ROOK * is_white_piece;
      if (move.to_ - move.from_ == 2)
      {
        hash ^= zobrist_board[(move.to_ + 1) * 12 + pieceIndex(rook_piece)];
        hash ^= zobrist_board[(move.to_ - 1) * 12 + pieceIndex(rook_piece)];
      }
      else if (move.from_ - move.to_ == 2)
      {
        hash ^= zobrist_board[(move.to_ - 2) * 12 + pieceIndex(rook_piece)];
        hash ^= zobrist_board[(move.to_ + 1) * 12 + pieceIndex(rook_piece)];
      }
    }

    hash ^= zobrist_side;

    hash ^= zobrist_castling[old_castling_rights];
    int new_castling_rights = old_castling_rights;
    if (move.to_ == H1 || move.from_ == H1)
    {
      new_castling_rights &= ~8;
    }
    if (move.to_ == A1 || move.from_ == A1)
    {
      new_castling_rights &= ~4;
    }
    if (move.to_ == H8 || move.from_ == H8)
    {
      new_castling_rights &= ~2;
    }
    if (move.to_ == A8 || move.from_ == A8)
    {
      new_castling_rights &= ~1;
    }
    if (move.from_ == E1)
    {
      new_castling_rights &= ~(8 | 4);
    }
    if (move.from_ == E8)
    {
      new_castling_rights &= ~(2 | 1);
    }
    hash ^= zobrist_castling[new_castling_rights];

    if (old_en_passant != -1)
    {
      hash ^= zobrist_enpassant[old_en_passant];
    }

    int new_en_passant = -1;
    if (move.to_ == move.from_ + (16 * is_white_piece)
      && moving_piece == WHITE_PAWN * is_white_piece)
    {
      new_en_passant = move.from_ + (8 * is_white_piece);
    }
    if (new_en_passant != -1)
    {
      hash ^= zobrist_enpassant[new_en_passant];
    }

    return hash;
  }

  int pieceIndex(int piece)
  {
    return piece > 0 ? piece + 5 : piece + 6;
  }
}
