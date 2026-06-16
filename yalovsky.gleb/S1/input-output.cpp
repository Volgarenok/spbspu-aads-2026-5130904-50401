#include "input-output.hpp"

#include <algorithm>
#include <cstddef>
#include <istream>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>

void yalovsky::readSequences(std::istream& in, SequenceList& sequences)
{
  SequenceList temp;
  std::string name;

  while (in >> name)
  {
    NumberList numbers;
    std::size_t value = 0;

    while (in >> value)
    {
      numbers.pushBack(value);
    }

    temp.pushBack(std::make_pair(std::move(name), std::move(numbers)));

    if (!in.eof())
    {
      in.clear();
    }
  }

  sequences.swap(temp);
}

void yalovsky::printNames(std::ostream& out, const SequenceList& sequences)
{
  SequenceList::const_iterator it = sequences.cbegin();

  out << it->first;
  ++it;

  for (; it != sequences.cend(); ++it)
  {
    out << ' ';
    out << it->first;
  }
}

void yalovsky::transposeSequences(const SequenceList& sequences, Matrix& result)
{
  Matrix temp;
  std::size_t maxSize = 0;

  for (SequenceList::const_iterator it = sequences.cbegin(); it != sequences.cend(); ++it)
  {
    maxSize = std::max(maxSize, it->second.size());
  }

  for (std::size_t index = 0; index < maxSize; ++index)
  {
    NumberList row;

    for (SequenceList::const_iterator it = sequences.cbegin(); it != sequences.cend(); ++it)
    {
      if (it->second.size() <= index)
      {
        continue;
      }

      NumberList::const_iterator valueIt = it->second.cbegin() + index;
      row.pushBack(*valueIt);
    }

    temp.pushBack(std::move(row));
  }

  result.swap(temp);
}

void yalovsky::printNumberList(std::ostream& out, const NumberList& numbers)
{
  if (numbers.empty())
  {
    return;
  }

  NumberList::const_iterator it = numbers.cbegin();

  out << *it;
  ++it;

  for (; it != numbers.cend(); ++it)
  {
    out << ' ';
    out << *it;
  }
}

void yalovsky::printMatrix(std::ostream& out, const Matrix& matrix)
{
  for (Matrix::const_iterator it = matrix.cbegin(); it != matrix.cend(); ++it)
  {
    printNumberList(out, *it);
    out << '\n';
  }
}

namespace
{
  std::size_t addChecked(std::size_t lhs, std::size_t rhs)
  {
    const std::size_t maxValue = std::numeric_limits< std::size_t >::max();

    if ((maxValue - lhs) < rhs)
    {
      throw std::overflow_error("overflow");
    }

    return lhs + rhs;
  }
}

void yalovsky::calculateSums(const Matrix& matrix, NumberList& sums)
{
  NumberList temp;

  for (Matrix::const_iterator rowIt = matrix.cbegin(); rowIt != matrix.cend(); ++rowIt)
  {
    std::size_t sum = 0;

    for (NumberList::const_iterator valueIt = rowIt->cbegin(); valueIt != rowIt->cend(); ++valueIt)
    {
      sum = addChecked(sum, *valueIt);
    }

    temp.pushBack(sum);
  }

  sums.swap(temp);
}
