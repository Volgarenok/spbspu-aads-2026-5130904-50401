#include "commands.hpp"

#include <istream>
#include <string>
#include <utility>
#include <stdexcept>

#include <vector.hpp>
#include <list.hpp>

namespace
{
  void requireStream(const std::istream &stream)
  {
    if (!stream)
    {
      throw std::runtime_error("invalid input");
    }
  }
}

void burukov::helpers::sortStrings(Vector< std::string > &strings)
{
  for (size_t i = 1; i < strings.getSize(); ++i)
  {
    std::string key = strings[i];
    size_t j = i;
    while (j > 0 && strings[j - 1] > key)
    {
      strings[j] = strings[j - 1];
      --j;
    }
    strings[j] = key;
  }
}

void burukov::helpers::sortWeights(Vector< size_t > &weights)
{
  for (size_t i = 1; i < weights.getSize(); ++i)
  {
    size_t key = weights[i];
    size_t j = i;
    while (j > 0 && weights[j - 1] > key)
    {
      weights[j] = weights[j - 1];
      --j;
    }
    weights[j] = key;
  }
}

void burukov::commandGraphs(std::istream &, std::ostream &outputStream, GraphDatabase &database)
{
  Vector< std::string > names;
  for (auto it = database.cbegin(); it != database.cend(); ++it)
  {
    names.pushBack(it->first);
  }
  if (names.isEmpty())
  {
    outputStream << '\n';
    return;
  }
  helpers::sortStrings(names);
  for (size_t i = 0; i < names.getSize(); ++i)
  {
    outputStream << names[i] << '\n';
  }
}

void burukov::commandVertexes(std::istream &inputStream, std::ostream &outputStream, GraphDatabase &database)
{
  std::string graphName;
  inputStream >> graphName;
  if (!database.contains(graphName))
  {
    throw std::runtime_error("graph not found");
  }
  const Graph &graph = database.at(graphName);
  Vector< std::string > vertices;
  for (auto it = graph.vertices().cbegin(); it != graph.vertices().cend(); ++it)
  {
    vertices.pushBack(*it);
  }
  if (vertices.isEmpty())
  {
    outputStream << '\n';
    return;
  }
  helpers::sortStrings(vertices);
  for (size_t i = 0; i < vertices.getSize(); ++i)
  {
    outputStream << vertices[i] << '\n';
  }
}

void burukov::commandBind(std::istream &inputStream, std::ostream &, GraphDatabase &database)
{
  std::string graphName;
  std::string from;
  std::string to;
  size_t weight = 0;
  inputStream >> graphName >> from >> to >> weight;
  requireStream(inputStream);
  if (!database.contains(graphName))
  {
    throw std::runtime_error("graph not found");
  }
  database.at(graphName).addEdge(from, to, weight);
}

void burukov::commandCut(std::istream &inputStream, std::ostream &, GraphDatabase &database)
{
  std::string graphName;
  std::string from;
  std::string to;
  size_t weight = 0;
  inputStream >> graphName >> from >> to >> weight;
  requireStream(inputStream);
  if (!database.contains(graphName))
  {
    throw std::runtime_error("graph not found");
  }
  database.at(graphName).removeEdge(from, to, weight);
}

void burukov::commandCreate(std::istream &inputStream, std::ostream &, GraphDatabase &database)
{
  std::string graphName;
  inputStream >> graphName;
  if (database.contains(graphName))
  {
    throw std::runtime_error("graph already exists");
  }
  size_t vertexCount = 0;
  inputStream >> vertexCount;
  requireStream(inputStream);
  Graph newGraph;
  for (size_t i = 0; i < vertexCount; ++i)
  {
    std::string vertex;
    inputStream >> vertex;
    requireStream(inputStream);
    newGraph.addVertex(vertex);
  }
  database.add(graphName, std::move(newGraph));
}

