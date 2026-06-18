#include "solver.hpp"

bool madieva::analyzeLine(
  const Vector< int > & line,
  const Vector< int > & hints,
  Vector< int > & result)
{
  size_t lineSize = line.getSize();

  if (hints.getSize() == 0) {
    result.reserve(lineSize);
    bool changed = false;
    for (size_t i = 0; i < lineSize; ++i) {
      if (line[i] == 1) {
        return false;
      }
      result.pushBack(-1);
      if (line[i] != -1) {
        changed = true;
      }
    }
    return changed;
  }

  Vector< int > counter;
  counter.reserve(lineSize);
  for (size_t i = 0; i < lineSize; ++i) {
    counter.pushBack(0);
  }

  Vector< int > current;
  current.reserve(lineSize);
  for (size_t i = 0; i < lineSize; ++i) {
    current.pushBack(0);
  }

  int totalVariants = 0;

  generateVariants(0, 0, hints, line, current, counter, totalVariants);

  if (totalVariants == 0) {
    return false;
  }

  result.reserve(lineSize);
  for (size_t i = 0; i < lineSize; ++i) {
    result.pushBack(0);
  }

  bool changed = false;
  for (size_t i = 0; i < lineSize; ++i) {
    if (counter[i] == totalVariants) {
      result[i] = 1;
      if (line[i] != 1) {
        changed = true;
      }
    } else if (counter[i] == 0) {
      result[i] = -1;
      if (line[i] != -1) {
        changed = true;
      }
    }
  }

  return changed;
}
