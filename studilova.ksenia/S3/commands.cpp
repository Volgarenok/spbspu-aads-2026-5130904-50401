#include "commands.hpp"

void studilova::initCommands(CommandsMap& commands)
{
  commands.add("graphs", graphs);
  commands.add("vertexes", vertexes);
  commands.add("outbound", outbound);
  commands.add("inbound", inbound);
  commands.add("bind", bind);
  commands.add("cut", cut);
  commands.add("create", create);
  commands.add("merge", merge);
  commands.add("extract", extract);
}

void studilova::graphs(std::istream&, std::ostream& out, GraphsMap& graphs)
{
  Vector< std::string > names;

  for (auto it = graphs.begin(); it != graphs.end(); ++it)
  {
    auto item = *it;
    names.pushBack(item.first);
  }

  sortVector(names, compareStrings);

  for (size_t i = 0; i < names.getSize(); ++i)
  {
    out << names[i] << "\n";
  }
}

void studilova::vertexes(std::istream& in, std::ostream& out, GraphsMap& graphs)
{
  std::string graphName;
  in >> graphName;

  if (!in || !graphs.has(graphName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  Vector< std::string > vertices = graphs.get(graphName).getVertices();

  sortVector(vertices, compareStrings);

  for (size_t i = 0; i < vertices.getSize(); ++i)
  {
    out << vertices[i] << "\n";
  }
}