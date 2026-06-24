#ifndef MOVE_GENERATOR_HPP
#define MOVE_GENERATOR_HPP

#include "position.hpp"
#include "move.hpp"

namespace chess
{
  struct MoveGenerator
  {
    static MoveArray generateLegalMoves(const Position& pos);
    static MoveArray generatePseudoLegalMoves(const Position& pos, bool castling = 1);

    static void generateKingMoves(const Position& pos, Square square, MoveArray& moves);
    static void generateQueenMoves(const Position& pos, Square square, MoveArray& moves);
    static void generateKnightMoves(const Position& pos, Square square, MoveArray& moves);
    static void generateBishopMoves(const Position& pos, Square square, MoveArray& moves);
    static void generateRookMoves(const Position& pos, Square square, MoveArray& moves);
    static void generatePawnMoves(const Position& pos, Square square, MoveArray& moves);
    static void generateCastlingMoves(const Position& pos, Square square, MoveArray& moves);

    static MoveArray generateActiveMoves(const Position& pos);
    static void generatePseudoLegalActiveMoves(const Position& pos, MoveArray& moves);

    static void generateQueenCaptures(const Position& pos, Square square, MoveArray& moves);
    static void generateBishopCaptures(const Position& pos, Square square, MoveArray& moves);
    static void generateRookCaptures(const Position& pos, Square square, MoveArray& moves);
    static void generateKnightCaptures(const Position& pos, Square square, MoveArray& moves);
    static void generateKingCaptures(const Position& pos, Square square, MoveArray& moves);
    static void generatePawnCapturesAndPromotions(const Position& pos, Square square, MoveArray& moves);

    static int countPseudoLegalQueenMoves(const Position& pos, Square square);
    static int countPseudoLegalBishopMoves(const Position& pos, Square square);
    static int countPseudoLegalRookMoves(const Position& pos, Square square);
    static int countPseudoLegalKnightMoves(const Position& pos, Square square);

    static bool isSquareAttacked(const Position& pos, Square square);
    static bool isSquareAttackedQuick(const Position& pos, Square square, bool byWhite);

    static Move findPawnAttacker(const Position& pos, int square, int side);
    static Move findKnightAttacker(const Position& pos, int square, int side);
    static Move findBishopAttacker(const Position& pos, int square, int side);
    static Move findRookAttacker(const Position& pos, int square, int side);
    static Move findQueenAttacker(const Position& pos, int square, int side);
    static Move findKingAttacker(const Position& pos, int square, int side);

    static bool isMate(const Position& pos);
    static bool isCheck(const Position& pos);
    static bool isStaleMate(const Position& pos);
  };
}

#endif
