#include "evaluator.hpp"
#include "move_generator.hpp"
#include "piece.hpp"
#include "piece_square_tables.hpp"
#include "zobrist.hpp"

namespace chess
{
  Evaluator::Evaluator():
    coefficients_()
  {}

  Evaluator::Evaluator(const EvaluationCoefficients& coefficients):
    coefficients_(coefficients)
  {}

  int Evaluator::applyCoefficient(int eval, int coefficient)
  {
    return eval * coefficient / 100;
  }

  int Evaluator::evaluate(const Position& pos) const
  {
    int material_eval = 0;
    int mobility_eval = 0;
    int tables_eval = 0;
    int pawns_eval = 0;
    int king_eval = 0;

    int white_pawn_cols[8] = {};
    int black_pawn_cols[8] = {};

    for (int i = A1; i <= H8; ++i)
    {
      Piece cur = static_cast< Piece >(pos.getPiece(i));
      material(cur, material_eval);
      mobility(pos, i, cur, mobility_eval);
      piece_square_tables(i, cur, tables_eval);
      pawn_structure_fill(cur, i, white_pawn_cols, black_pawn_cols);
    }
    pawn_structure_eval(white_pawn_cols, black_pawn_cols, pawns_eval);
    king_safety(pos, king_eval);

    return applyCoefficient(material_eval, coefficients_.material_)
      + applyCoefficient(mobility_eval, coefficients_.mobility_)
      + applyCoefficient(tables_eval, coefficients_.pieceSquareTables_)
      + applyCoefficient(pawns_eval, coefficients_.pawnStructure_)
      + applyCoefficient(king_eval, coefficients_.kingSafety_);
  }

  int Evaluator::relative_eval(const Position& pos) const
  {
    int eval = evaluate(pos);
    return pos.isWhiteToMove() ? eval : -eval;
  }

  void Evaluator::material(Piece piece, int& eval)
  {
    if (piece > 0)
    {
      eval += weights[piece];
    }
    else if (piece < 0)
    {
      eval -= weights[-piece];
    }
  }

  void Evaluator::mobility(const Position &pos, int square, Piece piece, int &eval)
  {
    const int piece_color = piece > 0 ? 1 : -1;
    int abs_piece = piece * piece_color;
    switch (abs_piece)
    {
      case WHITE_QUEEN:
        eval += MoveGenerator::countPseudoLegalQueenMoves(pos, static_cast< Square >(square)) * piece_color * 1;
        break;
      case WHITE_KNIGHT:
        eval += MoveGenerator::countPseudoLegalKnightMoves(pos, static_cast< Square >(square)) * piece_color * 4;
        break;
      case WHITE_BISHOP:
        eval += MoveGenerator::countPseudoLegalBishopMoves(pos, static_cast< Square >(square)) * piece_color * 5;
        break;
      case WHITE_ROOK:
        eval += MoveGenerator::countPseudoLegalRookMoves(pos, static_cast< Square >(square)) * piece_color * 2;
        break;
    }
  }

  void Evaluator::piece_square_tables(int square, Piece piece, int& eval)
  {
    const int piece_color = piece > 0 ? 1 : -1;
    int abs_piece = piece * piece_color;
    switch (abs_piece)
    {
      case WHITE_KING:
        eval += king_table[piece_color == 1 ? square : square ^ 56] * piece_color;
        break;
      case WHITE_QUEEN:
        eval += queen_table[piece_color == 1 ? square : square ^ 56] * piece_color;
        break;
      case WHITE_KNIGHT:
        eval += knight_table[piece_color == 1 ? square : square ^ 56] * piece_color;
        break;
      case WHITE_BISHOP:
        eval += bishop_table[piece_color == 1 ? square : square ^ 56] * piece_color;
        break;
      case WHITE_PAWN:
        eval += pawn_table[piece_color == 1 ? square : square ^ 56] * piece_color;
        break;
    }
  }

  void Evaluator::pawn_structure_fill(Piece piece, int square, int* white, int* black)
  {
    if (piece == WHITE_PAWN)
    {
      white[square % 8] += 1;
    }
    else if (piece == BLACK_PAWN)
    {
      black[square % 8] += 1;
    }
  }

  void Evaluator::pawn_structure_eval(int *white, int *black, int& eval)
  {
    for (int i = 1; i < 7; ++i)
    {
      if (white[i] >= 2)
      {
        if (white[i + 1] == 0 && white[i - 1] == 0)
        {
          eval -= 40;
        }
        else
        {
          eval -= 15;
        }
      }
      if (black[i] >= 2)
      {
        if (black[i + 1] == 0 && black[i - 1] == 0)
        {
          eval += 40;
        }
        else
        {
          eval += 15;
        }
      }
    }
    if (white[0] >= 2) eval -= 40;
    if (black[0] >= 2) eval += 40;
    if (white[7] >= 2) eval -= 40;
    if (black[7] >= 2) eval += 40;
  }

  void Evaluator::king_safety(const Position &pos, int &eval)
  {
    const int white_king_square = pos.getWhiteKingSquare();
    const int black_king_square = pos.getBlackKingSquare();
    if (white_king_square % 8 != 3 && white_king_square % 8 != 4 && white_king_square % 8 != 5)
    {
      int row = white_king_square / 8;
      int col = white_king_square % 8;
      if (row < 2)
      {
        int c1 = col - 1;
        int c2 = col + 1;

        if (c1 < 0) c1 = 0;
        if (c2 > 7) c2 = 7;

        for (int r = row; r <= row + 1; ++r)
        {
          for (int c = c1; c <= c2; ++c)
          {
            int square = r * 8 + c;
            if (square == white_king_square)
                continue;
            if (pos.getPiece(square) == WHITE_PAWN)
            {
              eval += 5;
            }
          }
        }
      }
    }

    if (black_king_square % 8 != 3 && black_king_square % 8 != 4  && black_king_square % 8 != 5)
    {
      int row = black_king_square / 8;
      int col = black_king_square % 8;
      if (row > 5)
      {
        int c1 = col - 1;
        int c2 = col + 1;

        if (c1 < 0) c1 = 0;
        if (c2 > 7) c2 = 7;

        for (int r = row; r >= row - 1; --r)
        {
          for (int c = c1; c <= c2; ++c)
          {
            int square = r * 8 + c;
            if (square == black_king_square)
                continue;
            if (pos.getPiece(square) == BLACK_PAWN)
            {
              eval -= 5;
            }
          }
        }
      }
    }
  }
}
