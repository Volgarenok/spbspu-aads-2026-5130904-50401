#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <vector.hpp>

namespace madieva
{
  bool analyzeLine(
    const Vector< int > & line,
    const Vector< int > & hints,
    Vector< int > & result);
}

#endif
