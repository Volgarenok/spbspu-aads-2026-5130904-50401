#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include "graph-storage.hpp"
#include "road-graph.hpp"

BOOST_AUTO_TEST_CASE(road_is_undirected)
{
  yalovsky::RoadGraph graph;
  graph.addCity("A");
  graph.addCity("B");
  graph.addRoad("A", "B", 10);

  BOOST_TEST(graph.getRoadWeight("A", "B") == 10);
  BOOST_TEST(graph.getRoadWeight("B", "A") == 10);
  BOOST_TEST(graph.getRoadCount() == 1);

  BOOST_CHECK_THROW(graph.addRoad("B", "A", 20), std::invalid_argument);
  BOOST_CHECK_THROW(graph.addRoad("A", "A", 5), std::invalid_argument);
  BOOST_CHECK_THROW(graph.addRoad("A", "B", 0), std::invalid_argument);
  BOOST_CHECK_THROW(graph.addRoad("A", "missing", 5), std::out_of_range);

  graph.removeRoad("B", "A");
  BOOST_TEST(!graph.hasRoad("A", "B"));
  BOOST_TEST(!graph.hasRoad("B", "A"));
}

BOOST_AUTO_TEST_CASE(removing_city_removes_incident_roads)
{
  yalovsky::RoadGraph graph;
  graph.addCity("A");
  graph.addCity("B");
  graph.addCity("C");
  graph.addRoad("A", "B", 10);
  graph.addRoad("B", "C", 20);

  graph.removeCity("B");

  BOOST_TEST(!graph.hasCity("B"));
  BOOST_TEST(graph.getRoadCount() == 0);
  BOOST_TEST(graph.getRoads("A").empty());
  BOOST_TEST(graph.getRoads("C").empty());
}

BOOST_AUTO_TEST_CASE(graphs_are_independent)
{
  yalovsky::GraphStorage storage;
  storage.makeGraph("first");
  storage.makeGraph("second");

  storage.getGraph("first").addCity("A");
  BOOST_TEST(!storage.getGraph("second").hasCity("A"));

  auto copy = storage;
  copy.getGraph("first").removeCity("A");
  BOOST_TEST(storage.getGraph("first").hasCity("A"));

  BOOST_CHECK_THROW(storage.makeGraph("first"), std::invalid_argument);
  storage.dropGraph("first");
  BOOST_CHECK_THROW(storage.getGraph("first"), std::out_of_range);
  BOOST_TEST(storage.getGraphs().size() == 1);
}
