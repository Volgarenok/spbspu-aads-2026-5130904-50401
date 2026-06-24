#include "move_generator.hpp"

namespace chess
{
  void MoveGenerator::generateKingMoves(const Position& pos, Square square, MoveArray& moves)
  {
    constexpr int possible_moves = 8;
    const int is_white_piece = pos.getPiece(square) > 0 ? 1 : -1;
    const int row = square / 8;
    const int col = square % 8;

    // начиная с клетки сверху, по часовой
    int row_offset[possible_moves] = {1, 1, 0, -1, -1, -1, 0, 1};
    int col_offset[possible_moves] = {0, 1, 1, 1, 0, -1, -1, -1};

    for (int i = 0; i < possible_moves; ++i)
    {
      int new_row = row + row_offset[i];
      int new_col = col + col_offset[i];
      if (new_row >= 0 && new_row < 8 && new_col >= 0 && new_col < 8)
      {
        int dest_square = new_row * 8 + new_col;
        if (pos.getPiece(dest_square) * is_white_piece < 1)
        {
          moves.push({square, static_cast< Square >(dest_square)});
        }
      }
    }
  }

  void MoveGenerator::generateCastlingMoves(const Position& pos, Square square, MoveArray& moves)
  {
    Castling rights = pos.getCastling();
    UndoInfo undo;
    Position half_king_move_pos = pos;
    bool king_castle = false;
    if (rights.king_ && pos.getPiece(square + 1) == EMPTY && pos.getPiece(square + 2) == EMPTY)
    {
      half_king_move_pos.makeMove(Move{square, static_cast< Square >(square + 1)}, undo);
      king_castle = true;
      if (!isSquareAttackedQuick(pos, static_cast< Square >(square), !pos.isWhiteToMove())
      && !isSquareAttackedQuick(half_king_move_pos, static_cast< Square >(square + 1), !pos.isWhiteToMove()))
      {
        moves.push({square, static_cast< Square >(square + 2), EMPTY, 0, 1});
      }
    }
    if (rights.queen_ && pos.getPiece(square - 1) == EMPTY
    && pos.getPiece(square - 2) == EMPTY && pos.getPiece(square - 3) == EMPTY)
    {
      if (king_castle)
      {
        half_king_move_pos.undoMove(Move{square, static_cast< Square >(square + 1)}, undo);
      }
      half_king_move_pos.makeMove(Move{square, static_cast< Square >(square - 1)}, undo);
      if (!isSquareAttackedQuick(pos, static_cast< Square >(square), !pos.isWhiteToMove())
      && !isSquareAttackedQuick(half_king_move_pos, static_cast< Square >(square - 1), !pos.isWhiteToMove()))
      {
        moves.push({square, static_cast< Square >(square - 2), EMPTY, 0, 1});
      }
    }
  }


  void MoveGenerator::generateQueenMoves(const Position& pos, Square square, MoveArray& moves)
  {
    generateRookMoves(pos, square, moves);
    generateBishopMoves(pos, square, moves);
  }

  void MoveGenerator::generateKnightMoves(const Position& pos, Square square, MoveArray& moves)
  {
    constexpr int possible_moves = 8;
    const int is_white_piece = pos.getPiece(square) > 0 ? 1 : -1;
    const int row = square / 8;
    const int col = square % 8;

    // начиная с клетки сверху справа, по часовой
    int row_offset[possible_moves] = {2, 1, -1, -2, -2, -1, 1, 2};
    int col_offset[possible_moves] = {1, 2, 2, 1, -1, -2, -2, -1};

    for (int i = 0; i < possible_moves; ++i)
    {
      int new_row = row + row_offset[i];
      int new_col = col + col_offset[i];
      if (new_row >= 0 && new_row < 8 && new_col >= 0 && new_col < 8)
      {
        int dest_square = new_row * 8 + new_col;
        if (pos.getPiece(dest_square) * is_white_piece < 1)
        {
          moves.push({square, static_cast< Square >(dest_square)});
        }
      }
    }
  }

  void MoveGenerator::generateBishopMoves(const Position& pos, Square square, MoveArray& moves)
  {
    const int is_white_piece = pos.getPiece(square) > 0 ? 1 : -1;

    // вверх вправо
    int col = (square % 8) + 1;
    int dest_square = square + 9;
    while (8 - col > 0 && dest_square <= H8 && pos.getPiece(dest_square) * is_white_piece < 1)
    {
      moves.push({square, static_cast< Square >(dest_square)});
      if (pos.getPiece(dest_square) != EMPTY)
      {
        break;
      }
      dest_square += 9;
      ++col;
    }

    // вниз вправо
    col = (square % 8) + 1;
    dest_square = square - 7;
    while (8 - col > 0 && dest_square >= A1 && pos.getPiece(dest_square) * is_white_piece < 1)
    {
      moves.push({square, static_cast< Square >(dest_square)});
      if (pos.getPiece(dest_square) != EMPTY)
      {
        break;
      }
      dest_square -= 7;
      ++col;
    }

    // вниз влево
    col = (square % 8) - 1;
    dest_square = square - 9;
    while (col >= 0 && dest_square >= A1 && pos.getPiece(dest_square) * is_white_piece < 1)
    {
      moves.push({square, static_cast< Square >(dest_square)});
      if (pos.getPiece(dest_square) != EMPTY)
      {
        break;
      }
      dest_square -= 9;
      --col;
    }

    // вверх влево
    col = (square % 8) - 1;
    dest_square = square + 7;
    while (col >= 0 && dest_square <= H8 && pos.getPiece(dest_square) * is_white_piece < 1)
    {
      moves.push({square, static_cast< Square >(dest_square)});
      if (pos.getPiece(dest_square) != EMPTY)
      {
        break;
      }
      dest_square += 7;
      --col;
    }
  }

