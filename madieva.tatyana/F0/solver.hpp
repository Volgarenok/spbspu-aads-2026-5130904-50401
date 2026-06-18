#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <vector.hpp>

namespace madieva
{
  void generateVariants(
    int pos,
    int groupIndex,
    const Vector< int > & hints,
    const Vector< int > & known,
    Vector< int > & current,
    Vector< int > & counter,
    int & totalVariants);

  bool analyzeLine(
    const Vector< int > & line,
    const Vector< int > & hints,
    Vector< int > & result);
}

#endif
