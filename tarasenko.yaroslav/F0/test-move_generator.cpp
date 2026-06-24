#include <boost/test/unit_test.hpp>
#include "move_generator.hpp"

using namespace chess;

BOOST_AUTO_TEST_CASE(king_moves)
{
  Position pos;
  pos.setInitial();
  MoveArray moves;
  MoveGenerator generator;
  generator.generateKingMoves(pos, E1, moves);
  BOOST_TEST(moves.size() == 0);

  UndoInfo undo;
  pos.makeMove({E2, E4}, undo);
  generator.generateKingMoves(pos, E1, moves);
  BOOST_TEST(moves.get(0) == Move({E1, E2}));
  BOOST_TEST(moves.size() == 1);

  pos.clear();
  pos.placePiece(D4, WHITE_KING);
  moves.clear();
  generator.generateKingMoves(pos, D4, moves);
  Square squares[8] = {D5, E5, E4, E3, D3, C3, C4, C5};
  for (size_t i = 0; i < 8; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({D4, squares[i]}));
  }

  pos.clear();
  pos.placePiece(D4, WHITE_KING);
  pos.placePiece(E4, WHITE_PAWN);
  moves.clear();
  generator.generateKingMoves(pos, D4, moves);
  Square squares2[7] = {D5, E5, E3, D3, C3, C4, C5};
  for (size_t i = 0; i < 7; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({D4, squares2[i]}));
  }

  pos.clear();
  pos.placePiece(D4, WHITE_KING);
  pos.placePiece(E4, BLACK_PAWN);
  moves.clear();
  generator.generateKingMoves(pos, D4, moves);
  Square squares3[8] = {D5, E5, E4, E3, D3, C3, C4, C5};
  for (size_t i = 0; i < 8; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({D4, squares3[i]}));
  }

  pos.clear();
  pos.placePiece(H2, WHITE_KING);
  moves.clear();
  generator.generateKingMoves(pos, H2, moves);
  Square squares4[5] = {H3, H1, G1, G2, G3};
  for (size_t i = 0; i < 5; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({H2, squares4[i]}));
  }

  pos.clear();
  pos.placePiece(D4, BLACK_KING);
  pos.placePiece(E4, BLACK_PAWN);
  moves.clear();
  generator.generateKingMoves(pos, D4, moves);
  Square squares5[7] = {D5, E5, E3, D3, C3, C4, C5};
  for (size_t i = 0; i < 7; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({D4, squares5[i]}));
  }

  pos.clear();
  pos.placePiece(D4, BLACK_KING);
  pos.placePiece(E4, WHITE_PAWN);
  moves.clear();
  generator.generateKingMoves(pos, D4, moves);
  Square squares6[8] = {D5, E5, E4, E3, D3, C3, C4, C5};
  for (size_t i = 0; i < 8; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({D4, squares6[i]}));
  }
}

BOOST_AUTO_TEST_CASE(rook_moves)
{
  Position pos;
  pos.setInitial();
  MoveArray moves;
  MoveGenerator generator;
  generator.generateRookMoves(pos, A1, moves);
  generator.generateRookMoves(pos, H1, moves);
  BOOST_TEST(moves.size() == 0);

  pos.clear();
  pos.placePiece(D4, WHITE_ROOK);
  moves.clear();
  generator.generateRookMoves(pos, D4, moves);
  Square squares[14] = {D5, D6, D7, D8, D3, D2, D1,
  E4, F4, G4, H4, C4, B4, A4};
  for (size_t i = 0; i < 14; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({D4, squares[i]}));
  }
  BOOST_TEST(moves.size() == 14);

  pos.clear();
  pos.placePiece(D4, WHITE_ROOK);
  pos.placePiece(B4, WHITE_PAWN);
  pos.placePiece(D6, BLACK_PAWN);
  moves.clear();
  generator.generateRookMoves(pos, D4, moves);
  Square squares2[10] = {D5, D6, D3, D2, D1, E4, F4, G4, H4, C4};
  for (size_t i = 0; i < 10; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({D4, squares2[i]}));
  }
  BOOST_TEST(moves.size() == 10);

  pos.clear();
  pos.placePiece(D4, BLACK_ROOK);
  pos.placePiece(B4, BLACK_PAWN);
  pos.placePiece(D6, WHITE_PAWN);
  moves.clear();
  generator.generateRookMoves(pos, D4, moves);
  Square squares3[10] = {D5, D6, D3, D2, D1, E4, F4, G4, H4, C4};
  for (size_t i = 0; i < 10; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({D4, squares3[i]}));
  }
  BOOST_TEST(moves.size() == 10);
}

