#ifndef IO
#define IO

#include "list.hpp"
#include <istream>
#include <ostream>
#include <string>
#include <utility>

namespace levkin {
  using String = std::string;
  using Lst = List< size_t >;
  using Pair = std::pair< String, Lst >;
  using Data = List< Pair >;
  using Out = std::ostream;
  using In = std::istream;
  using IterList = List< LCIter< size_t > >;
  using IterIter = LIter< LCIter< size_t > >;
  Out& printNames(Out& out, const Data& data);
  IterList getIters(const Data& data);
  In& readData(In&, Data& data);
  Out& printTransposed(Out&, const Data& data);
  Out& printSums(Out&, const Data& data);
}

#endif
