#ifndef YALOVSKY_F0_COMMANDS_HPP
#define YALOVSKY_F0_COMMANDS_HPP

#include <iosfwd>

namespace yalovsky
{
  class GraphStorage;

  void runCommands(std::istream& input, std::ostream& output, GraphStorage& storage);

  namespace detail
  {
    void makeGraphCommand(std::istream& input, std::ostream& output, GraphStorage& storage);
    void dropGraphCommand(std::istream& input, std::ostream& output, GraphStorage& storage);
    void addCityCommand(std::istream& input, std::ostream& output, GraphStorage& storage);
    void removeCityCommand(std::istream& input, std::ostream& output, GraphStorage& storage);
    void addRoadCommand(std::istream& input, std::ostream& output, GraphStorage& storage);
    void removeRoadCommand(std::istream& input, std::ostream& output, GraphStorage& storage);
    void roadWeightCommand(std::istream& input, std::ostream& output, GraphStorage& storage);
    void showGraphsCommand(std::istream& input, std::ostream& output, GraphStorage& storage);
    void showCitiesCommand(std::istream& input, std::ostream& output, GraphStorage& storage);
    void showGraphCommand(std::istream& input, std::ostream& output, GraphStorage& storage);
    void findPathCommand(std::istream& input, std::ostream& output, GraphStorage& storage);
    void suggestRoadCommand(std::istream& input, std::ostream& output, GraphStorage& storage);
    void saveCommand(std::istream& input, std::ostream& output, GraphStorage& storage);
    void loadCommand(std::istream& input, std::ostream& output, GraphStorage& storage);
  }
}

#endif
