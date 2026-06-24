#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "evaluator.hpp"
#include "position.hpp"
#include "move.hpp"
#include "transposition_table.hpp"
#include "zobrist.hpp"
#include <utility>
#include <cstdint>
#include <chrono>

namespace chess
{
  struct SearchNodes
  {
    uint64_t nnodes = 0;
    uint64_t qnodes = 0;
  };

  struct Engine
  {
  private:
    TranspositionTable tt_;
    Evaluator evaluator_;
    bool use_time_ = false;
    bool stopped_ = false;
    std::chrono::steady_clock::time_point deadline_;

    bool isTimeUp();

  public:
    Engine();
    explicit Engine(const EvaluationCoefficients& coefficients);
    Engine(uint64_t size);

    std::pair< Move, float> findBestMove(Position& pos, int depth, SearchNodes* nodes = nullptr, int time_ms = 0);
    std::pair< Move, int> searchRoot(Position& pos, MoveArray& moves, uint64_t init_hash,
      int depth, int alpha, int beta, SearchNodes& nodes, const Move& prev_best);
    int negamax(Position& pos, int depth, int alpha, int beta, int ply, SearchNodes& nodes, uint64_t hash,
      bool allow_null = true);
    int quiescence(Position& pos, int alpha, int beta, int ply, SearchNodes& qnode);

    void rateMoves(MoveArray& moves, Position& pos, const Move& tt_move = null_move, const Move& prev_best = null_move);
    void rateMove(Move& move, Position& pos, const Move& tt_move, const Move& prev_best);
    void rateCaptures(MoveArray& moves, Position& pos);
    void rateCapture(Move& move, Position& pos);
    void MvBestMoveToBeg(MoveArray& moves, int ind);

    Move leastValuableAttacker(const Position& pos, int square);
    int see(Position& pos, int square);
    int seeCapture(Position& pos, const Move& move);
  };
};

#endif