BOOST_AUTO_TEST_CASE(knight_moves)
{
  Position pos;
  pos.setInitial();
  MoveArray moves;
  MoveGenerator generator;
  generator.generateKnightMoves(pos, B1, moves);
  generator.generateKnightMoves(pos, G1, moves);
  Square squares[4] = {C3, A3, H3, F3};
  for (size_t i = 0; i < 4; ++i)
  {
    BOOST_TEST(moves.get(i).to_ == squares[i]);
  }
  BOOST_TEST(moves.size() == 4);

  pos.clear();
  pos.placePiece(D4, WHITE_KNIGHT);
  moves.clear();
  generator.generateKnightMoves(pos, D4, moves);
  Square squares1[8] = {E6, F5, F3, E2, C2, B3, B5, C6};
  for (size_t i = 0; i < 8; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({D4, squares1[i]}));
  }

  pos.clear();
  pos.placePiece(D4, WHITE_KNIGHT);
  pos.placePiece(F3, WHITE_PAWN);
  moves.clear();
  generator.generateKnightMoves(pos, D4, moves);
  Square squares2[7] = {E6, F5, E2, C2, B3, B5, C6};
  for (size_t i = 0; i < 7; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({D4, squares2[i]}));
  }

  pos.clear();
  pos.placePiece(D4, WHITE_KNIGHT);
  pos.placePiece(F3, BLACK_PAWN);
  moves.clear();
  generator.generateKnightMoves(pos, D4, moves);
  Square squares3[8] = {E6, F5, F3, E2, C2, B3, B5, C6};
  for (size_t i = 0; i < 8; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({D4, squares3[i]}));
  }

  pos.clear();
  pos.placePiece(H1, WHITE_KNIGHT);
  moves.clear();
  generator.generateKnightMoves(pos, H1, moves);
  Square squares4[2] = {F2, G3};
  for (size_t i = 0; i < 2; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({H1, squares4[i]}));
  }

  pos.setInitial();
  moves.clear();
  generator.generateKnightMoves(pos, B8, moves);
  generator.generateKnightMoves(pos, G8, moves);
  Square squares6[4] = {C6, A6, H6, F6};
  for (size_t i = 0; i < 4; ++i)
  {
    BOOST_TEST(moves.get(i).to_ == squares6[i]);
  }
}

BOOST_AUTO_TEST_CASE(bishop_moves)
{
  Position pos;
  pos.setInitial();
  MoveArray moves;
  MoveGenerator generator;
  generator.generateBishopMoves(pos, C1, moves);
  generator.generateBishopMoves(pos, F1, moves);
  BOOST_TEST(moves.size() == 0);

  pos.clear();
  pos.placePiece(D4, WHITE_BISHOP);
  moves.clear();
  generator.generateBishopMoves(pos, D4, moves);
  Square squares[13] = {E5, F6, G7, H8, E3, F2, G1, C3, B2, A1, C5, B6, A7};
  for (size_t i = 0; i < 13; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({D4, squares[i]}));
  }
  BOOST_TEST(moves.size() == 13);

  pos.clear();
  pos.placePiece(E4, WHITE_BISHOP);
  pos.placePiece(C6, BLACK_PAWN);
  pos.placePiece(F5, WHITE_PAWN);
  moves.clear();
  Square squares2[8] = {F3, G2, H1, D3, C2, B1, D5, C6};
  generator.generateBishopMoves(pos, E4, moves);
  for (size_t i = 0; i < 8; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({E4, squares2[i]}));
  }
  BOOST_TEST(moves.size() == 8);

  pos.clear();
  pos.placePiece(E4, BLACK_BISHOP);
  pos.placePiece(C6, WHITE_PAWN);
  pos.placePiece(F5, BLACK_PAWN);
  moves.clear();
  generator.generateBishopMoves(pos, E4, moves);
  for (size_t i = 0; i < 8; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({E4, squares2[i]}));
  }
  BOOST_TEST(moves.size() == 8);
}

