#include "commands.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>

namespace ulanova
{
  CommandProcessor::CommandProcessor(GraphStorage& storage):
    storage_(storage),
    handlers_()
  {
    handlers_["graphs"] = &CommandProcessor::handleGraphs;
    handlers_["vertexes"] = &CommandProcessor::handleVertices;
    handlers_["vertices"] = &CommandProcessor::handleVertices;
    handlers_["outbound"] = &CommandProcessor::handleOutbound;
    handlers_["inbound"] = &CommandProcessor::handleInbound;
    handlers_["bind"] = &CommandProcessor::handleBind;
    handlers_["cut"] = &CommandProcessor::handleCut;
    handlers_["create"] = &CommandProcessor::handleCreate;
    handlers_["merge"] = &CommandProcessor::handleMerge;
    handlers_["extract"] = &CommandProcessor::handleExtract;
  }

  void CommandProcessor::run(std::istream& input, std::ostream& output)
  {
    std::string line;

    while (std::getline(input, line))
    {
      Args args = split(line);

      if (args.empty())
      {
        continue;
      }

      auto it = handlers_.find(args[0]);

      if (it == handlers_.end())
      {
        printInvalid(output);
        continue;
      }

      try
      {
        Handler handler = it->second;
        (this->*handler)(args, output);
      }
      catch (...)
      {
        printInvalid(output);
      }
    }
  }

  CommandProcessor::Args CommandProcessor::split(const std::string& line)
  {
    Args args;
    std::istringstream input(line);
    std::string word;

    while (input >> word)
    {
      args.push_back(word);
    }

    return args;
  }

  bool CommandProcessor::parseUnsigned(const std::string& text, unsigned& value)
  {
    std::istringstream input(text);
    unsigned tmp = 0;
    char tail = '\0';

    if (!(input >> tmp))
    {
      return false;
    }

    if (input >> tail)
    {
      return false;
    }

    value = tmp;
    return true;
  }

  bool CommandProcessor::parseSize(const std::string& text, size_t& value)
  {
    std::istringstream input(text);
    size_t tmp = 0;
    char tail = '\0';

    if (!(input >> tmp))
    {
      return false;
    }

    if (input >> tail)
    {
      return false;
    }

    value = tmp;
    return true;
  }

  void CommandProcessor::sortStrings(Vector<std::string>& values)
  {
    std::sort(values.begin(), values.end());
  }

  void CommandProcessor::sortEdges(Vector<Edge>& edges)
  {
    std::sort(edges.begin(), edges.end(), [](const Edge& lhs, const Edge& rhs)
    {
      if (lhs.vertex != rhs.vertex)
      {
        return lhs.vertex < rhs.vertex;
      }

      return lhs.weight < rhs.weight;
    });
  }

  void CommandProcessor::printEdges(const Vector<Edge>& edges, std::ostream& output)
  {
    if (edges.isEmpty())
    {
      output << '\n';
      return;
    }

    size_t i = 0;

    while (i < edges.getsize())
    {
      output << edges[i].vertex << ' ' << edges[i].weight;

      size_t j = i + 1;
      while (j < edges.getsize() && edges[j].vertex == edges[i].vertex)
      {
        output << ' ' << edges[j].weight;
        ++j;
      }

      output << '\n';
      i = j;
    }
  }

  void CommandProcessor::printInvalid(std::ostream& output)
  {
    output << "<INVALID COMMAND>\n";
  }

  void CommandProcessor::handleGraphs(const Args& args, std::ostream& output)
  {
    if (args.size() != 1)
    {
      printInvalid(output);
      return;
    }

    Vector<std::string> names = storage_.getGraphNames();
    sortStrings(names);

    if (names.isEmpty())
    {
      output << '\n';
      return;
    }

    for (auto it = names.begin(); it != names.end(); ++it)
    {
      output << *it << '\n';
    }
  }