  void MoveGenerator::generateRookMoves(const Position& pos, Square square, MoveArray& moves)
  {
    const int is_white_piece = pos.getPiece(square) > 0 ? 1 : -1;

    // вверх
    int dest_square = square + 8;
    while (dest_square <= H8 && pos.getPiece(dest_square) * is_white_piece < 1)
    {
      moves.push({square, static_cast< Square >(dest_square)});
      if (pos.getPiece(dest_square) != EMPTY)
      {
        break;
      }
      dest_square += 8;
    }

    // вниз
    dest_square = square - 8;
    while (dest_square >= A1 && pos.getPiece(dest_square) * is_white_piece < 1)
    {
      moves.push({square, static_cast< Square >(dest_square)});
      if (pos.getPiece(dest_square) != EMPTY)
      {
        break;
      }
      dest_square -= 8;
    }

    // вправо
    int col = (square % 8) + 1;
    dest_square = square + 1;
    while (8 - col > 0 && pos.getPiece(dest_square) * is_white_piece < 1)
    {
      moves.push({square, static_cast< Square >(dest_square)});
      if (pos.getPiece(dest_square) != EMPTY)
      {
        break;
      }
      ++dest_square;
      ++col;
    }

    // влево
    col = (square % 8) - 1;
    dest_square = square - 1;
    while (col >= 0 && pos.getPiece(dest_square) * is_white_piece < 1)
    {
      moves.push({square, static_cast< Square >(dest_square)});
      if (pos.getPiece(dest_square) != EMPTY)
      {
        break;
      }
      --dest_square;
      --col;
    }
  }

  void MoveGenerator::generatePawnMoves(const Position& pos, Square square, MoveArray& moves)
  {
    const int is_white_piece = pos.getPiece(square) > 0 ? 1 : -1;
    const int displacement = is_white_piece == 1 ? 8 : -8;
    const int start_row = is_white_piece == 1 ? 1 : 6;
    const int promotion_row = is_white_piece == 1 ? 6 : 1;
    const int enPassant_row = is_white_piece == 1 ? 4 : 3;

    const int row = square / 8;
    const int col = square % 8;

    auto promote = [&moves, square, is_white_piece](int displacement){
        moves.push({square, static_cast< Square >(square + displacement), static_cast< Piece >(WHITE_QUEEN * is_white_piece)});
        moves.push({square, static_cast< Square >(square + displacement), static_cast< Piece >(WHITE_KNIGHT * is_white_piece)});
        moves.push({square, static_cast< Square >(square + displacement), static_cast< Piece >(WHITE_ROOK * is_white_piece)});
        moves.push({square, static_cast< Square >(square + displacement), static_cast< Piece >(WHITE_BISHOP * is_white_piece)});
    };

    // ходы вперед
    if (pos.getPiece(square + displacement) == EMPTY)
    {
      if (row == promotion_row)
      {
        promote(displacement);
      }
      else
      {
        moves.push({square, static_cast< Square >(square + displacement)});
        if (row == start_row && pos.getPiece(square + displacement * 2) == EMPTY)
        {
          moves.push({square, static_cast< Square >(square + displacement * 2)});
        }
      }
    }

    // взятия
    const int take_displacements[2] = {9 * is_white_piece, 7 * is_white_piece};
    const int corner_col_for_take[2] = {is_white_piece == 1 ? 7 : 0, is_white_piece == 1 ? 0 : 7};

    for (size_t i = 0; i < 2; ++i)
    {
      if (col != corner_col_for_take[i])
      {
        const int take_piece = pos.getPiece(square + take_displacements[i]);
        if (take_piece * is_white_piece < 0)
        {
          if (row == promotion_row)
          {
            promote(take_displacements[i]);
          }
          else
          {
            moves.push({square, static_cast< Square >(square + take_displacements[i])});
          }
        }
        // взятие на проходе
        else if (row == enPassant_row && pos.getEnPassantSquare() == square + take_displacements[i])
        {
          moves.push({square, static_cast< Square >(square + take_displacements[i]), EMPTY, true});
        }
      }
    }
  }