BOOST_AUTO_TEST_CASE(queen_moves)
{
  Position pos;
  pos.setInitial();
  MoveArray moves;
  MoveGenerator generator;
  generator.generateQueenMoves(pos, D1, moves);
  BOOST_TEST(moves.size() == 0);

  pos.clear();
  pos.placePiece(A1, WHITE_QUEEN);
  pos.placePiece(F6, BLACK_PAWN);
  pos.placePiece(G1, WHITE_ROOK);
  moves.clear();
  generator.generateQueenMoves(pos, A1, moves);
  Square squares[17] = {A2, A3, A4, A5, A6, A7, A8, B1, C1, D1, E1, F1, B2, C3, D4, E5, F6};
  for (size_t i = 0; i < 17; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({A1, squares[i]}));
  }
  BOOST_TEST(moves.size() == 17);

  pos.clear();
  pos.placePiece(A1, BLACK_QUEEN);
  pos.placePiece(F6, WHITE_PAWN);
  pos.placePiece(G1, BLACK_ROOK);
  moves.clear();
  generator.generateQueenMoves(pos, A1, moves);
  for (size_t i = 0; i < 17; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({A1, squares[i]}));
  }
  BOOST_TEST(moves.size() == 17);
}

BOOST_AUTO_TEST_CASE(pawn_moves)
{
  Position pos;
  pos.setInitial();
  MoveArray moves;
  MoveGenerator generator;
  generator.generatePawnMoves(pos, A2, moves);
  generator.generatePawnMoves(pos, B2, moves);
  generator.generatePawnMoves(pos, H2, moves);
  generator.generatePawnMoves(pos, A7, moves);
  generator.generatePawnMoves(pos, B7, moves);
  generator.generatePawnMoves(pos, H7, moves);
  Square squares[12] = {A3, A4, B3, B4, H3, H4, A6, A5, B6, B5, H6, H5};
  Square init_squares[12] = {A2, A2, B2, B2, H2, H2, A7, A7, B7, B7, H7, H7};
  for (size_t i = 0; i < 12; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({init_squares[i], squares[i]}));
  }
  BOOST_TEST(moves.size() == 12);

  pos.clear();
  moves.clear();
  pos.placePiece(D4, WHITE_PAWN);
  pos.placePiece(D5, WHITE_PAWN);
  generator.generatePawnMoves(pos, D4, moves);
  BOOST_TEST(moves.size() == 0);
}