  void CommandProcessor::handleVertices(const Args& args, std::ostream& output)
  {
    if (args.size() != 2)
    {
      printInvalid(output);
      return;
    }

    const Graph* graph = storage_.findGraph(args[1]);

    if (graph == nullptr)
    {
      printInvalid(output);
      return;
    }

    Vector<std::string> vertices = graph->getVertices();
    sortStrings(vertices);
    if (vertices.isEmpty())
    {
      output << '\n';
      return;
    }

    for (auto it = vertices.begin(); it != vertices.end(); ++it)
    {
      output << *it << '\n';
    }
  }

  void CommandProcessor::handleOutbound(const Args& args, std::ostream& output)
  {
    if (args.size() != 3)
    {
      printInvalid(output);
      return;
    }

    const Graph* graph = storage_.findGraph(args[1]);

    if (graph == nullptr || !graph->hasVertex(args[2]))
    {
      printInvalid(output);
      return;
    }

    Vector<Edge> edges = graph->getOutbound(args[2]);
    sortEdges(edges);
    printEdges(edges, output);

  }

  void CommandProcessor::handleInbound(const Args& args, std::ostream& output)
  {
    if (args.size() != 3)
    {
      printInvalid(output);
      return;
    }

    const Graph* graph = storage_.findGraph(args[1]);

    if (graph == nullptr || !graph->hasVertex(args[2]))
    {
      printInvalid(output);
      return;
    }

    Vector<Edge> edges = graph->getInbound(args[2]);
    sortEdges(edges);
    printEdges(edges, output);

  }

  void CommandProcessor::handleBind(const Args& args, std::ostream&)
  {
    if (args.size() != 5)
    {
      throw std::runtime_error("bad bind");
    }

    Graph* graph = storage_.findGraph(args[1]);
    unsigned weight = 0;

    if (graph == nullptr || !parseUnsigned(args[4], weight))
    {
      throw std::runtime_error("bad bind");
    }

    graph->addEdge(args[2], args[3], weight);
  }

  void CommandProcessor::handleCut(const Args& args, std::ostream& output)
  {
    if (args.size() != 5)
    {
      printInvalid(output);
      return;
    }

    Graph* graph = storage_.findGraph(args[1]);
    unsigned weight = 0;

    if (graph == nullptr || !parseUnsigned(args[4], weight))
    {
      printInvalid(output);
      return;
    }

    if (!graph->removeEdge(args[2], args[3], weight))
    {
      printInvalid(output);
    }
  }

  void CommandProcessor::handleCreate(const Args& args, std::ostream& output)
  {
    if (args.size() < 3)
    {
      printInvalid(output);
      return;
    }

    size_t count = 0;

    if (!parseSize(args[2], count) || args.size() != count + 3 || storage_.hasGraph(args[1]))
    {
      printInvalid(output);
      return;
    }

    Graph graph(args[1]);

    for (size_t i = 0; i < count; ++i)
    {
      graph.addVertex(args[i + 3]);
    }

    storage_.addGraph(graph);
  }

  void CommandProcessor::handleMerge(const Args& args, std::ostream& output)
  {
    if (args.size() != 4)
    {
      printInvalid(output);
      return;
    }

    if (storage_.hasGraph(args[1]))
    {
      printInvalid(output);
      return;
    }

    const Graph* lhs = storage_.findGraph(args[2]);
    const Graph* rhs = storage_.findGraph(args[3]);

    if (lhs == nullptr || rhs == nullptr)
    {
      printInvalid(output);
      return;
    }

    Graph result = Graph::merge(args[1], *lhs, *rhs);
    storage_.addGraph(result);
  }

  void CommandProcessor::handleExtract(const Args& args, std::ostream& output)
  {
    if (args.size() < 4)
    {
      printInvalid(output);
      return;
    }

    if (storage_.hasGraph(args[1]))
    {
      printInvalid(output);
      return;
    }

    const Graph* graph = storage_.findGraph(args[2]);
    size_t count = 0;

    if (graph == nullptr || !parseSize(args[3], count) || args.size() != count + 4)
    {
      printInvalid(output);
      return;
    }

    Vector<std::string> vertices;

    for (size_t i = 0; i < count; ++i)
    {
      vertices.push_back(args[i + 4]);
    }

    Graph result = graph->extract(args[1], vertices);
    storage_.addGraph(result);
  }
}
