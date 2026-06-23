#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <string>

#include "graph_storage.hpp"
#include "hash_table.hpp"

namespace ulanova
{
  void sortStrings(Vector< std::string >& values);
  void sortEdges(Vector< Edge >& edges);
  void printEdges(const Vector< Edge >& edges, std::ostream& output);
  void printInvalid(std::ostream& output);

  class CommandProcessor
  {
  public:
    explicit CommandProcessor(GraphStorage& storage);

    void run(std::istream& input, std::ostream& output);

  private:
    using Handler = void (CommandProcessor::*)(std::istream&, std::ostream&);

    GraphStorage& storage_;
    HashTable< std::string, Handler, StringHash, StringEqual > handlers_;

    void handleGraphs(std::istream& input, std::ostream& output);
    void handleVertices(std::istream& input, std::ostream& output);
    void handleOutbound(std::istream& input, std::ostream& output);
    void handleInbound(std::istream& input, std::ostream& output);
    void handleBind(std::istream& input, std::ostream& output);
    void handleCut(std::istream& input, std::ostream& output);
    void handleCreate(std::istream& input, std::ostream& output);
    void handleMerge(std::istream& input, std::ostream& output);
    void handleExtract(std::istream& input, std::ostream& output);
  };
}

#endif
