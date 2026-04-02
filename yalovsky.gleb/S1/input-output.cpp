#include "input-output.hpp"

#include <algorithm>
#include <istream>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <utility>

void yalovsky::readSequences(std::istream& in, SequenceList& sequences)
{
  while (true)
  {
    std::string name;
    if (!(in >> name))
    {
      return;
    }

    NumberList numbers;

    while (true)
    {
      std::size_t value = 0;
      if (in >> value)
      {
        numbers.pushBack(value);
      }
      else
      {
        if (in.eof())
        {
          sequences.pushBack(std::make_pair(name, std::move(numbers)));
          return;
        }
        in.clear();
        break;
      }
    }

    sequences.pushBack(std::make_pair(name, std::move(numbers)));
  }
}

void yalovsky::printNames(std::ostream& out, const SequenceList& sequences)
{
  SequenceList::const_iterator it = sequences.cbegin();

  out << it->first;
  ++it;

  for (; it != sequences.cend(); ++it)
  {
    out << ' ' << it->first;
  }

  out << '\n';
}

void yalovsky::transposeSequences(const SequenceList& sequences, Matrix& result)
{
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

      NumberList::const_iterator numIt = it->second.cbegin() + index;
      row.pushBack(*numIt);
    }

    result.pushBack(std::move(row));
  }
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
    out << ' ' << *it;
  }

  out << '\n';
}

void yalovsky::printMatrix(std::ostream& out, const Matrix& matrix)
{
  for (Matrix::const_iterator it = matrix.cbegin(); it != matrix.cend(); ++it)
  {
    printNumberList(out, *it);
  }
}

namespace
{
  std::size_t addChecked(std::size_t a, std::size_t b)
  {
    const std::size_t maxVal = std::numeric_limits< std::size_t >::max();

    if (maxVal - a < b)
    {
      throw std::overflow_error("overflow");
    }

    return a + b;
  }
}