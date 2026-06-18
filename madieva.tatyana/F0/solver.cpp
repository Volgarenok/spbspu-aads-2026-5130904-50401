#include "solver.hpp"

bool madieva::analyzeLine(
  const Vector< int > & line,
  const Vector< int > & hints,
  Vector< int > & result)
{
  int lineSize = line.getSize();

  if (hints.getSize() == 0) {
    result.reserve(lineSize);
    bool changed = false;
    for (int i = 0; i < lineSize; ++i) {
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
  for (int i = 0; i < lineSize; ++i) {
    counter.pushBack(0);
  }

  Vector< int > current;
  current.reserve(lineSize);
  for (int i = 0; i < lineSize; ++i) {
    current.pushBack(0);
  }

  int totalVariants = 0;

  generateVariants(0, 0, hints, line, current, counter, totalVariants);

  if (totalVariants == 0) {
    return false;
  }

  result.reserve(lineSize);
  for (int i = 0; i < lineSize; ++i) {
    result.pushBack(0);
  }

  bool changed = false;
  for (int i = 0; i < lineSize; ++i) {
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

void madieva::generateVariants(
  int pos,
  int groupIndex,
  const Vector< int > & hints,
  const Vector< int > & known,
  Vector< int > & current,
  Vector< int > & counter,
  int & totalVariants)
{
  int lineSize = current.getSize();
  int hintsSize = hints.getSize();
  if (groupIndex == hintsSize) {
    for (int i = pos; i < lineSize; ++i) {
      if (current[i] == 1) {
        return;
      }
    }

    for (int i = 0; i < lineSize; ++i) {
      if (known[i] != 0 && known[i] != current[i]) {
        return;
      }
    }
    ++totalVariants;
    for (int i = 0; i < lineSize; ++i) {
      if (current[i] == 1) {
        ++counter[i];
      }
    }
    return;
  }

  int remainingSpace = 0;
    for (int i = groupIndex; i < hintsSize; ++i) {
      remainingSpace += hints[i];
    }
    if (groupIndex < hintsSize - 1) {
      remainingSpace += hintsSize - 1 - groupIndex;
    }
    if (pos + remainingSpace > lineSize) {
      return;
    }
    int groupLength = hints[groupIndex];
    for (int start = pos; start <= lineSize - groupLength; ++start) {
    bool canPlace = true;
    int count = 0;
    for (int i = start; i < start + groupLength && canPlace; ++i) {
      ++count;
      if (known[i] == -1) {
        canPlace = false;
      }
    }
    if (!canPlace) {
      start += count;
      continue;
    }
    if (start > pos) {
      if (known[start - 1] == 1) {
        continue;
      }
    }
    for (int i = start; i < start + groupLength; ++i) {
      current[i] = 1;
    }

    int nextPos = start + groupLength;
    if (groupIndex < hintsSize - 1) {
      for (int emptyPos = nextPos; emptyPos <= lineSize; ++emptyPos) {
        if (emptyPos < lineSize) {
          if (known[emptyPos] == 1) {
            break;
          }
          current[emptyPos] = -1;
        }

        generateVariants(emptyPos + 1, groupIndex + 1, hints, known, current, counter, totalVariants);

        if (emptyPos < lineSize) {
          current[emptyPos] = 0;
        }
      }
    } else {
      generateVariants(nextPos, groupIndex + 1, hints, known, current, counter, totalVariants);
    }

    for (int i = start; i < start + groupLength; ++i) {
      current[i] = 0;
    }
  }
}


