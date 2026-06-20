#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <vector.hpp>

namespace madieva
{
  void generateVariants(
    size_t pos,
    size_t groupIndex,
    const Vector< size_t > & hints,
    const Vector< int > & known,
    Vector< int > & current,
    Vector< size_t > & counter,
    size_t & totalVariants);

  bool analyzeLine(
    const Vector< int > & line,
    const Vector< size_t > & hints,
    Vector< int > & result);

  Vector< Vector< int > > solvePuzzle(
    const Vector< Vector< size_t > > & rowHints,
    const Vector< Vector< size_t > > & colHints,
    size_t rows,
    size_t cols);
}

#endif
