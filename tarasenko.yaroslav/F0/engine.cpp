#include "engine.hpp"
#include "evaluator.hpp"
#include "move.hpp"
#include "move_generator.hpp"
#include "piece.hpp"
#include "position.hpp"
#include "transposition_table.hpp"
#include "zobrist.hpp"
#include "piece_square_tables.hpp"
#include <cstdint>
#include <chrono>

namespace chess
{
  const int MIN = -30000;
  const int MATE = 29000;

  bool useTTEntry(const TTEntry& entry, uint64_t hash, int depth, int alpha, int beta, Move& tt_move, int& eval)
  {
    if (!entry.used_ || entry.key_ != hash)
    {
      return false;
    }

    if (entry.depth_ >= depth)
    {
      if (entry.type_ == EXACT)
      {
        eval = entry.eval_;
        return true;
      }
      else if (entry.type_ == LOWER_BOUND && entry.eval_ >= beta)
      {
        eval = entry.eval_;
        return true;
      }
      else if (entry.type_ == UPPER_BOUND && entry.eval_ <= alpha)
      {
        eval = entry.eval_;
        return true;
      }
    }
    tt_move = entry.bestMove_;
    return false;
  }

  TTEntryType getTTEntryType(int eval, int alpha_orig, int beta)
  {
    if (eval <= alpha_orig)
    {
      return UPPER_BOUND;
    }
    else if (eval >= beta)
    {
      return LOWER_BOUND;
    }
    return EXACT;
  }

  int pieceSquareDelta(Position& pos, const Move& move)
  {
    Piece cur_piece = static_cast< Piece >(pos.getPiece(move.from_));
    int abs_piece = cur_piece > 0 ? cur_piece : -cur_piece;
    switch (abs_piece)
    {
      case WHITE_KING:
        return king_table[move.to_] - king_table[move.from_];
      case WHITE_QUEEN:
        return queen_table[move.to_] - queen_table[move.from_];
      case WHITE_KNIGHT:
        return knight_table[move.to_] - knight_table[move.from_];
      case WHITE_BISHOP:
        return bishop_table[move.to_] - bishop_table[move.from_];
      case WHITE_PAWN:
        return pawn_table[move.to_] - pawn_table[move.from_];
    }
    return 0;
  }

  bool isQuietMove(Position& pos, const Move& move)
  {
    return pos.getPiece(move.to_) == EMPTY
      && move.promotionPiece_ == EMPTY
      && !move.isEnPassant_
      && !move.isCastling_;
  }

  int getLMRReduction(int depth, int move_number)
  {
    int reduction = 1 + move_number / 4;
    if (reduction > depth - 2)
    {
      reduction = depth - 2;
    }
    return reduction;
  }

  bool canReduceMove(Position& pos, const Move& move, int depth, int move_number)
  {
    return depth > 3 && move_number > 2 && isQuietMove(pos, move);
  }
  uint64_t nullMoveHash(const Position& pos)
  {
    Position null_pos = pos.getToggledSideToMovePosition();
    null_pos.setEnPassantSquare(-1);
    return zobristHash(null_pos);
  }

  bool canTryNullMove(int depth, bool in_check, bool allow_null)
  {
    return allow_null && depth >= 3 && !in_check;
  }

