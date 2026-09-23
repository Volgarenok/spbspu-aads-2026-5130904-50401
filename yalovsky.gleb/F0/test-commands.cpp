#include <boost/test/unit_test.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include "commands.hpp"
#include "file-io.hpp"
#include "graph-storage.hpp"
#include "input.hpp"

BOOST_AUTO_TEST_CASE(commands_reject_invalid_lines_without_mutation)
{
  yalovsky::GraphStorage storage;
  std::istringstream input(
    "make-graph rejected extra\n"
    "make-graph demo\n"
    "add-city demo A\n"
    "add-city demo B\n"
    "add-road demo A B -1\n"
    "add-road demo A B 10abc\n"
    "add-road demo A B 10 extra\n"
    "add-road demo A\n"
    "add-road demo A B 10\n"
    "road-weight demo B A\n"
    "show-graphs\n"
    "exit\n"
    "make-graph after-exit\n");
  std::ostringstream output;

  yalovsky::runCommands(input, output, storage);

  const std::string expected =
    "<INVALID COMMAND>\n"
    "Graph created\n"
    "City added\n"
    "City added\n"
    "<INVALID COMMAND>\n"
    "<INVALID COMMAND>\n"
    "<INVALID COMMAND>\n"
    "<INVALID COMMAND>\n"
    "Road added\n"
    "Distance: 10\n"
    "demo\n";

  BOOST_TEST(output.str() == expected);
  BOOST_TEST(storage.getGraphs().size() == 1);
  BOOST_TEST(storage.getGraph("demo").getRoadCount() == 1);
}

BOOST_AUTO_TEST_CASE(storage_round_trip_preserves_all_graphs)
{
  yalovsky::GraphStorage source;
  source.makeGraph("empty");
  source.makeGraph("roads");
  source.getGraph("roads").addCity("A");
  source.getGraph("roads").addCity("B");
  source.getGraph("roads").addCity("isolated");
  source.getGraph("roads").addRoad("A", "B", 12);

  std::ostringstream saved;
  yalovsky::writeStorage(saved, source);

  yalovsky::GraphStorage restored;
  restored.makeGraph("old");
  std::istringstream input(saved.str());
  yalovsky::readStorage(input, restored);

  BOOST_TEST(restored.getGraphs().size() == 2);
  BOOST_TEST(restored.getGraph("empty").getCities().empty());
  BOOST_TEST(restored.getGraph("roads").hasCity("isolated"));
  BOOST_TEST(restored.getGraph("roads").getRoadWeight("B", "A") == 12);
  BOOST_CHECK_THROW(restored.getGraph("old"), std::out_of_range);

  std::ostringstream savedAgain;
  yalovsky::writeStorage(savedAgain, restored);
  BOOST_TEST(savedAgain.str() == saved.str());
}

BOOST_AUTO_TEST_CASE(invalid_file_preserves_current_storage)
{
  yalovsky::GraphStorage storage;
  storage.makeGraph("keep");
  storage.getGraph("keep").addCity("safe");

  const std::string invalidFiles[] = {
    "",
    "NAVIGATOR 2 0 END",
    "NAVIGATOR 1 1 GRAPH broken 1 0 CITY A",
    "NAVIGATOR 1 1 GRAPH broken 2 0 CITY A CITY A ENDGRAPH END",
    "NAVIGATOR 1 1 GRAPH broken 1 1 CITY A ROAD A B 5 ENDGRAPH END",
    "NAVIGATOR 1 0 END extra"
  };

  for (const std::string& text: invalidFiles)
  {
    std::istringstream input(text);
    BOOST_CHECK_THROW(yalovsky::readStorage(input, storage), std::exception);
    BOOST_TEST(storage.getGraphs().size() == 1);
    BOOST_TEST(storage.getGraph("keep").hasCity("safe"));
  }
}

BOOST_AUTO_TEST_CASE(unsigned_numbers_are_parsed_strictly)
{
  const std::string invalidNumbers[] = {
    "-1", "+1", "1.5", "10abc", "184467440737095516160"
  };

  for (const std::string& text: invalidNumbers)
  {
    std::istringstream input(text);
    BOOST_CHECK_THROW(yalovsky::readUnsigned(input), std::exception);
  }

  std::istringstream valid("700");
  BOOST_TEST(yalovsky::readUnsigned(valid) == 700);
}
