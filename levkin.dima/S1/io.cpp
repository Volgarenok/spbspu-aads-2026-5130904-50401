#include "io.hpp"
#include <cstdio>
namespace levkin
{
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
  Out& printTransposed(Out&, const Data& data);
  Out& printSums(Out&, const Data& data);
}