  int Engine::negamax(Position& pos, int depth, int alpha, int beta, int ply, SearchNodes& nodes, uint64_t hash,
    bool allow_null)
  {
    if (isTimeUp())
    {
      return alpha;
    }

    Move tt_move = null_move;
    TTEntry entry = tt_.getEntry(hash);
    int tt_eval = 0;
    if (useTTEntry(entry, hash, depth, alpha, beta, tt_move, tt_eval))
    {
      return tt_eval;
    }

    if (depth == 0)
    {
      return quiescence(pos, alpha, beta, ply, nodes);
    }

    MoveGenerator gen;
    bool in_check = gen.isCheck(pos);
    if (canTryNullMove(depth, in_check, allow_null))
    {
      int reduction = depth >= 6 ? 3 : 2;
      Position null_pos = pos.getToggledSideToMovePosition();
      null_pos.setEnPassantSquare(-1);
      int null_eval = -negamax(null_pos, depth - 1 - reduction, -beta, -beta + 1, ply + 1, nodes,
        nullMoveHash(pos), false);
      if (null_eval >= beta)
      {
        return beta;
      }
    }

    MoveArray moves = gen.generatePseudoLegalMoves(pos);

    rateMoves(moves, pos, tt_move);

    int alpha_orig = alpha;
    int eval = MIN;
    UndoInfo undo;
    Move best = null_move;
    bool side = !pos.isWhiteToMove();
    bool no_moves = true;
    int move_number = 0;
    for (int i = 0; i < moves.size(); ++i)
    {
      MvBestMoveToBeg(moves, i);
      Move move = moves.get(i);
      bool can_reduce = canReduceMove(pos, move, depth, move_number);
      uint64_t cur_hash = incrementZobristHash(hash, pos, move);
      pos.makeMove(move, undo);
      if (gen.isSquareAttackedQuick(pos, static_cast< Square >(pos.getOppositeColourKingSquare()), side))
      {
        pos.undoMove(move, undo);
        continue;
      }
      no_moves = false;
      ++nodes.nnodes;
      int reduction = can_reduce ? getLMRReduction(depth, move_number) : 0;
      int curr_eval = -negamax(pos, depth - 1 - reduction, -beta, -alpha, ply + 1, nodes, cur_hash);
      if (stopped_)
      {
        pos.undoMove(move, undo);
        return alpha;
      }
      if (reduction && curr_eval > alpha)
      {
        curr_eval = -negamax(pos, depth - 1, -beta, -alpha, ply + 1, nodes, cur_hash);
        if (stopped_)
        {
          pos.undoMove(move, undo);
          return alpha;
        }
      }
      if (curr_eval > eval)
      {
        eval = curr_eval;
        best = move;
      }
      pos.undoMove(move, undo);
      ++move_number;

      alpha = std::max(alpha, eval);
      if (alpha >= beta)
      {
        break;
      }
    }

    if (no_moves)
    {
      if (gen.isCheck(pos))
      {
        return -MATE + ply;
      }
      return 0;
    }

    TTEntryType type = getTTEntryType(eval, alpha_orig, beta);
    tt_.addEntry(TTEntry{hash, eval, depth, type, true, best});

    return eval;
  }

  int Engine::quiescence(Position& pos, int alpha, int beta, int ply, SearchNodes& nodes)
  {
    if (isTimeUp())
    {
      return alpha;
    }

    UndoInfo undo;
    MoveArray moves;

    bool isActive = 0;

    MoveGenerator gen;
    if (gen.isCheck(pos))
    {
      moves = gen.generatePseudoLegalMoves(pos);
    }

    else
    {
      int stand_pat = evaluator_.relative_eval(pos);
      alpha = std::max(alpha, stand_pat);
      if (stand_pat >= beta)
      {
        return beta;
      }

      gen.generatePseudoLegalActiveMoves(pos, moves);
      isActive = 1;
    }

    isActive ? rateCaptures(moves, pos) : rateMoves(moves, pos);
    int eval = MIN;
    bool side = !pos.isWhiteToMove();
    bool no_moves = true;
    for (int i = 0; i < moves.size(); ++i)
    {
      MvBestMoveToBeg(moves, i);
      if (moves.get(i).score_ == -1000)
      {
        break;
      }
      pos.makeMove(moves.get(i), undo);
      if (gen.isSquareAttackedQuick(pos, static_cast< Square >(pos.getOppositeColourKingSquare()), side))
      {
        pos.undoMove(moves.get(i), undo);
        continue;
      }
      no_moves = false;
      ++nodes.qnodes;
      eval = std::max(eval, -quiescence(pos, -beta, -alpha, ply + 1, nodes));
      if (stopped_)
      {
        pos.undoMove(moves.get(i), undo);
        return alpha;
      }
      pos.undoMove(moves.get(i), undo);

      alpha = std::max(alpha, eval);
      if (alpha >= beta)
      {
        return beta;
      }
    }

    if (no_moves && !isActive)
    {
      return -MATE + ply;
    }

    return alpha;
  }