void burukov::commandOutbound(std::istream &inputStream, std::ostream &outputStream, GraphDatabase &database)
{
  std::string graphName;
  std::string vertex;
  inputStream >> graphName >> vertex;
  if (!database.contains(graphName))
  {
    throw std::runtime_error("graph not found");
  }
  const Graph &graph = database.at(graphName);
  if (!graph.hasVertex(vertex))
  {
    throw std::runtime_error("vertex not found");
  }
  struct OutgoingInfo
  {
    std::string target;
    Vector< size_t > weights;
  };
  Vector< OutgoingInfo > outgoingList;
  for (auto it = graph.edges().cbegin(); it != graph.edges().cend(); ++it)
  {
    if (it->first.first != vertex)
    {
      continue;
    }
    bool alreadyExists = false;
    for (size_t i = 0; i < outgoingList.getSize(); ++i)
    {
      if (outgoingList[i].target == it->first.second)
      {
        for (auto weightIt = it->second.cbegin(); weightIt != it->second.cend(); ++weightIt)
        {
          outgoingList[i].weights.pushBack(*weightIt);
        }
        alreadyExists = true;
        break;
      }
    }
    if (!alreadyExists)
    {
      OutgoingInfo info;
      info.target = it->first.second;
      for (auto weightIt = it->second.cbegin(); weightIt != it->second.cend(); ++weightIt)
      {
        info.weights.pushBack(*weightIt);
      }
      outgoingList.pushBack(std::move(info));
    }
  }
  if (outgoingList.isEmpty())
  {
    outputStream << '\n';
    return;
  }
  Vector< std::string > targets;
  for (size_t i = 0; i < outgoingList.getSize(); ++i)
  {
    targets.pushBack(outgoingList[i].target);
  }
  helpers::sortStrings(targets);
  for (size_t i = 0; i < targets.getSize(); ++i)
  {
    outputStream << targets[i];
    for (size_t j = 0; j < outgoingList.getSize(); ++j)
    {
      if (outgoingList[j].target == targets[i])
      {
        helpers::sortWeights(outgoingList[j].weights);
        for (size_t w = 0; w < outgoingList[j].weights.getSize(); ++w)
        {
          outputStream << ' ' << outgoingList[j].weights[w];
        }
        break;
      }
    }
    outputStream << '\n';
  }
}

void burukov::commandInbound(std::istream &inputStream, std::ostream &outputStream, GraphDatabase &database)
{
  std::string graphName;
  std::string vertex;
  inputStream >> graphName >> vertex;
  if (!database.contains(graphName))
  {
    throw std::runtime_error("graph not found");
  }
  const Graph &graph = database.at(graphName);
  if (!graph.hasVertex(vertex))
  {
    throw std::runtime_error("vertex not found");
  }
  struct IncomingInfo
  {
    std::string source;
    Vector< size_t > weights;
  };
  Vector< IncomingInfo > incomingList;
  for (auto it = graph.edges().cbegin(); it != graph.edges().cend(); ++it)
  {
    if (it->first.second != vertex)
    {
      continue;
    }
    bool alreadyExists = false;
    for (size_t i = 0; i < incomingList.getSize(); ++i)
    {
      if (incomingList[i].source == it->first.first)
      {
        for (auto weightIt = it->second.cbegin(); weightIt != it->second.cend(); ++weightIt)
        {
          incomingList[i].weights.pushBack(*weightIt);
        }
        alreadyExists = true;
        break;
      }
    }
    if (!alreadyExists)
    {
      IncomingInfo info;
      info.source = it->first.first;
      for (auto weightIt = it->second.cbegin(); weightIt != it->second.cend(); ++weightIt)
      {
        info.weights.pushBack(*weightIt);
      }
      incomingList.pushBack(std::move(info));
    }
  }
  if (incomingList.isEmpty())
  {
    outputStream << '\n';
    return;
  }
  Vector< std::string > sources;
  for (size_t i = 0; i < incomingList.getSize(); ++i)
  {
    sources.pushBack(incomingList[i].source);
  }
  helpers::sortStrings(sources);
  for (size_t i = 0; i < sources.getSize(); ++i)
  {
    outputStream << sources[i];
    for (size_t j = 0; j < incomingList.getSize(); ++j)
    {
      if (incomingList[j].source == sources[i])
      {
        helpers::sortWeights(incomingList[j].weights);
        for (size_t w = 0; w < incomingList[j].weights.getSize(); ++w)
        {
          outputStream << ' ' << incomingList[j].weights[w];
        }
        break;
      }
    }
    outputStream << '\n';
  }
}

