#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "graph.hpp"
#include <iomanip>
#include <iosfwd>
namespace zubarev
{
  void cmd_graphs(std::istream&, std::ostream& out, GraphTable& graph);
  void cmd_vertexes(std::istream& in, std::ostream& out, GraphTable& graph);
  void cmd_outbound(std::istream& in, std::ostream& out, GraphTable& graph);
  void cmd_inbound(std::istream& in, std::ostream& out, GraphTable& graph);
  void cmd_bind(std::istream& in, std::ostream& out, GraphTable& graph);
  void cmd_cut(std::istream& in, std::ostream& out, GraphTable& graph);
  void cmd_create(std::istream& in, std::ostream& out, GraphTable& graph);
  void cmd_merge(std::istream& in, std::ostream& out, GraphTable& graph);
  void cmd_extract(std::istream& in, std::ostream& out, GraphTable& graph);
}

#endif