  bool MoveGenerator::isSquareAttacked(const Position& pos, Square square)
  {
    const int row = square / 8;
    const int col = square % 8;
    const int is_white_move = pos.isWhiteToMove() ? 1 : -1;
    MoveArray moves;
    int i = 0;

    generateRookMoves(pos, square, moves);
    for (; i < moves.size(); ++i)
    {
      if (pos.getPiece(moves.get(i).to_) == WHITE_ROOK * is_white_move) return true;
      if (pos.getPiece(moves.get(i).to_) == WHITE_QUEEN * is_white_move) return true;
    }

    generateBishopMoves(pos, square, moves);
    for (; i < moves.size(); ++i)
    {
      if (pos.getPiece(moves.get(i).to_) == WHITE_BISHOP * is_white_move) return true;
      if (pos.getPiece(moves.get(i).to_) == WHITE_QUEEN * is_white_move) return true;
    }

    generateKnightMoves(pos, square, moves);
    for (; i < moves.size(); ++i)
    {
      if (pos.getPiece(moves.get(i).to_) == WHITE_KNIGHT * is_white_move) return true;
    }

    generateKingMoves(pos, square, moves);
    for (; i < moves.size(); ++i)
    {
      if (pos.getPiece(moves.get(i).to_) == WHITE_KING * is_white_move) return true;
    }

    if (is_white_move == 1)
    {
      if (row > 0 && col > 0 && pos.getPiece(square - 9) == WHITE_PAWN) return true;
      if (row > 0 && col < 7 && pos.getPiece(square - 7) == WHITE_PAWN) return true;
    }
    else
    {
      if (row < 7 && col > 0 && pos.getPiece(square + 7) == BLACK_PAWN) return true;
      if (row < 7 && col < 7 && pos.getPiece(square + 9) == BLACK_PAWN) return true;
    }

    return false;
  }

