#include "analyzer.hpp"

#include <cmath>

namespace chess
{
  Analyzer::Analyzer(const EvaluationCoefficients& coefficients):
    coefficients_(coefficients)
  {}

  double Analyzer::analyze(const Game& game, int depth, int time_ms)
  {
    if (game.getMovesCount() == 0)
    {
      return 1;
    }

    Position pos = game.getStartPosition();
    double sum = 0;
    for (size_t i = 0; i < game.getMovesCount(); ++i)
    {
      bool white_to_move = pos.isWhiteToMove();
      float before = evaluate(pos, depth, time_ms);
      UndoInfo undo;
      Move move = game.getMove(i);
      pos.makeMove(move, undo);
      float after = evaluate(pos, depth, time_ms);
      sum += getMoveAccuracy(before, after, white_to_move);
    }
    return sum / game.getMovesCount();
  }

  double Analyzer::getMoveAccuracy(float before, float after, bool white_to_move) const
  {
    double loss = white_to_move ? before - after : after - before;
    if (loss <= 0)
    {
      return 1;
    }
    double accuracy = 1 - loss / 2;
    if (accuracy < 0)
    {
      return 0;
    }
    return accuracy;
  }

  float Analyzer::evaluate(Position& pos, int depth, int time_ms) const
  {
    Engine engine(coefficients_);
    return engine.findBestMove(pos, depth, nullptr, time_ms).second;
  }
}
