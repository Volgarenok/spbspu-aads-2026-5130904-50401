#include "io.hpp"
#include <cstdio>
#include <limits>
#include <stdexcept>
#include "list.hpp"

namespace levkin {

  IterList getIters(const Data& data)
  {
    IterList res;

    for (LCIter< Pair > it = data.cbegin(); it != data.cend(); ++it) {
      res.pushBack(it->second.cbegin());
    }
    return res;
  }

  In& readData(In& in, Data& data)
  {
    while (in) {
      String name;
      if (!(in >> name))
        break;

      Lst currentList;
      while (in.peek() == ' ' || in.peek() == '\t') {
        in.ignore();
      }
      while (in.peek() != '\n' && in.peek() != '\r' && in.peek() != EOF) {
        size_t digit = 0;
        if (in >> digit) {
          currentList.pushBack(digit);
        } else {
          in.clear();
          break;
        }
        while (in.peek() == ' ' || in.peek() == '\t') {
          in.ignore();
        }
      }
      Pair pair(name, currentList);
      data.pushBack(pair);

      if (in.peek() == '\n' || in.peek() == '\r') {
        in.ignore();
      }
    }
    return in;
  }
  Out& printNames(Out& out, const Data& data)
  {
    LCIter< Pair > pairIt = data.cbegin();
    for (; pairIt != data.cend(); ++pairIt) {
      if (pairIt != data.cbegin()) {
        out << " ";
      }
      out << pairIt->first;
    }
    out << '\n';

    return out;
  }
  Out& printTransposed(Out& out, const Data& data)
  {
    if (data.cbegin() == data.cend()) {
      return out;
    }
    IterList iters = getIters(data);
    List< size_t > sums;
    
    bool rollOneMoreTime = true;
    
    while (rollOneMoreTime) {
      rollOneMoreTime = false;
      size_t rowSum = 0;
      bool firstInRow = true;

      IterIter iterIter = iters.begin();
      LCIter< Pair > dataIter = data.cbegin();

      for (; iterIter != iters.end(); ++iterIter, ++dataIter) {
        if (*iterIter != dataIter->second.cend()) {
          if (!firstInRow) {
            out << " ";
          }

          out << **iterIter;
          firstInRow = false;

          size_t val = **iterIter;
          if (std::numeric_limits< size_t >::max() - rowSum < val) {
            throw std::overflow_error("can't fit sum in size_t");
          }
          rowSum += val;
          ++(*iterIter);

          rollOneMoreTime = true;
        }
      }
      if (rollOneMoreTime) {
        sums.pushBack(rowSum);
        out << "\n";
      }
    }

    if (sums.size() > 0) {
      LCIter< size_t > sumIt = sums.cbegin();
      out << *sumIt;
      ++sumIt;
      for (; sumIt != sums.cend(); ++sumIt) {
        out << " " << *sumIt;
      }
      out << "\n";
    } else {
      out << "0\n";
    }
    return out;
  }

}