BOOST_AUTO_TEST_CASE(pawn_captures)
{
  Position pos;
  MoveArray moves;
  MoveGenerator generator;
  pos.placePiece(D4, WHITE_PAWN);
  pos.placePiece(E5, BLACK_PAWN);
  pos.placePiece(C5, BLACK_BISHOP);
  pos.placePiece(F4, WHITE_BISHOP);
  generator.generatePawnMoves(pos, D4, moves);
  generator.generatePawnMoves(pos, E5, moves);
  Square squares[6] = {D5, E5, C5, E4, D4, F4};
  Square init_squares[6] = {D4, D4, D4, E5, E5, E5};
  for (size_t i = 0; i < 6; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({init_squares[i], squares[i]}));
  }
  BOOST_TEST(moves.size() == 6);

  pos.clear();
  moves.clear();
  pos.placePiece(D4, WHITE_PAWN);
  pos.placePiece(D5, BLACK_PAWN);
  pos.placePiece(E5, WHITE_PAWN);
  pos.placePiece(C5, BLACK_BISHOP);
  pos.placePiece(E4, WHITE_BISHOP);
  generator.generatePawnMoves(pos, D4, moves);
  generator.generatePawnMoves(pos, D5, moves);
  Square squares2[2] = {C5, E4};
  BOOST_TEST(moves.get(0) == Move({D4, squares2[0]}));
  BOOST_TEST(moves.get(1) == Move({D5, squares2[1]}));
  BOOST_TEST(moves.size() == 2);

  pos.clear();
  moves.clear();
  pos.placePiece(H2, WHITE_PAWN);
  pos.placePiece(G3, BLACK_PAWN);
  pos.placePiece(H2 + 9, BLACK_PAWN);
  pos.placePiece(G1, BLACK_ROOK);
  generator.generatePawnMoves(pos, H2, moves);
  Square squares3[3] = {H3, H4, G3};
  for (size_t i = 0; i < 3; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({H2, squares3[i]}));
  }
  BOOST_TEST(moves.size() == 3);

  pos.clear();
  moves.clear();
  pos.placePiece(A2, WHITE_PAWN);
  pos.placePiece(B3, BLACK_QUEEN);
  pos.placePiece(A4, BLACK_PAWN);
  pos.placePiece(A2 + 7, BLACK_PAWN);
  generator.generatePawnMoves(pos, A2, moves);
  Square squares4[2] = {A3, B3};
  for (size_t i = 0; i < 2; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({A2, squares4[i]}));
  }
  BOOST_TEST(moves.size() == 2);

  pos.clear();
  moves.clear();
  pos.placePiece(H7, BLACK_PAWN);
  pos.placePiece(G6, WHITE_PAWN);
  pos.placePiece(H7 - 7, BLACK_PAWN);
  pos.placePiece(G8, WHITE_ROOK);
  generator.generatePawnMoves(pos, H7, moves);
  Square squares5[3] = {H6, H5, G6};
  for (size_t i = 0; i < 3; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({H7, squares5[i]}));
  }
  BOOST_TEST(moves.size() == 3);

  pos.clear();
  moves.clear();
  pos.placePiece(A7, BLACK_PAWN);
  pos.placePiece(B6, WHITE_KNIGHT);
  pos.placePiece(A5, WHITE_PAWN);
  pos.placePiece(A7 - 9, WHITE_PAWN);
  generator.generatePawnMoves(pos, A7, moves);
  Square squares6[2] = {A6, B6};
  for (size_t i = 0; i < 2; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({A7, squares6[i]}));
  }
  BOOST_TEST(moves.size() == 2);
}

BOOST_AUTO_TEST_CASE(pawn_promotions)
{
  Position pos;
  MoveArray moves;
  MoveGenerator generator;
  pos.placePiece(E7, WHITE_PAWN);
  generator.generatePawnMoves(pos, E7, moves);
  Piece pieces[4] = {WHITE_QUEEN, WHITE_KNIGHT, WHITE_ROOK, WHITE_BISHOP};
  for (size_t i = 0; i < 4; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({E7, E8, pieces[i]}));
  }
  BOOST_TEST(moves.size() == 4);

  pos.clear();
  moves.clear();
  pos.placePiece(E2, BLACK_PAWN);
  generator.generatePawnMoves(pos, E2, moves);
  Piece pieces2[4] = {BLACK_QUEEN, BLACK_KNIGHT, BLACK_ROOK, BLACK_BISHOP};
  for (size_t i = 0; i < 4; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({E2, E1, pieces2[i]}));
  }
  BOOST_TEST(moves.size() == 4);
}