void burukov::commandMerge(std::istream &inputStream, std::ostream &, GraphDatabase &database)
{
  std::string newName;
  std::string graph1;
  std::string graph2;
  inputStream >> newName >> graph1 >> graph2;
  if (database.contains(newName) || !database.contains(graph1) || !database.contains(graph2))
  {
    throw std::runtime_error("invalid merge");
  }
  Graph merged;
  const Graph &source1 = database.at(graph1);
  const Graph &source2 = database.at(graph2);
  for (auto it = source1.vertices().cbegin(); it != source1.vertices().cend(); ++it)
  {
    merged.addVertex(*it);
  }
  for (auto it = source2.vertices().cbegin(); it != source2.vertices().cend(); ++it)
  {
    merged.addVertex(*it);
  }
  for (auto it = source1.edges().cbegin(); it != source1.edges().cend(); ++it)
  {
    for (auto weightIt = it->second.cbegin(); weightIt != it->second.cend(); ++weightIt)
    {
      merged.addEdge(it->first.first, it->first.second, *weightIt);
    }
  }
  for (auto it = source2.edges().cbegin(); it != source2.edges().cend(); ++it)
  {
    for (auto weightIt = it->second.cbegin(); weightIt != it->second.cend(); ++weightIt)
    {
      merged.addEdge(it->first.first, it->first.second, *weightIt);
    }
  }
  database.add(newName, std::move(merged));
}

void burukov::commandExtract(std::istream &inputStream, std::ostream &, GraphDatabase &database)
{
  std::string newName;
  std::string oldName;
  size_t keepCount = 0;
  inputStream >> newName >> oldName >> keepCount;
  requireStream(inputStream);
  if (database.contains(newName) || !database.contains(oldName))
  {
    throw std::runtime_error("invalid extract");
  }
  const Graph &source = database.at(oldName);
  List< std::string > verticesToKeep;
  for (size_t i = 0; i < keepCount; ++i)
  {
    std::string vertex;
    inputStream >> vertex;
    requireStream(inputStream);
    verticesToKeep.pushFront(vertex);
  }
  for (auto it = verticesToKeep.cbegin(); it != verticesToKeep.cend(); ++it)
  {
    if (!source.hasVertex(*it))
    {
      throw std::runtime_error("vertex not found");
    }
  }
  Graph extracted;
  for (auto it = verticesToKeep.cbegin(); it != verticesToKeep.cend(); ++it)
  {
    extracted.addVertex(*it);
  }
  for (auto it = source.edges().cbegin(); it != source.edges().cend(); ++it)
  {
    bool fromOk = false;
    bool toOk = false;
    for (auto keepIt = verticesToKeep.cbegin(); keepIt != verticesToKeep.cend(); ++keepIt)
    {
      if (*keepIt == it->first.first)
      {
        fromOk = true;
      }
      if (*keepIt == it->first.second)
      {
        toOk = true;
      }
    }
    if (fromOk && toOk)
    {
      for (auto weightIt = it->second.cbegin(); weightIt != it->second.cend(); ++weightIt)
      {
        extracted.addEdge(it->first.first, it->first.second, *weightIt);
      }
    }
  }
  database.add(newName, std::move(extracted));
}
