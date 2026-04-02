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