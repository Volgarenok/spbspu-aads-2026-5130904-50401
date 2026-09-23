#include <boost/test/unit_test.hpp>
#include <limits>
#include <stdexcept>
#include "algorithms.hpp"
#include "road-graph.hpp"

BOOST_AUTO_TEST_CASE(shortest_path_can_use_indirect_route)
{
  yalovsky::RoadGraph graph;
  graph.addCity("A");
  graph.addCity("B");
  graph.addCity("C");
  graph.addCity("D");
  graph.addRoad("A", "B", 10);
  graph.addRoad("B", "C", 20);
  graph.addRoad("A", "C", 50);

  const auto path = yalovsky::findShortestPath(graph, "A", "C");
  BOOST_TEST(path.distance == 30);
  BOOST_TEST(path.size == 3);
  BOOST_TEST(path.cities[0] == "A");
  BOOST_TEST(path.cities[1] == "B");
  BOOST_TEST(path.cities[2] == "C");

  const auto reverse = yalovsky::findShortestPath(graph, "C", "A");
  BOOST_TEST(reverse.distance == 30);
  BOOST_TEST(reverse.cities[0] == "C");

  const auto same = yalovsky::findShortestPath(graph, "A", "A");
  BOOST_TEST(same.distance == 0);
  BOOST_TEST(same.size == 1);

  BOOST_CHECK_THROW(yalovsky::findShortestPath(graph, "A", "D"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(suggestion_chooses_largest_improvement)
{
  yalovsky::RoadGraph graph;
  graph.addCity("A");
  graph.addCity("B");
  graph.addCity("C");
  graph.addCity("D");
  graph.addRoad("A", "B", 10);
  graph.addRoad("B", "C", 20);

  const auto suggestion = yalovsky::suggestRoad(graph, 5);
  BOOST_TEST(suggestion.from == "A");
  BOOST_TEST(suggestion.to == "C");
  BOOST_TEST(suggestion.improvement == 25);
  BOOST_TEST(!graph.hasRoad("A", "C"));

  BOOST_CHECK_THROW(yalovsky::suggestRoad(graph, 30), std::logic_error);
}

BOOST_AUTO_TEST_CASE(path_overflow_is_distinguished_from_disconnection)
{
  yalovsky::RoadGraph graph;
  graph.addCity("A");
  graph.addCity("B");
  graph.addCity("C");

  const auto maximum = std::numeric_limits< yalovsky::Distance >::max();
  graph.addRoad("A", "B", maximum);
  graph.addRoad("B", "C", 1);

  const auto direct = yalovsky::findShortestPath(graph, "A", "B");
  BOOST_TEST(direct.distance == maximum);

  BOOST_CHECK_THROW(yalovsky::findShortestPath(graph, "A", "C"), std::overflow_error);

  graph.addRoad("A", "C", 5);
  const auto alternative = yalovsky::findShortestPath(graph, "A", "B");
  BOOST_TEST(alternative.distance == 6);
}