BOOST_AUTO_TEST_CASE(pawn_promotions_with_capture)
{
  Position pos;
  MoveArray moves;
  MoveGenerator generator;
  pos.placePiece(E7, WHITE_PAWN);
  pos.placePiece(F8, BLACK_KNIGHT);
  pos.placePiece(D8, WHITE_KNIGHT);
  pos.placePiece(H7, WHITE_PAWN);
  pos.placePiece(G8, BLACK_KNIGHT);
  pos.placePiece(H8, WHITE_BISHOP);
  generator.generatePawnMoves(pos, E7, moves);
  generator.generatePawnMoves(pos, H7, moves);
  Piece pieces[4] = {WHITE_QUEEN, WHITE_KNIGHT, WHITE_ROOK, WHITE_BISHOP};
  for (size_t i = 0; i < 4; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({E7, E8, pieces[i]}));
    BOOST_TEST(moves.get(i + 4) == Move({E7, F8, pieces[i]}));
    BOOST_TEST(moves.get(i + 8) == Move({H7, G8, pieces[i]}));
  }
  BOOST_TEST(moves.size() == 12);

  pos.clear();
  moves.clear();
  pos.placePiece(E2, BLACK_PAWN);
  pos.placePiece(D1, WHITE_KNIGHT);
  pos.placePiece(F1, BLACK_KNIGHT);
  pos.placePiece(A2, BLACK_PAWN);
  pos.placePiece(B1, WHITE_KNIGHT);
  generator.generatePawnMoves(pos, E2, moves);
  generator.generatePawnMoves(pos, A2, moves);
  Piece pieces2[4] = {BLACK_QUEEN, BLACK_KNIGHT, BLACK_ROOK, BLACK_BISHOP};
  for (size_t i = 0; i < 4; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({E2, E1, pieces2[i]}));
    BOOST_TEST(moves.get(i + 4) == Move({E2, D1, pieces2[i]}));
    BOOST_TEST(moves.get(i + 8) == Move({A2, A1, pieces2[i]}));
    BOOST_TEST(moves.get(i + 12) == Move({A2, B1, pieces2[i]}));
  }
  BOOST_TEST(moves.size() == 16);
}

BOOST_AUTO_TEST_CASE(en_passant)
{
  Position pos;
  MoveArray moves;
  MoveGenerator generator;
  pos.setEnPassantSquare(E6);
  pos.placePiece(D5, WHITE_PAWN);
  pos.placePiece(F5, WHITE_PAWN);
  pos.placePiece(E5, BLACK_PAWN);
  pos.placePiece(D6, BLACK_PAWN);
  pos.placePiece(F6, BLACK_PAWN);
  generator.generatePawnMoves(pos, D5, moves);
  generator.generatePawnMoves(pos, F5, moves);
  BOOST_TEST(moves.get(0) == Move({D5, E6, EMPTY, true}));
  BOOST_TEST(moves.get(1) == Move({F5, E6, EMPTY, true}));
  BOOST_TEST(moves.size() == 2);

  pos.clear();
  moves.clear();
  pos.setEnPassantSquare(G3);
  pos.placePiece(H4, BLACK_PAWN);
  pos.placePiece(G4, WHITE_PAWN);
  pos.placePiece(A4, WHITE_PAWN);
  pos.placePiece(H3, WHITE_PAWN);
  generator.generatePawnMoves(pos, H4, moves);
  BOOST_TEST(moves.get(0) == Move({H4, G3, EMPTY, true}));

  pos.clear();
  moves.clear();
  pos.setEnPassantSquare(D6);
  pos.placePiece(E5, WHITE_PAWN);
  pos.placePiece(D5, BLACK_PAWN);
  pos.placePiece(F6, BLACK_PAWN);
  generator.generatePawnMoves(pos, E5, moves);
  Square squares[3] = {E6, F6, D6};
  bool enPassants[3] = {false, false, true};
  for (size_t i = 0; i < 3; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({E5, squares[i], EMPTY, enPassants[i]}));
  }
  BOOST_TEST(moves.size() == 3);
}

