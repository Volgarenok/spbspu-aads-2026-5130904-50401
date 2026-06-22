#ifndef ANALYTICS_HPP
#define ANALYTICS_HPP
#include <iostream>
#include "cafe.hpp"

namespace zharov
{
  void cmdOptimize(std::ostream&, std::istream&, CafeSystem&);
  void analyzeItem(std::ostream&, std::istream&, const CafeSystem&);
}
#endif