  bool MoveGenerator::isSquareAttackedQuick(const Position& pos, Square square, bool byWhite)
  {
    const int row = square / 8;
    const int col = square % 8;
    const int side = byWhite ? 1 : -1;

    if (byWhite)
    {
      if (row > 0 && col > 0 && pos.getPiece(square - 9) == WHITE_PAWN) return true;
      if (row > 0 && col < 7 && pos.getPiece(square - 7) == WHITE_PAWN) return true;
    }
    else
    {
      if (row < 7 && col > 0 && pos.getPiece(square + 7) == BLACK_PAWN) return true;
      if (row < 7 && col < 7 && pos.getPiece(square + 9) == BLACK_PAWN) return true;
    }

    int row_offset[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    int col_offset[8] = {0, 1, 1, 1, 0, -1, -1, -1};

    for (int i = 0; i < 8; ++i)
    {
      int new_row = row + row_offset[i];
      int new_col = col + col_offset[i];
      if (new_row >= 0 && new_row < 8 && new_col >= 0 && new_col < 8)
      {
        int dest_square = new_row * 8 + new_col;
        if (pos.getPiece(dest_square) == WHITE_KING * side) return true;
      }
    }

    int row_offset2[8] = {2, 1, -1, -2, -2, -1, 1, 2};
    int col_offset2[8] = {1, 2, 2, 1, -1, -2, -2, -1};

    for (int i = 0; i < 8; ++i)
    {
      int new_row2 = row + row_offset2[i];
      int new_col2 = col + col_offset2[i];
      if (new_row2 >= 0 && new_row2 < 8 && new_col2 >= 0 && new_col2 < 8)
      {
        int dest_square = new_row2 * 8 + new_col2;
        if (pos.getPiece(dest_square) == WHITE_KNIGHT * side) return true;
      }
    }

    // вверх
    int dest_square = square + 8;
    while (dest_square <= H8 && pos.getPiece(dest_square) == EMPTY)
    {
      dest_square += 8;
    }
    if (dest_square <= H8)
    {
      if (pos.getPiece(dest_square) == WHITE_ROOK * side || pos.getPiece(dest_square) == WHITE_QUEEN * side)
      {
        return true;
      }
    }

    // вниз
    dest_square = square - 8;
    while (dest_square >= A1 && pos.getPiece(dest_square) == EMPTY)
    {
      dest_square -= 8;
    }
    if (dest_square >= A1)
    {
      if (pos.getPiece(dest_square) == WHITE_ROOK * side || pos.getPiece(dest_square) == WHITE_QUEEN * side)
      {
        return true;
      }
    }


    // вправо
    int col_temp = (square % 8) + 1;
    dest_square = square + 1;
    while (8 - col_temp > 0 && pos.getPiece(dest_square) == EMPTY)
    {
      ++dest_square;
      ++col_temp;
    }
    if (8 - col_temp > 0)
    {
      if (pos.getPiece(dest_square) == WHITE_ROOK * side || pos.getPiece(dest_square) == WHITE_QUEEN * side)
      {
        return true;
      }
    }

    // влево
    col_temp = (square % 8) - 1;
    dest_square = square - 1;
    while (col_temp >= 0 && pos.getPiece(dest_square) == EMPTY)
    {
      --dest_square;
      --col_temp;
    }
    if (col_temp >= 0)
    {
      if (pos.getPiece(dest_square) == WHITE_ROOK * side || pos.getPiece(dest_square) == WHITE_QUEEN * side)
      {
        return true;
      }
    }

    // вверх вправо
    col_temp = (square % 8) + 1;
    dest_square = square + 9;
    while (8 - col_temp > 0 && dest_square <= H8 && pos.getPiece(dest_square) == EMPTY)
    {
      dest_square += 9;
      ++col_temp;
    }
    if (8 - col_temp > 0 && dest_square <= H8)
    {
      if (pos.getPiece(dest_square) == WHITE_BISHOP * side || pos.getPiece(dest_square) == WHITE_QUEEN * side)
      {
        return true;
      }
    }

    // вниз вправо
    col_temp = (square % 8) + 1;
    dest_square = square - 7;
    while (8 - col_temp > 0 && dest_square >= A1 && pos.getPiece(dest_square) == EMPTY)
    {
      dest_square -= 7;
      ++col_temp;
    }
    if (8 - col_temp > 0 && dest_square >= A1)
    {
      if (pos.getPiece(dest_square) == WHITE_BISHOP * side || pos.getPiece(dest_square) == WHITE_QUEEN * side)
      {
        return true;
      }
    }

    // вниз влево
    col_temp = (square % 8) - 1;
    dest_square = square - 9;
    while (col_temp >= 0 && dest_square >= A1 && pos.getPiece(dest_square) == EMPTY)
    {
      dest_square -= 9;
      --col_temp;
    }
    if (col_temp >= 0 && dest_square >= A1)
    {
      if (pos.getPiece(dest_square) == WHITE_BISHOP * side || pos.getPiece(dest_square) == WHITE_QUEEN * side)
      {
        return true;
      }
    }

    // вверх влево
    col_temp = (square % 8) - 1;
    dest_square = square + 7;
    while (col_temp >= 0 && dest_square <= H8 && pos.getPiece(dest_square) == EMPTY)
    {
      dest_square += 7;
      --col_temp;
    }
    if (col_temp >= 0 && dest_square <= H8)
    {
      if (pos.getPiece(dest_square) == WHITE_BISHOP * side || pos.getPiece(dest_square) == WHITE_QUEEN * side)
      {
        return true;
      }
    }

    return false;
  }

  MoveArray MoveGenerator::generatePseudoLegalMoves(const Position& pos, bool castling)
  {
    MoveArray moves;
    const int side_to_move = pos.isWhiteToMove() ? 1 : -1;
    for (int i = A1; i <= H8; ++i)
    {
      const int piece = pos.getPiece(i);
      if (piece * side_to_move <= 0)
      {
        continue;
      }
      const int is_white_piece = piece > 0 ? 1 : -1;
      const int abs_piece = piece * is_white_piece;
      switch (abs_piece)
      {
        case EMPTY:
          break;
        case WHITE_KNIGHT:
          generateKnightMoves(pos, static_cast< Square >(i), moves);
          break;
        case WHITE_BISHOP:
          generateBishopMoves(pos, static_cast< Square >(i), moves);
          break;
        case WHITE_QUEEN:
          generateQueenMoves(pos, static_cast< Square >(i), moves);
          break;
        case WHITE_PAWN:
          generatePawnMoves(pos, static_cast< Square >(i), moves);
          break;
        case WHITE_ROOK:
          generateRookMoves(pos, static_cast< Square >(i), moves);
          break;
        case WHITE_KING:
          generateKingMoves(pos, static_cast< Square >(i), moves);
          if (castling)
          {
            generateCastlingMoves(pos, static_cast< Square >(i), moves);
          }
          break;
      }
    }
    return moves;
  }

  MoveArray MoveGenerator::generateLegalMoves(const Position& pos)
  {
    MoveArray moves = generatePseudoLegalMoves(pos);
    MoveArray legal_moves;
    Position new_pos = pos;
    UndoInfo undo;
    for (int i = 0; i < moves.size(); ++i)
    {
      Move move = moves.get(i);
      new_pos.makeMove(move, undo);
      if (!isSquareAttackedQuick(new_pos, static_cast< Square >(new_pos.getOppositeColourKingSquare()), new_pos.isWhiteToMove()))
      {
        legal_moves.push(move);
      }
      new_pos.undoMove(move, undo);
    }
    return legal_moves;
  }

  bool MoveGenerator::isMate(const Position& pos)
  {
    if (generateLegalMoves(pos).empty()
      && isCheck(pos))
    {
      return true;
    }
    return false;
  }

  bool MoveGenerator::isStaleMate(const Position& pos)
  {
    if (generateLegalMoves(pos).empty()
      && !isSquareAttackedQuick(pos, static_cast< Square >(pos.getCurentColourKingSquare()), !pos.isWhiteToMove()))
    {
      return true;
    }
    return false;
  }

  bool MoveGenerator::isCheck(const Position& pos)
  {
    if (isSquareAttackedQuick(pos, static_cast< Square >(pos.getCurentColourKingSquare()), !pos.isWhiteToMove()))
    {
      return true;
    }
    return false;
  }

  Move MoveGenerator::findPawnAttacker(const Position& pos, int square, int side)
  {
    const int row = square / 8;
    const int col = square % 8;
    const int pawn = WHITE_PAWN * (side > 0 ? 1 : -1);

    if (side == 1)
    {
      if (row > 0 && col > 0 && pos.getPiece(square - 9) == pawn)
      {
        return Move{static_cast< Square >(square - 9), static_cast< Square >(square)};
      }
      if (row > 0 && col < 7 && pos.getPiece(square - 7) == pawn)
      {
        return Move{static_cast< Square >(square - 7), static_cast< Square >(square)};
      }
    }
    else
    {
      if (row < 7 && col > 0 && pos.getPiece(square + 7) == pawn)
      {
        return Move{static_cast< Square >(square + 7), static_cast< Square >(square)};
      }
      if (row < 7 && col < 7 && pos.getPiece(square + 9) == pawn)
      {
        return Move{static_cast< Square >(square + 9), static_cast< Square >(square)};
      }
    }

    return null_move;
  }

  Move MoveGenerator::findKnightAttacker(const Position& pos, int square, int side)
  {
    const int row = square / 8;
    const int col = square % 8;
    const int knight = WHITE_KNIGHT * (side > 0 ? 1 : -1);
    const int row_offset[8] = {2, 1, -1, -2, -2, -1, 1, 2};
    const int col_offset[8] = {1, 2, 2, 1, -1, -2, -2, -1};

    for (int i = 0; i < 8; ++i)
    {
      int cur_row = row + row_offset[i];
      int cur_col = col + col_offset[i];
      if (cur_row >= 0 && cur_row < 8 && cur_col >= 0 && cur_col < 8)
      {
        int from = cur_row * 8 + cur_col;
        if (pos.getPiece(from) == knight)
        {
          return Move{static_cast< Square >(from), static_cast< Square >(square)};
        }
      }
    }

    return null_move;
  }

  Move MoveGenerator::findBishopAttacker(const Position& pos, int square, int side)
  {
    const int bishop = WHITE_BISHOP * (side > 0 ? 1 : -1);

    // вверх вправо
    int col = (square % 8) + 1;
    int from = square + 9;
    while (8 - col > 0 && from <= H8 && pos.getPiece(from) == EMPTY)
    {
      from += 9;
      ++col;
    }
    if (8 - col > 0 && from <= H8 && pos.getPiece(from) == bishop)
    {
      return Move{static_cast< Square >(from), static_cast< Square >(square)};
    }

    // вниз вправо
    col = (square % 8) + 1;
    from = square - 7;
    while (8 - col > 0 && from >= A1 && pos.getPiece(from) == EMPTY)
    {
      from -= 7;
      ++col;
    }
    if (8 - col > 0 && from >= A1 && pos.getPiece(from) == bishop)
    {
      return Move{static_cast< Square >(from), static_cast< Square >(square)};
    }

    // вниз влево
    col = (square % 8) - 1;
    from = square - 9;
    while (col >= 0 && from >= A1 && pos.getPiece(from) == EMPTY)
    {
      from -= 9;
      --col;
    }
    if (col >= 0 && from >= A1 && pos.getPiece(from) == bishop)
    {
      return Move{static_cast< Square >(from), static_cast< Square >(square)};
    }

    // вверх влево
    col = (square % 8) - 1;
    from = square + 7;
    while (col >= 0 && from <= H8 && pos.getPiece(from) == EMPTY)
    {
      from += 7;
      --col;
    }
    if (col >= 0 && from <= H8 && pos.getPiece(from) == bishop)
    {
      return Move{static_cast< Square >(from), static_cast< Square >(square)};
    }

    return null_move;
  }

  Move MoveGenerator::findRookAttacker(const Position& pos, int square, int side)
  {
    const int rook = WHITE_ROOK * (side > 0 ? 1 : -1);

    // вверх
    int from = square + 8;
    while (from <= H8 && pos.getPiece(from) == EMPTY)
    {
      from += 8;
    }
    if (from <= H8 && pos.getPiece(from) == rook)
    {
      return Move{static_cast< Square >(from), static_cast< Square >(square)};
    }

    // вниз
    from = square - 8;
    while (from >= A1 && pos.getPiece(from) == EMPTY)
    {
      from -= 8;
    }
    if (from >= A1 && pos.getPiece(from) == rook)
    {
      return Move{static_cast< Square >(from), static_cast< Square >(square)};
    }

    // вправо
    int col = (square % 8) + 1;
    from = square + 1;
    while (8 - col > 0 && pos.getPiece(from) == EMPTY)
    {
      ++from;
      ++col;
    }
    if (8 - col > 0 && pos.getPiece(from) == rook)
    {
      return Move{static_cast< Square >(from), static_cast< Square >(square)};
    }

    // влево
    col = (square % 8) - 1;
    from = square - 1;
    while (col >= 0 && pos.getPiece(from) == EMPTY)
    {
      --from;
      --col;
    }
    if (col >= 0 && pos.getPiece(from) == rook)
    {
      return Move{static_cast< Square >(from), static_cast< Square >(square)};
    }

    return null_move;
  }

  Move MoveGenerator::findQueenAttacker(const Position& pos, int square, int side)
  {
    const int queen = WHITE_QUEEN * (side > 0 ? 1 : -1);

    // вверх
    int from = square + 8;
    while (from <= H8 && pos.getPiece(from) == EMPTY)
    {
      from += 8;
    }
    if (from <= H8 && pos.getPiece(from) == queen)
    {
      return Move{static_cast< Square >(from), static_cast< Square >(square)};
    }

    // вниз
    from = square - 8;
    while (from >= A1 && pos.getPiece(from) == EMPTY)
    {
      from -= 8;
    }
    if (from >= A1 && pos.getPiece(from) == queen)
    {
      return Move{static_cast< Square >(from), static_cast< Square >(square)};
    }

    // вправо
    int col = (square % 8) + 1;
    from = square + 1;
    while (8 - col > 0 && pos.getPiece(from) == EMPTY)
    {
      ++from;
      ++col;
    }
    if (8 - col > 0 && pos.getPiece(from) == queen)
    {
      return Move{static_cast< Square >(from), static_cast< Square >(square)};
    }

    // влево
    col = (square % 8) - 1;
    from = square - 1;
    while (col >= 0 && pos.getPiece(from) == EMPTY)
    {
      --from;
      --col;
    }
    if (col >= 0 && pos.getPiece(from) == queen)
    {
      return Move{static_cast< Square >(from), static_cast< Square >(square)};
    }

    // вверх вправо
    col = (square % 8) + 1;
    from = square + 9;
    while (8 - col > 0 && from <= H8 && pos.getPiece(from) == EMPTY)
    {
      from += 9;
      ++col;
    }
    if (8 - col > 0 && from <= H8 && pos.getPiece(from) == queen)
    {
      return Move{static_cast< Square >(from), static_cast< Square >(square)};
    }

    // вниз вправо
    col = (square % 8) + 1;
    from = square - 7;
    while (8 - col > 0 && from >= A1 && pos.getPiece(from) == EMPTY)
    {
      from -= 7;
      ++col;
    }
    if (8 - col > 0 && from >= A1 && pos.getPiece(from) == queen)
    {
      return Move{static_cast< Square >(from), static_cast< Square >(square)};
    }

    // вниз влево
    col = (square % 8) - 1;
    from = square - 9;
    while (col >= 0 && from >= A1 && pos.getPiece(from) == EMPTY)
    {
      from -= 9;
      --col;
    }
    if (col >= 0 && from >= A1 && pos.getPiece(from) == queen)
    {
      return Move{static_cast< Square >(from), static_cast< Square >(square)};
    }

    // вверх влево
    col = (square % 8) - 1;
    from = square + 7;
    while (col >= 0 && from <= H8 && pos.getPiece(from) == EMPTY)
    {
      from += 7;
      --col;
    }
    if (col >= 0 && from <= H8 && pos.getPiece(from) == queen)
    {
      return Move{static_cast< Square >(from), static_cast< Square >(square)};
    }

    return null_move;
  }

  Move MoveGenerator::findKingAttacker(const Position& pos, int square, int side)
  {
    const int row = square / 8;
    const int col = square % 8;
    const int king = WHITE_KING * (side > 0 ? 1 : -1);
    const int row_offset[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    const int col_offset[8] = {0, 1, 1, 1, 0, -1, -1, -1};

    for (int i = 0; i < 8; ++i)
    {
      int cur_row = row + row_offset[i];
      int cur_col = col + col_offset[i];
      if (cur_row >= 0 && cur_row < 8 && cur_col >= 0 && cur_col < 8)
      {
        int from = cur_row * 8 + cur_col;
        if (pos.getPiece(from) == king)
        {
          return Move{static_cast< Square >(from), static_cast< Square >(square)};
        }
      }
    }

    return null_move;
  }

  void MoveGenerator::generateRookCaptures(const Position& pos, Square square, MoveArray& moves)
  {
    const int is_white_piece = pos.getPiece(square) > 0 ? 1 : -1;

    // вверх
    int dest_square = square + 8;
    while (dest_square <= H8 && pos.getPiece(dest_square) * is_white_piece < 1)
    {
      if (pos.getPiece(dest_square) != EMPTY)
      {
        moves.push({square, static_cast< Square >(dest_square)});
        break;
      }
      dest_square += 8;
    }

    // вниз
    dest_square = square - 8;
    while (dest_square >= A1 && pos.getPiece(dest_square) * is_white_piece < 1)
    {
      if (pos.getPiece(dest_square) != EMPTY)
      {
        moves.push({square, static_cast< Square >(dest_square)});
        break;
      }
      dest_square -= 8;
    }

    // вправо
    int col = (square % 8) + 1;
    dest_square = square + 1;
    while (8 - col > 0 && pos.getPiece(dest_square) * is_white_piece < 1)
    {
      if (pos.getPiece(dest_square) != EMPTY)
      {
        moves.push({square, static_cast< Square >(dest_square)});
        break;
      }
      ++dest_square;
      ++col;
    }

    // влево
    col = (square % 8) - 1;
    dest_square = square - 1;
    while (col >= 0 && pos.getPiece(dest_square) * is_white_piece < 1)
    {
      if (pos.getPiece(dest_square) != EMPTY)
      {
        moves.push({square, static_cast< Square >(dest_square)});
        break;
      }
      --dest_square;
      --col;
    }
  }

  void MoveGenerator::generateBishopCaptures(const Position& pos, Square square, MoveArray& moves)
  {
    const int is_white_piece = pos.getPiece(square) > 0 ? 1 : -1;

    // вверх вправо
    int col = (square % 8) + 1;
    int dest_square = square + 9;
    while (8 - col > 0 && dest_square <= H8 && pos.getPiece(dest_square) * is_white_piece < 1)
    {
      if (pos.getPiece(dest_square) != EMPTY)
      {
        moves.push({square, static_cast< Square >(dest_square)});
        break;
      }
      dest_square += 9;
      ++col;
    }

    // вниз вправо
    col = (square % 8) + 1;
    dest_square = square - 7;
    while (8 - col > 0 && dest_square >= A1 && pos.getPiece(dest_square) * is_white_piece < 1)
    {
      if (pos.getPiece(dest_square) != EMPTY)
      {
        moves.push({square, static_cast< Square >(dest_square)});
        break;
      }
      dest_square -= 7;
      ++col;
    }

    // вниз влево
    col = (square % 8) - 1;
    dest_square = square - 9;
    while (col >= 0 && dest_square >= A1 && pos.getPiece(dest_square) * is_white_piece < 1)
    {
      if (pos.getPiece(dest_square) != EMPTY)
      {
        moves.push({square, static_cast< Square >(dest_square)});
        break;
      }
      dest_square -= 9;
      --col;
    }

    // вверх влево
    col = (square % 8) - 1;
    dest_square = square + 7;
    while (col >= 0 && dest_square <= H8 && pos.getPiece(dest_square) * is_white_piece < 1)
    {
      if (pos.getPiece(dest_square) != EMPTY)
      {
        moves.push({square, static_cast< Square >(dest_square)});
        break;
      }
      dest_square += 7;
      --col;
    }
  }

  void MoveGenerator::generateKnightCaptures(const Position& pos, Square square, MoveArray& moves)
  {
    constexpr int possible_moves = 8;
    const int is_white_piece = pos.getPiece(square) > 0 ? 1 : -1;
    const int row = square / 8;
    const int col = square % 8;

    // начиная с клетки сверху справа, по часовой
    int row_offset[possible_moves] = {2, 1, -1, -2, -2, -1, 1, 2};
    int col_offset[possible_moves] = {1, 2, 2, 1, -1, -2, -2, -1};

    for (int i = 0; i < possible_moves; ++i)
    {
      int new_row = row + row_offset[i];
      int new_col = col + col_offset[i];
      if (new_row >= 0 && new_row < 8 && new_col >= 0 && new_col < 8)
      {
        int dest_square = new_row * 8 + new_col;
        if (pos.getPiece(dest_square) * is_white_piece < 0)
        {
          moves.push({square, static_cast< Square >(dest_square)});
        }
      }
    }
  }

  void MoveGenerator::generateKingCaptures(const Position& pos, Square square, MoveArray& moves)
  {
    constexpr int possible_moves = 8;
    const int is_white_piece = pos.getPiece(square) > 0 ? 1 : -1;
    const int row = square / 8;
    const int col = square % 8;

    // начиная с клетки сверху, по часовой
    int row_offset[possible_moves] = {1, 1, 0, -1, -1, -1, 0, 1};
    int col_offset[possible_moves] = {0, 1, 1, 1, 0, -1, -1, -1};

    for (int i = 0; i < possible_moves; ++i)
    {
      int new_row = row + row_offset[i];
      int new_col = col + col_offset[i];
      if (new_row >= 0 && new_row < 8 && new_col >= 0 && new_col < 8)
      {
        int dest_square = new_row * 8 + new_col;
        if (pos.getPiece(dest_square) * is_white_piece < 0)
        {
          moves.push({square, static_cast< Square >(dest_square)});
        }
      }
    }
  }

  void MoveGenerator::generatePawnCapturesAndPromotions(const Position& pos, Square square, MoveArray& moves)
  {
    const int is_white_piece = pos.getPiece(square) > 0 ? 1 : -1;
    const int displacement = is_white_piece == 1 ? 8 : -8;

    const int promotion_row = is_white_piece == 1 ? 6 : 1;
    const int enPassant_row = is_white_piece == 1 ? 4 : 3;

    const int row = square / 8;
    const int col = square % 8;

    auto promote = [&moves, square, is_white_piece](int displacement){
        moves.push({square, static_cast< Square >(square + displacement), static_cast< Piece >(WHITE_QUEEN * is_white_piece)});
        moves.push({square, static_cast< Square >(square + displacement), static_cast< Piece >(WHITE_KNIGHT * is_white_piece)});
        moves.push({square, static_cast< Square >(square + displacement), static_cast< Piece >(WHITE_ROOK * is_white_piece)});
        moves.push({square, static_cast< Square >(square + displacement), static_cast< Piece >(WHITE_BISHOP * is_white_piece)});
    };

    if (pos.getPiece(square + displacement) == EMPTY && row == promotion_row)
    {
      promote(displacement);
    }

    // взятия
    const int take_displacements[2] = {9 * is_white_piece, 7 * is_white_piece};
    const int corner_col_for_take[2] = {is_white_piece == 1 ? 7 : 0, is_white_piece == 1 ? 0 : 7};

    for (size_t i = 0; i < 2; ++i)
    {
      if (col != corner_col_for_take[i])
      {
        const int take_piece = pos.getPiece(square + take_displacements[i]);
        if (take_piece * is_white_piece < 0)
        {
          if (row == promotion_row)
          {
            promote(take_displacements[i]);
          }
          else
          {
            moves.push({square, static_cast< Square >(square + take_displacements[i])});
          }
        }
        // взятие на проходе
        else if (row == enPassant_row && pos.getEnPassantSquare() == square + take_displacements[i])
        {
          moves.push({square, static_cast< Square >(square + take_displacements[i]), EMPTY, true});
        }
      }
    }
  }

  void MoveGenerator::generateQueenCaptures(const Position& pos, Square square, MoveArray& moves)
  {
    generateRookCaptures(pos, square, moves);
    generateBishopCaptures(pos, square, moves);
  }

  void MoveGenerator::generatePseudoLegalActiveMoves(const Position& pos, MoveArray& moves)
  {
    const int side_to_move = pos.isWhiteToMove() ? 1 : -1;
    for (int i = A1; i <= H8; ++i)
    {
      const int piece = pos.getPiece(i);
      if (piece * side_to_move <= 0)
      {
        continue;
      }
      const int is_white_piece = piece > 0 ? 1 : -1;
      const int abs_piece = piece * is_white_piece;
      switch (abs_piece)
      {
        case EMPTY:
          break;
        case WHITE_KNIGHT:
          generateKnightCaptures(pos, static_cast< Square >(i), moves);
          break;
        case WHITE_BISHOP:
          generateBishopCaptures(pos, static_cast< Square >(i), moves);
          break;
        case WHITE_QUEEN:
          generateQueenCaptures(pos, static_cast< Square >(i), moves);
          break;
        case WHITE_PAWN:
          generatePawnCapturesAndPromotions(pos, static_cast< Square >(i), moves);
          break;
        case WHITE_ROOK:
          generateRookCaptures(pos, static_cast< Square >(i), moves);
          break;
        case WHITE_KING:
          generateKingCaptures(pos, static_cast< Square >(i), moves);
          break;
      }
    }
  }

  MoveArray MoveGenerator::generateActiveMoves(const Position& pos)
  {
    MoveArray active_moves;
    MoveArray legal_moves;
    Position new_pos = pos;
    UndoInfo undo;
    generatePseudoLegalActiveMoves(pos, active_moves);

    for (int i = 0; i < active_moves.size(); ++i)
    {
      Move curr = active_moves.get(i);
      new_pos.makeMove(curr, undo);
      if (!isSquareAttackedQuick(new_pos, static_cast< Square >(new_pos.getOppositeColourKingSquare()), new_pos.isWhiteToMove()))
      {
        legal_moves.push(curr);
      }
      new_pos.undoMove(curr, undo);
    }
    return legal_moves;
  }

  int MoveGenerator::countPseudoLegalRookMoves(const Position &pos, Square square)
  {
    int count = 0;
    const int is_white_piece = pos.getPiece(square) > 0 ? 1 : -1;

    // вверх
    int dest_square = square + 8;
    while (dest_square <= H8 && pos.getPiece(dest_square) * is_white_piece < 1)
    {
      ++count;
      if (pos.getPiece(dest_square) != EMPTY)
      {
        break;
      }
      dest_square += 8;
    }

    // вниз
    dest_square = square - 8;
    while (dest_square >= A1 && pos.getPiece(dest_square) * is_white_piece < 1)
    {
      ++count;
      if (pos.getPiece(dest_square) != EMPTY)
      {
        break;
      }
      dest_square -= 8;
    }

    // вправо
    int col = (square % 8) + 1;
    dest_square = square + 1;
    while (8 - col > 0 && pos.getPiece(dest_square) * is_white_piece < 1)
    {
      ++count;
      if (pos.getPiece(dest_square) != EMPTY)
      {
        break;
      }
      ++dest_square;
      ++col;
    }

    // влево
    col = (square % 8) - 1;
    dest_square = square - 1;
    while (col >= 0 && pos.getPiece(dest_square) * is_white_piece < 1)
    {
      ++count;
      if (pos.getPiece(dest_square) != EMPTY)
      {
        break;
      }
      --dest_square;
      --col;
    }

    return count;
  }

  int MoveGenerator::countPseudoLegalBishopMoves(const Position &pos, Square square)
  {
    int count = 0;
    const int is_white_piece = pos.getPiece(square) > 0 ? 1 : -1;

    // вверх вправо
    int col = (square % 8) + 1;
    int dest_square = square + 9;
    while (8 - col > 0 && dest_square <= H8 && pos.getPiece(dest_square) * is_white_piece < 1)
    {
      ++count;
      if (pos.getPiece(dest_square) != EMPTY)
      {
        break;
      }
      dest_square += 9;
      ++col;
    }

    // вниз вправо
    col = (square % 8) + 1;
    dest_square = square - 7;
    while (8 - col > 0 && dest_square >= A1 && pos.getPiece(dest_square) * is_white_piece < 1)
    {
      ++count;
      if (pos.getPiece(dest_square) != EMPTY)
      {
        break;
      }
      dest_square -= 7;
      ++col;
    }

    // вниз влево
    col = (square % 8) - 1;
    dest_square = square - 9;
    while (col >= 0 && dest_square >= A1 && pos.getPiece(dest_square) * is_white_piece < 1)
    {
      ++count;
      if (pos.getPiece(dest_square) != EMPTY)
      {
        break;
      }
      dest_square -= 9;
      --col;
    }

    // вверх влево
    col = (square % 8) - 1;
    dest_square = square + 7;
    while (col >= 0 && dest_square <= H8 && pos.getPiece(dest_square) * is_white_piece < 1)
    {
      ++count;
      if (pos.getPiece(dest_square) != EMPTY)
      {
        break;
      }
      dest_square += 7;
      --col;
    }

    return count;
  }

  int MoveGenerator::countPseudoLegalQueenMoves(const Position &pos, Square square)
  {
    return countPseudoLegalRookMoves(pos, square) + countPseudoLegalBishopMoves(pos, square);
  }

  int MoveGenerator::countPseudoLegalKnightMoves(const Position &pos, Square square)
  {
    int count = 0;

    const int row = square / 8;
    const int col = square % 8;
    const int is_white_piece = pos.getPiece(square) > 0 ? 1 : -1;

    int row_offset[8] = {2, 1, -1, -2, -2, -1, 1, 2};
    int col_offset[8] = {1, 2, 2, 1, -1, -2, -2, -1};

    for (int i = 0; i < 8; ++i)
    {
      int new_row = row + row_offset[i];
      int new_col = col + col_offset[i];
      if (new_row >= 0 && new_row < 8 && new_col >= 0 && new_col < 8)
      {
        int dest_square = new_row * 8 + new_col;
        if (pos.getPiece(dest_square) * is_white_piece < 1)
        {
          ++count;
        };
      }
    }

    return count;
  }
}
