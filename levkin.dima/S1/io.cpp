#include "io.hpp"
#include <cstdio>
#include <limits>
#include <stdexcept>
#include <sstream>
#include "list.hpp"

levkin::IterList levkin::getIters(const levkin::Data& data)
{
  levkin::IterList res;
  for (levkin::LCIter< levkin::Pair > it = data.cbegin(); it != data.cend(); ++it) {
    res.pushBack(it->second.cbegin());
  }
  return res;
}

levkin::In& levkin::readData(levkin::In& in, levkin::Data& data)
{
  levkin::String line;
  while (std::getline(in, line)) {
    if (line.empty()) {
      continue;
    }
    std::istringstream iss(line);
    levkin::String name;
    if (iss >> name) {
      levkin::Lst currentList;
      size_t digit = 0;
      while (iss >> digit) {
        currentList.pushBack(digit);
      }
      levkin::Pair pair(name, currentList);
      data.pushBack(pair);
    }
  }
  return in;
}

levkin::Out& levkin::printNames(levkin::Out& out, const levkin::Data& data)
{
  if (data.cbegin() == data.cend()) {
    return out;
  }
  levkin::LCIter< levkin::Pair > pairIt = data.cbegin();
  out << pairIt->first;
  ++pairIt;
  for (; pairIt != data.cend(); ++pairIt) {
    out << " " << pairIt->first;
  }
  return out;
}

levkin::Out& levkin::printTransposed(levkin::Out& out, const levkin::Data& data)
{
  if (data.cbegin() == data.cend()) {
    return out;
  }
  levkin::IterList iters = levkin::getIters(data);
  levkin::List< size_t > sums;
  bool overflowed = false;
  bool moreDataLeft = true;

  while (moreDataLeft) {
    size_t rowSum = 0;
    bool hasOutputInRow = false;
    bool firstInRow = true;
    levkin::IterIter iterIter = iters.begin();
    levkin::LCIter< levkin::Pair > dataIter = data.cbegin();

    for (; iterIter != iters.end(); ++iterIter, ++dataIter) {
      if (*iterIter != dataIter->second.cend()) {
        size_t val = **iterIter;
        if (!overflowed) {
          if (std::numeric_limits< size_t >::max() - rowSum < val) {
            overflowed = true;
          } else {
            rowSum += val;
          }
        }
        if (!firstInRow) {
          out << " ";
        }
        out << val;
        firstInRow = false;
        hasOutputInRow = true;
        ++(*iterIter);
      }
    }

    if (!hasOutputInRow) {
      moreDataLeft = false;
    } else {
      if (!overflowed) {
        sums.pushBack(rowSum);
      }
      out << "\n";
    }
  }

  if (overflowed) {
    throw std::overflow_error("can't fit sum in size_t. wierd");
  }

  if (sums.size() > 0) {
    levkin::LCIter< size_t > sumIt = sums.cbegin();
    out << *sumIt;
    ++sumIt;
    for (; sumIt != sums.cend(); ++sumIt) {
      out << " " << *sumIt;
    }
  } else {
    out << "0";
  }
  return out;
}