BOOST_AUTO_TEST_CASE(generate_pseudo_legal_moves)
{
  Position pos({
    {H1, WHITE_KNIGHT}, {F2, WHITE_PAWN}, {G3, WHITE_PAWN}, {F3, BLACK_PAWN}, {G4, BLACK_PAWN}
  });
  MoveGenerator generator;
  MoveArray moves = generator.generatePseudoLegalMoves(pos);
  BOOST_TEST(moves.size() == 0);

  Position pos2({
    {H1, WHITE_KING}, {G8, BLACK_ROOK}, {G1, WHITE_KNIGHT}
  });
  moves.clear();
  moves = generator.generatePseudoLegalMoves(pos2);
  BOOST_TEST(moves.size() == 5);
}

BOOST_AUTO_TEST_CASE(generate_pseudo_legal_moves_in_initial)
{
  Position pos;
  MoveGenerator generator;
  pos.setInitial();
  MoveArray moves = generator.generatePseudoLegalMoves(pos);
  Square squares[20] = {C3, A3, H3, F3, A3, A4, B3, B4, C3, C4, D3, D4,
  E3, E4, F3, F4, G3, G4, H3, H4};
  for (size_t i = 0; i < 2; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({B1, squares[i]}));
  }
  for (size_t i = 2; i < 4; ++i)
  {
    BOOST_TEST(moves.get(i) == Move({G1, squares[i]}));
  }
  for (size_t i = 0, k = 0; i < 8; i += 2, ++k)
  {
    Square square_from = static_cast< Square >(A2 + k);
    BOOST_TEST(moves.get(i + 4) == Move({square_from, squares[i + 4]}));
    BOOST_TEST(moves.get(i + 5) == Move({square_from, squares[i + 5]}));
  }
}

BOOST_AUTO_TEST_CASE(generate_legal_moves)
{
  Position pos({
    {H1, WHITE_KING}, {G8, BLACK_ROOK}
  });
  MoveGenerator generator;
  MoveArray moves = generator.generateLegalMoves(pos);
  BOOST_TEST(containsMove(moves, Move{H1, H2}));
  BOOST_TEST(moves.size() == 1);

  Position pos2({
    {H1, WHITE_KING}, {H2, WHITE_KNIGHT}, {G8, BLACK_ROOK}, {H8, BLACK_ROOK}
  });
  moves.clear();
  moves = generator.generateLegalMoves(pos2);
  BOOST_TEST(moves.size() == 0);

  Position pos3({
    {H1, WHITE_KING}, {H2, WHITE_ROOK}, {H4, BLACK_ROOK}, {A2, BLACK_ROOK}
  });
  moves.clear();
  moves = generator.generateLegalMoves(pos3);
  BOOST_TEST(containsMove(moves, Move{H2, H3}));
  BOOST_TEST(containsMove(moves, Move{H2, H4}));
  BOOST_TEST(containsMove(moves, Move{H1, G1}));
  BOOST_TEST(moves.size() == 3);

  Position pos4({
    {H1, WHITE_KING}, {H2, WHITE_PAWN}, {H5, BLACK_ROOK}, {G3, BLACK_ROOK}
  });
  moves.clear();
  moves = generator.generateLegalMoves(pos4);
  BOOST_TEST(containsMove(moves, Move{H2, H3}));
  BOOST_TEST(containsMove(moves, Move{H2, H4}));
  BOOST_TEST(moves.size() == 2);

  Position pos5({
    {H1, WHITE_KING}, {A2, WHITE_ROOK}, {H5, BLACK_ROOK}, {G3, BLACK_ROOK}
  });
  moves.clear();
  moves = generator.generateLegalMoves(pos5);
  BOOST_TEST(containsMove(moves, Move{A2, H2}));
  BOOST_TEST(moves.size() == 1);
}

