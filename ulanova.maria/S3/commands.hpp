#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <map>
#include <string>
#include <vector>

#include "graph_storage.hpp"

namespace ulanova
{
  class CommandProcessor
  {
  public:
    explicit CommandProcessor(GraphStorage& storage);

    void run(std::istream& input, std::ostream& output);

  private:
    using Args = std::vector<std::string>;
    using Handler = void (CommandProcessor::*)(const Args&, std::ostream&);

    GraphStorage& storage_;
    std::map<std::string, Handler> handlers_;

    void handleGraphs(const Args& args, std::ostream& output);
    void handleVertices(const Args& args, std::ostream& output);
    void handleOutbound(const Args& args, std::ostream& output);
    void handleInbound(const Args& args, std::ostream& output);
    void handleBind(const Args& args, std::ostream& output);
    void handleCut(const Args& args, std::ostream& output);
    void handleCreate(const Args& args, std::ostream& output);
    void handleMerge(const Args& args, std::ostream& output);
    void handleExtract(const Args& args, std::ostream& output);

    static Args split(const std::string& line);
    static bool parseUnsigned(const std::string& text, unsigned& value);
    static bool parseSize(const std::string& text, size_t& value);

    static void sortStrings(Vector<std::string>& values);
    static void sortEdges(Vector<Edge>& edges);
    static void printEdges(const Vector<Edge>& edges, std::ostream& output);
    static void printInvalid(std::ostream& output);
  };
}

#endif
