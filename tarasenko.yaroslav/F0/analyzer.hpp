#ifndef ANALYZER_HPP
#define ANALYZER_HPP

#include "engine.hpp"
#include "game.hpp"

namespace chess
{
  struct Analyzer
  {
    explicit Analyzer(const EvaluationCoefficients& coefficients);

    double analyze(const Game& game, int depth, int time_ms = 0);

  private:
    EvaluationCoefficients coefficients_;

    double getMoveAccuracy(float before, float after, bool white_to_move) const;
    float evaluate(Position& pos, int depth, int time_ms) const;
  };
}

#endif