BOOST_AUTO_TEST_CASE(generate_legal_moves_in_initial)
{
  Position pos;
  MoveGenerator generator;
  pos.setInitial();
  MoveArray pseudo = generator.generatePseudoLegalMoves(pos);
  MoveArray legal = generator.generateLegalMoves(pos);
  BOOST_TEST(isEqualArraysUnordered(pseudo, legal));
}

BOOST_AUTO_TEST_CASE(make_promotion_move)
{
  Position pos({
    {E7, WHITE_PAWN}
  });
  MoveGenerator generator;
  UndoInfo undo;
  MoveArray moves = generator.generateLegalMoves(pos);
  pos.makeMove(moves.get(0), undo);
  BOOST_TEST(pos.getPiece(E8) == WHITE_QUEEN);
  BOOST_TEST(moves.size() == 4);
}

BOOST_AUTO_TEST_CASE(undo_promotion_move)
{
  Position pos({
    {E7, WHITE_PAWN}
  });
  MoveGenerator generator;
  UndoInfo undo;
  MoveArray moves = generator.generateLegalMoves(pos);
  pos.makeMove(moves.get(0), undo);
  pos.undoMove(moves.get(0), undo);
  BOOST_TEST(pos.getPiece(E7) == WHITE_PAWN);
  BOOST_TEST(pos.getPiece(E8) == EMPTY);
  BOOST_TEST(isEqualArraysUnordered(moves, generator.generateLegalMoves(pos)));
}

BOOST_AUTO_TEST_CASE(make_en_passant_move)
{
  Position pos({
    {E5, WHITE_PAWN}, {D5, BLACK_PAWN}, {E6, BLACK_PAWN}
  });
  pos.setEnPassantSquare(D6);
  MoveGenerator generator;
  MoveArray moves = generator.generateLegalMoves(pos);
  UndoInfo undo;
  pos.makeMove(moves.get(0), undo);
  BOOST_TEST(pos.getPiece(D5) == 0);
}

BOOST_AUTO_TEST_CASE(undo_en_passant_move)
{
  Position pos({
    {E5, WHITE_PAWN}, {D7, BLACK_PAWN}, {A2, WHITE_BISHOP}, {G8, BLACK_KING}
  }, false);
  MoveGenerator generator;
  MoveArray black_moves = generator.generateLegalMoves(pos);
  UndoInfo undo;
  pos.makeMove(getMove(black_moves, D7, D5), undo);
  MoveArray white_moves = generator.generateLegalMoves(pos);
  pos.makeMove(getMove(white_moves, E5, D6), undo);
  BOOST_TEST(undo.enPassantSquare_ == D6);
  BOOST_TEST(generator.generateLegalMoves(pos).size() == 4);
  pos.undoMove(getMove(white_moves, E5, D6), undo);
  BOOST_TEST(isEqualArraysUnordered(white_moves, generator.generateLegalMoves(pos)));
}

