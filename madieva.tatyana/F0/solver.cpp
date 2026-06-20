#include "solver.hpp"

bool madieva::analyzeLine(
  const Vector< int > & line,
  const Vector< size_t > & hints,
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

  Vector< size_t > counter;
  counter.reserve(lineSize);
  for (size_t i = 0; i < lineSize; ++i) {
    counter.pushBack(0);
  }

  Vector< int > current;
  current.reserve(lineSize);
  for (size_t i = 0; i < lineSize; ++i) {
    current.pushBack(0);
  }

  size_t totalVariants = 0;

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

void madieva::generateVariants(
  size_t pos,
  size_t groupIndex,
  const Vector< size_t > & hints,
  const Vector< int > & known,
  Vector< int > & current,
  Vector< size_t > & counter,
  size_t & totalVariants)
{
  size_t lineSize = current.getSize();
  size_t hintsSize = hints.getSize();
  if (groupIndex == hintsSize) {
    for (size_t i = pos; i < lineSize; ++i) {
      if (current[i] == 1) {
        return;
      }
    }

    for (size_t i = 0; i < lineSize; ++i) {
      int value = current[i];
      if (value == 0) {
        value = -1;
      }
      if (known[i] != 0 && known[i] != value) {
        return;
      }
    }

    ++totalVariants;
    for (size_t i = 0; i < lineSize; ++i) {
      if (current[i] == 1) {
        ++counter[i];
      }
    }
    return;
  }

  size_t remainingSpace = 0;
  for (size_t i = groupIndex; i < hintsSize; ++i) {
    remainingSpace += hints[i];
  }
  if (groupIndex < hintsSize - 1) {
    remainingSpace += hintsSize - 1 - groupIndex;
  }
  if (pos + remainingSpace > lineSize) {
    return;
  }
  size_t groupLength = hints[groupIndex];
  for (size_t start = pos; start <= lineSize - groupLength; ++start) {
    bool canPlace = true;
    size_t count = 0;
    for (size_t i = start; i < start + groupLength && canPlace; ++i) {
      ++count;
      if (known[i] == -1) {
        canPlace = false;
      }
    }
    if (!canPlace) {
      start += count - 1;
      continue;
    }
    if (start > pos) {
      if (known[start - 1] == 1) {
        continue;
      }
    }
    for (size_t i = start; i < start + groupLength; ++i) {
      current[i] = 1;
    }

    size_t nextPos = start + groupLength;
    if (groupIndex < hintsSize - 1) {
      for (size_t emptyPos = nextPos; emptyPos <= lineSize; ++emptyPos) {
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

    for (size_t i = start; i < start + groupLength; ++i) {
      current[i] = 0;
    }
  }
}

madieva::Vector< madieva::Vector< int > > madieva::solvePuzzle(
    const Vector< Vector< size_t > > & rowHints,
    const Vector< Vector< size_t > > & colHints,
    size_t rows,
    size_t cols)
{
  Vector< Vector< int > > picture;
  picture.reserve(rows);
  for (size_t i = 0; i < rows; ++i) {
    picture.pushBack(Vector< int >());
    for (size_t j = 0; j < cols; ++j) {
      picture[i].pushBack(0);
    }
  }

  bool changed = true;
  size_t iterations = 0;
  const size_t MAX_ITERATIONS = 1000;

  while (changed && iterations < MAX_ITERATIONS) {
    changed = false;
    ++iterations;
    for (size_t i = 0; i < rows; ++i) {
      Vector< int > newLine;
      bool lineChanged = analyzeLine(picture[i], rowHints[i], newLine);

      if (lineChanged) {
        picture[i] = newLine;
        changed = true;
      }
    }

    for (size_t j = 0; j < cols; ++j) {
      Vector< int > column;
      column.reserve(rows);
      for (size_t i = 0; i < rows; ++i) {
        column.pushBack(picture[i][j]);
      }

      Vector< int > newColumn;
      bool colChanged = analyzeLine(column, colHints[j], newColumn);

      if (colChanged) {
        for (size_t i = 0; i < rows; ++i) {
          picture[i][j] = newColumn[i];
        }
        changed = true;
      }
    }
  }

  bool isFullySolved = true;
  for (size_t i = 0; i < rows && isFullySolved; ++i) {
    for (size_t j = 0; j < cols && isFullySolved; ++j) {
      if (picture[i][j] == 0) {
        isFullySolved = false;
      }
    }
  }

  if (isFullySolved) {
    return picture;
  }

  return Vector< Vector< int > >();
}
