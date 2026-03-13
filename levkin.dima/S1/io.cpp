#include "io.hpp"
#include "list.hpp"
#include <cstdio>
namespace levkin
{
  using IterList = List< LCIter< size_t > >;

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
  Out& printTransposed(Out& out, const Data& data);
  Out& printSums(Out&, const Data& data) {}
}
