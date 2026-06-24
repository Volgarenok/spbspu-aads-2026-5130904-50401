#include <boost/test/unit_test.hpp>
#include "position.hpp"
#include "move.hpp"

using namespace chess;

BOOST_AUTO_TEST_CASE(default_constructor)
{
  Position empty_pos;
  for (size_t i = 0; i < 64; ++i)
  {
    BOOST_TEST(empty_pos.getPiece(i) == EMPTY);
  }
  BOOST_TEST(empty_pos.isWhiteToMove() == true);
}

BOOST_AUTO_TEST_CASE(setInitial)
{
  Position pos;
  pos.setInitial();
  BOOST_TEST(pos.isWhiteToMove() == true);
  BOOST_TEST(pos.getPiece(E1) == WHITE_KING);
  BOOST_TEST(pos.getPiece(F8) == BLACK_BISHOP);
  BOOST_TEST(pos.getPiece(G5) == EMPTY);
}

BOOST_AUTO_TEST_CASE(clear)
{
  Position pos;
  pos.setInitial();
  pos.clear();
  for (size_t i = 0; i < 64; ++i)
  {
    BOOST_TEST(pos.getPiece(i) == EMPTY);
  }
  BOOST_TEST(pos.isWhiteToMove() == true);
}

BOOST_AUTO_TEST_CASE(make_moves)
{
  Position pos;
  pos.setInitial();
  UndoInfo undo;
  pos.makeMove({E2, E4}, undo);
  BOOST_TEST(pos.getPiece(E4) == WHITE_PAWN);
  BOOST_TEST(!pos.isWhiteToMove());
  pos.makeMove({D7, D5}, undo);
  BOOST_TEST(pos.getPiece(D5) == BLACK_PAWN);
  BOOST_TEST(pos.isWhiteToMove());
  pos.makeMove({E4, D5}, undo);
  BOOST_TEST(pos.getPiece(D5) == WHITE_PAWN);
  BOOST_TEST(pos.getPiece(E4) == EMPTY);
}

BOOST_AUTO_TEST_CASE(undo_moves)
{
  Position pos;
  pos.setInitial();
  UndoInfo undo [3];
  Move moves [3] = {{E2, E4}, {D7, D5}, {E4, D5}};
  for (size_t i = 0; i < 3; ++i)
  {
    pos.makeMove(moves[i], undo[i]);
  }
  pos.undoMove(moves[2], undo[2]);
  BOOST_TEST(pos.getPiece(E4) == WHITE_PAWN);
  BOOST_TEST(pos.getPiece(D5) == BLACK_PAWN);
  BOOST_TEST(pos.isWhiteToMove());
  pos.undoMove(moves[1], undo[1]);
  BOOST_TEST(pos.getPiece(D5) == EMPTY);
  BOOST_TEST(pos.getPiece(D7) == BLACK_PAWN);
  BOOST_TEST(!pos.isWhiteToMove());
  pos.undoMove(moves[0], undo[0]);
  BOOST_TEST(pos.getPiece(E4) == EMPTY);
  BOOST_TEST(pos.getPiece(E2) == WHITE_PAWN);
}

BOOST_AUTO_TEST_CASE(fen_initialization)
{
  Position pos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  Position init_pos;
  init_pos.setInitial();
  BOOST_TEST(pos == init_pos);
}
