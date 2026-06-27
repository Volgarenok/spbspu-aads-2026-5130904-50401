#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include "graph.hpp"

namespace chernov {
  using cmd_t = void (*)(std::istream &, std::ostream &, Graphs &);

  void cmdGraphs(std::istream & input, std::ostream & output, Graphs & graphs);
  void cmdVertexes(std::istream & input, std::ostream & output, Graphs & graphs);
  void cmdOutbound(std::istream & input, std::ostream & output, Graphs & graphs);
  void cmdInbound(std::istream & input, std::ostream & output, Graphs & graphs);
  void cmdBind(std::istream & input, std::ostream & output, Graphs & graphs);
  void cmdCut(std::istream & input, std::ostream & output, Graphs & graphs);
  void cmdCreate(std::istream & input, std::ostream & output, Graphs & graphs);
  void cmdMerge(std::istream & input, std::ostream & output, Graphs & graphs);
  void cmdExtract(std::istream & input, std::ostream & output, Graphs & graphs);
}

#endif
