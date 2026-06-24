#include "evaluator.hpp"

#include <stdexcept>

namespace chess
{
  EvaluationStrategy getEvaluationStrategy(const std::string& strategy)
  {
    if (strategy == "balanced")
    {
      return EvaluationStrategy::BALANCED;
    }
    if (strategy == "aggressive")
    {
      return EvaluationStrategy::AGGRESSIVE;
    }
    if (strategy == "defensive")
    {
      return EvaluationStrategy::DEFENSIVE;
    }
    if (strategy == "material")
    {
      return EvaluationStrategy::MATERIAL;
    }
    if (strategy == "positional")
    {
      return EvaluationStrategy::POSITIONAL;
    }
    throw std::logic_error("invalid strategy");
  }

  EvaluationCoefficients getEvaluationCoefficients(EvaluationStrategy strategy)
  {
    EvaluationCoefficients coefficients;
    switch (strategy)
    {
      case EvaluationStrategy::BALANCED:
        return coefficients;
      case EvaluationStrategy::AGGRESSIVE:
        coefficients.material_ = 95;
        coefficients.mobility_ = 135;
        coefficients.pieceSquareTables_ = 110;
        coefficients.pawnStructure_ = 90;
        coefficients.kingSafety_ = 80;
        return coefficients;
      case EvaluationStrategy::DEFENSIVE:
        coefficients.material_ = 100;
        coefficients.mobility_ = 90;
        coefficients.pieceSquareTables_ = 100;
        coefficients.pawnStructure_ = 120;
        coefficients.kingSafety_ = 150;
        return coefficients;
      case EvaluationStrategy::MATERIAL:
        coefficients.material_ = 140;
        coefficients.mobility_ = 75;
        coefficients.pieceSquareTables_ = 80;
        coefficients.pawnStructure_ = 80;
        coefficients.kingSafety_ = 90;
        return coefficients;
      case EvaluationStrategy::POSITIONAL:
        coefficients.material_ = 85;
        coefficients.mobility_ = 120;
        coefficients.pieceSquareTables_ = 140;
        coefficients.pawnStructure_ = 120;
        coefficients.kingSafety_ = 110;
        return coefficients;
    }
    return coefficients;
  }
}
