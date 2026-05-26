#include "graphs-commands.hpp"

void donkeev::sortNames(topit::Vector< std::string >& vector)
{
  for (size_t i = 0; i < vector.getSize(); ++i)
  {
    size_t minId = i;
    for (size_t j = i + 1; j < vector.getSize(); ++j)
    {
      if (vector[j] < vector[minId])
      {
        minId = j;
      }
    }

    std::swap(vector[i], vector[minId]);
  }
}

void donkeev::printGrapsNames(graphsHashTable_t graphsTable, const std::string&, std::ostream& out)
{
  topit::Vector< std::string > sortedVector;

  donkeev::HTIt< std::string, donkeev::Graph > begin = graphsTable.begin();
  donkeev::HTIt< std::string, donkeev::Graph > end = graphsTable.end();
  for (; begin != end; ++begin)
  {
    sortedVector.pushBack((*begin).first);
  }

  sortNames(sortedVector);
  for (size_t i = 0; i < sortedVector.getSize(); ++i)
  {
    out << sortedVector[i] << '\n';
  }
}
