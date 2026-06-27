#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "graph.hpp"
#include <iosfwd>

namespace levkin {
  using cmd_t = void (*)(std::istream&, std::ostream&, DB&);

  void cmdGraphs(std::istream& input, std::ostream& output, DB& graphs);
  void cmdVertexes(std::istream& input, std::ostream& output, DB& graphs);
  void cmdOutbound(std::istream& input, std::ostream& output, DB& graphs);
  void cmdInbound(std::istream& input, std::ostream& output, DB& graphs);
  void cmdBind(std::istream& input, std::ostream& output, DB& graphs);
  void cmdCut(std::istream& input, std::ostream& output, DB& graphs);
  void cmdCreate(std::istream& input, std::ostream& output, DB& graphs);
  void cmdMerge(std::istream& input, std::ostream& output, DB& graphs);
  void cmdExtract(std::istream& input, std::ostream& output, DB& graphs);
}

#endif