  std::pair< Move, float > Engine::findBestMove(Position& pos, int depth, SearchNodes* nodes, int time_ms)
  {
    use_time_ = time_ms > 0;
    stopped_ = false;
    if (use_time_)
    {
      deadline_ = std::chrono::steady_clock::now() + std::chrono::milliseconds(time_ms);
    }

    SearchNodes local_nodes;
    SearchNodes& search_nodes = nodes ? *nodes : local_nodes;
    uint64_t init_hash = zobristHash(pos);
    MoveGenerator gen;
    MoveArray moves = gen.generateLegalMoves(pos);
    if (moves.empty())
    {
      use_time_ = false;
      if (gen.isCheck(pos))
      {
        return {null_move,-MATE};
      }
      return {null_move,0};
    }
    Move best_move = moves.get(0);
    int best_eval = 0;
    int max_depth = use_time_ ? 64 : depth;
    for (int cur_depth = 1; cur_depth <= max_depth; ++cur_depth)
    {
      if (isTimeUp())
      {
        break;
      }
      int eval = 0;

      int window = 50;
      int alpha = cur_depth <= 4 ? MIN : best_eval - 50;
      int beta = cur_depth <= 4 ? -MIN : best_eval + 50;
      while (true)
      {
        std::pair< Move, int > result = searchRoot(pos, moves, init_hash,
          cur_depth, alpha, beta, search_nodes, best_move);
        if (stopped_)
        {
          break;
        }
        eval = result.second;

        if (eval <= alpha)
        {
          alpha -= window;
        }
        else if (eval >= beta)
        {
          beta += window;
        }
        else
        {
          best_move = result.first;
          break;
        }
        window *= 2;
      }
      if (stopped_)
      {
        break;
      }
      best_eval = eval;
    }
    use_time_ = false;
    return {best_move, (pos.isWhiteToMove() ? best_eval : -best_eval) / 100.0};
  }

  std::pair< Move, int > Engine::searchRoot(Position& pos, MoveArray& moves, uint64_t init_hash,
    int depth, int alpha, int beta, SearchNodes& nodes, const Move& prev_best)
  {
    int eval = MIN;
    Move best_move = prev_best;
    UndoInfo undo;

    rateMoves(moves, pos, null_move, prev_best);
    for (int i = 0; i < moves.size(); ++i)
    {
      if (isTimeUp())
      {
        break;
      }
      MvBestMoveToBeg(moves, i);
      uint64_t hash = incrementZobristHash(init_hash, pos, moves.get(i));
      pos.makeMove(moves.get(i), undo);
      int cur_eval = -negamax(pos, depth - 1, -beta, -alpha, 0, nodes, hash);
      if (stopped_)
      {
        pos.undoMove(moves.get(i), undo);
        break;
      }
      if (cur_eval > eval)
      {
        eval = cur_eval;
        best_move = moves.get(i);
        alpha = eval;
      }
      pos.undoMove(moves.get(i), undo);

      if (alpha >= beta)
      {
        break;
      }
    }

    return {best_move, eval};
  }

  void Engine::rateMoves(MoveArray& moves, Position& pos, const Move& tt_move, const Move& prev_best)
  {
    for (int i = 0; i < moves.size(); ++i)
    {
      rateMove(moves.moves_[i], pos, tt_move, prev_best);
    }
  }

  void Engine::rateMove(Move& move, Position& pos, const Move& tt_move, const Move& prev_best)
  {
    int score = 0;

    if (move == prev_best)
    {
      move.score_ = 200000;
      return;
    }

    if (move == tt_move)
    {
      move.score_ = 100000;
      return;
    }

    int to = pos.getPiece(move.to_);
    if (to != EMPTY)
    {
      int from = pos.getPiece(move.from_);
      int attacker = weights[std::abs(from)];
      int victim = weights[std::abs(to)];
      if (victim < attacker)
      {
        int see = seeCapture(pos, move);
        if (see < 0)
        {
          score -= 5000 + see;
        }
        else
        {
          score += 5000 + see;
        }
      }
      else
      {
        score += 10000 + weights[std::abs(to)] - weights[std::abs(pos.getPiece(move.from_))] / 8;
      }
    }
    if (move.promotionPiece_ != EMPTY)
    {
      score += 12000 + std::abs(move.promotionPiece_);
    }

    score += pieceSquareDelta(pos, move);

    move.score_ = score;
  }