BOOST_AUTO_TEST_CASE(castling)
{
  Position pos({
    {E1, WHITE_KING}, {A1, WHITE_ROOK}, {H1, WHITE_ROOK}
  }, 1, 1, 1);
  MoveGenerator generator;
  MoveArray moves = generator.generateLegalMoves(pos);
  BOOST_TEST(containsMove(moves, getMove(moves, E1, G1)));
  BOOST_TEST(containsMove(moves, getMove(moves, E1, C1)));

  pos.placePiece(C1, WHITE_BISHOP);
  pos.placePiece(F1, BLACK_BISHOP);
  moves = generator.generateLegalMoves(pos);
  BOOST_TEST(!containsMove(moves, getMove(moves, E1, G1)));
  BOOST_TEST(!containsMove(moves, getMove(moves, E1, C1)));

  pos.removePiece(C1);
  pos.removePiece(F1);
  pos.placePiece(F8, BLACK_ROOK);
  moves = generator.generateLegalMoves(pos);
  BOOST_TEST(!containsMove(moves, getMove(moves, E1, G1)));
  BOOST_TEST(containsMove(moves, getMove(moves, E1, C1)));

  pos.placePiece(D8, BLACK_ROOK);
  moves = generator.generateLegalMoves(pos);
  BOOST_TEST(!containsMove(moves, getMove(moves, E1, G1)));
  BOOST_TEST(!containsMove(moves, getMove(moves, E1, C1)));

  pos.removePiece(F8);
  pos.removePiece(D8);
  pos.placePiece(E8, BLACK_ROOK);
  moves = generator.generateLegalMoves(pos);
  BOOST_TEST(!containsMove(moves, getMove(moves, E1, G1)));
  BOOST_TEST(!containsMove(moves, getMove(moves, E1, C1)));

  Position pos2({
    {E8, BLACK_KING}, {A8, BLACK_ROOK}, {H8, BLACK_ROOK}
  }, 0, 0, 0, 1, 1);
  pos2.placePiece(G1, WHITE_ROOK);
  moves = generator.generateLegalMoves(pos2);
  BOOST_TEST(!containsMove(moves, getMove(moves, E8, G8)));
  BOOST_TEST(containsMove(moves, getMove(moves, E8, C8)));
}

BOOST_AUTO_TEST_CASE(undo_castling)
{
  Position pos({
    {E1, WHITE_KING}, {A1, WHITE_ROOK}, {H1, WHITE_ROOK}
  }, 1, 1, 1);
  MoveGenerator generator;
  MoveArray moves = generator.generateLegalMoves(pos);
  UndoInfo undo;
  pos.makeMove(getMove(moves, E1, G1), undo);
  BOOST_TEST(pos.getPiece(H1) == EMPTY);
  BOOST_TEST(pos.getPiece(G1) == WHITE_KING);
  BOOST_TEST(pos.getPiece(F1) == WHITE_ROOK);
  pos.undoMove(getMove(moves, E1, G1), undo);
  BOOST_TEST(isEqualArraysUnordered(moves, generator.generateLegalMoves(pos)));
}

BOOST_AUTO_TEST_CASE(moves_if_front_of_pawn)
{
  Position pos({
    {F5, WHITE_KING}, {E7, BLACK_PAWN}
  });
  MoveGenerator generator;
  MoveArray moves = generator.generateLegalMoves(pos);
  BOOST_TEST(containsMove(moves, getMove(moves, F5, E5)));
  BOOST_TEST(containsMove(moves, getMove(moves, F5, E6)));
  BOOST_TEST(!containsMove(moves, getMove(moves, F5, F6)));
}

BOOST_AUTO_TEST_CASE(castling_through_pawn_attacked_square)
{
  Position pos({
    {E1, WHITE_KING}, {E2, BLACK_PAWN}, {A1, WHITE_ROOK}, {H1, WHITE_ROOK}
  }, 1, 1, 1);
  MoveGenerator generator;
  MoveArray moves = generator.generateLegalMoves(pos);
  BOOST_TEST(!containsMove(moves, getMove(moves, E1, G1)));
  BOOST_TEST(!containsMove(moves, getMove(moves, E1, C1)));
}

BOOST_AUTO_TEST_CASE(mate)
{
  Position pos({
    {H6, WHITE_KING}, {H8, BLACK_KING}, {F8, WHITE_ROOK}
  }, 0);
  MoveGenerator generator;
  BOOST_TEST(generator.isMate(pos));
  pos.removePiece(F8);
  BOOST_TEST(!generator.isMate(pos));
}

BOOST_AUTO_TEST_CASE(stale_mate)
{
  Position pos({
    {H6, WHITE_KING}, {H8, BLACK_KING}, {G6, WHITE_ROOK}
  }, 0);
  MoveGenerator generator;
  BOOST_TEST(generator.isStaleMate(pos));
  pos.removePiece(G6);
  BOOST_TEST(!generator.isStaleMate(pos));
}
