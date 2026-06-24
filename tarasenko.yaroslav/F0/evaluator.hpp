#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP

#include "position.hpp"
#include <string>

namespace chess
{
  struct EvaluationCoefficients
  {
    int material_ = 100;
    int mobility_ = 100;
    int pieceSquareTables_ = 100;
    int pawnStructure_ = 100;
    int kingSafety_ = 100;
  };

  enum class EvaluationStrategy
  {
    BALANCED,
    AGGRESSIVE,
    DEFENSIVE,
    MATERIAL,
    POSITIONAL
  };

  struct Evaluator
  {
    Evaluator();
    explicit Evaluator(const EvaluationCoefficients& coefficients);

    int evaluate(const Position& pos) const;
    int relative_eval(const Position& pos) const;

    static void material(Piece piece, int& eval);
    static void mobility(const Position& pos, int square, Piece piece, int& eval);
    static void piece_square_tables(int square, Piece piece, int& eval);
    static void pawn_structure_fill(Piece piece, int square, int* white, int* black);
    static void pawn_structure_eval(int* white, int* black, int& eval);
    static void king_safety(const Position& pos, int& eval);

  private:
    EvaluationCoefficients coefficients_;

    static int applyCoefficient(int eval, int coefficient);
  };

  EvaluationStrategy getEvaluationStrategy(const std::string& strategy);
  EvaluationCoefficients getEvaluationCoefficients(EvaluationStrategy strategy);
}

#endif