  void Engine::rateCaptures(MoveArray& moves, Position& pos)
  {
    for (int i = 0; i < moves.size(); ++i)
    {
      rateCapture(moves.moves_[i], pos);
    }
  }

  void Engine::rateCapture(Move& move, Position& pos)
  {
    int score = 0;

    int from = pos.getPiece(move.from_);
    int to = pos.getPiece(move.to_);
    int attacker = weights[std::abs(from)];
    int victim = weights[std::abs(to)];
    if (victim < attacker)
    {
      int see = seeCapture(pos, move);
      if (see < 50)
      {
        move.score_ = -1000;
        return;
      }
      score += 10000 + see;
    }
    else
    {
      score += 10000 + weights[std::abs(to)] - weights[std::abs(pos.getPiece(move.from_))] / 8;
    }

    score += pieceSquareDelta(pos, move);

    move.score_ = score;
  }

  void Engine::MvBestMoveToBeg(MoveArray& moves, int ind)
  {
    int max_eval = MIN;
    int best_ind = ind;
    for (int i = ind; i < moves.size(); ++i)
    {
      int curr = moves.get(i).score_;
      if (curr > max_eval)
      {
        max_eval = curr;
        best_ind = i;
      }
    }

    std::swap(moves.moves_[best_ind], moves.moves_[ind]);
  }

  Engine::Engine()
  {
    initZobristHash();
  }

  Engine::Engine(const EvaluationCoefficients& coefficients):
    evaluator_(coefficients)
  {
    initZobristHash();
  }

  Engine::Engine(uint64_t size):
    tt_(size)
  {
    initZobristHash();
  }

  bool Engine::isTimeUp()
  {
    if (use_time_ && std::chrono::steady_clock::now() >= deadline_)
    {
      stopped_ = true;
      return true;
    }
    return false;
  }

  Move Engine::leastValuableAttacker(const Position& pos, int square)
  {
    const int side = pos.isWhiteToMove() ? 1 : -1;

    Move attacker = MoveGenerator::findPawnAttacker(pos, square, side);
    if (attacker != null_move)
    {
      return attacker;
    }

    attacker = MoveGenerator::findKnightAttacker(pos, square, side);
    if (attacker != null_move)
    {
      return attacker;
    }

    attacker = MoveGenerator::findBishopAttacker(pos, square, side);
    if (attacker != null_move)
    {
      return attacker;
    }

    attacker = MoveGenerator::findRookAttacker(pos, square, side);
    if (attacker != null_move)
    {
      return attacker;
    }

    attacker = MoveGenerator::findQueenAttacker(pos, square, side);
    if (attacker != null_move)
    {
      return attacker;
    }

    return null_move;
  }

  int Engine::see(Position& pos, int square)
  {
    constexpr int max_exchanges = 32;
    Move attacks[max_exchanges];
    UndoInfo undo[max_exchanges];
    int captured_values[max_exchanges];
    int depth = 0;

    while (depth < max_exchanges)
    {
      Move attack = leastValuableAttacker(pos, square);
      if (attack == null_move)
      {
        break;
      }

      attacks[depth] = attack;
      captured_values[depth] = weights[std::abs(pos.getPiece(square))];
      pos.makeMove(attack, undo[depth]);
      ++depth;
    }

    int value = 0;
    for (int i = depth - 1; i >= 0; --i)
    {
      int current_value = captured_values[i] - value;
      value = current_value > 0 ? current_value : 0;
    }

    for (int i = depth - 1; i >= 0; --i)
    {
      pos.undoMove(attacks[i], undo[i]);
    }

    return value;
  }

  int Engine::seeCapture(Position& pos, const Move& move)
  {
    int captured_value = weights[std::abs(pos.getPiece(move.to_))];
    UndoInfo undo;

    pos.makeMove(move, undo);
    int opponent_gain = see(pos, move.to_);
    pos.undoMove(move, undo);

    return captured_value - opponent_gain;
  }
};
