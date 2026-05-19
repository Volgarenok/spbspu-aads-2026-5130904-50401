#include "commands.hpp"

#include <stdexcept>
#include <utility>

void studilova::Print(std::istream& in, std::ostream& out, Datasets& datasets)
{
  std::string name;
  if (!(in >> name))
  {
    throw std::runtime_error("invalid input");
  }

  const Dataset& dataset = datasets.get(name);

  if (dataset.empty())
  {
    out << "EMPTY\n";
    return;
  }

  out << name;

  for (Dataset::CIt it = dataset.cbegin(); it != dataset.cend(); ++it)
  {
    std::pair< const int&, const std::string& > pair = *it;
    out << ' ' << pair.first << ' ' << pair.second;
  }

  out << '\n';
}