#include "graph.hpp"
#include <boost/test/unit_test.hpp>


#include <stdexcept>
#include <string>

BOOST_AUTO_TEST_CASE(test_graph_create_graph_and_vertex)
{
  ulanova::Graph graph("gr1");

  BOOST_TEST_CHECK(graph.getName() == "gr1");
  BOOST_TEST_CHECK(!graph.hasVertex("a"));

  graph.addVertex("a");

  BOOST_TEST_CHECK(graph.hasVertex("a"));
}

BOOST_AUTO_TEST_CASE(test_graph_add_edge)
{
  ulanova::Graph graph("gr1");

  graph.addEdge("a", "b", 40);

  BOOST_TEST_CHECK(graph.hasVertex("a"));
  BOOST_TEST_CHECK(graph.hasVertex("b"));
  BOOST_TEST_CHECK(graph.hasEdge("a", "b", 40));
  BOOST_TEST_CHECK(!graph.hasEdge("b", "a", 40));

  ulanova::Vector<ulanova::Edge> outbound = graph.getOutbound("a");

  BOOST_TEST_CHECK(outbound.getsize() == 1);
  BOOST_TEST_CHECK(outbound[0].vertex == "b");
  BOOST_TEST_CHECK(outbound[0].weight == 40);
}

BOOST_AUTO_TEST_CASE(test_graph_remove_edge)
{
  ulanova::Graph graph("gr1");

  graph.addEdge("a", "b", 40);
  graph.addEdge("a", "c", 10);

  BOOST_TEST_CHECK(graph.removeEdge("a", "b", 40));
  BOOST_TEST_CHECK(!graph.hasEdge("a", "b", 40));
  BOOST_TEST_CHECK(graph.hasEdge("a", "c", 10));
}

BOOST_AUTO_TEST_CASE(test_graph_merge)
{
  ulanova::Graph lhs("lhs");
  lhs.addEdge("a", "b", 1);

  ulanova::Graph rhs("rhs");
  rhs.addEdge("b", "c", 2);

  ulanova::Graph merged = ulanova::Graph::merge("merged", lhs, rhs);

  BOOST_TEST_CHECK(merged.hasVertex("a"));
  BOOST_TEST_CHECK(merged.hasVertex("b"));
  BOOST_TEST_CHECK(merged.hasVertex("c"));
  BOOST_TEST_CHECK(merged.hasEdge("a", "b", 1));
  BOOST_TEST_CHECK(merged.hasEdge("b", "c", 2));
}

BOOST_AUTO_TEST_CASE(test_graph_extract)
{
  ulanova::Graph graph("gr1");

  graph.addEdge("a", "b", 1);
  graph.addEdge("b", "c", 2);
  graph.addEdge("a", "c", 3);

  ulanova::Vector<std::string> vertices;
  vertices.push_back("a");
  vertices.push_back("b");

  ulanova::Graph extracted = graph.extract("sub", vertices);

  BOOST_TEST_CHECK(extracted.hasVertex("a"));
  BOOST_TEST_CHECK(extracted.hasVertex("b"));
  BOOST_TEST_CHECK(!extracted.hasVertex("c"));
  BOOST_TEST_CHECK(extracted.hasEdge("a", "b", 1));
  BOOST_TEST_CHECK(!extracted.hasEdge("b", "c", 2));
}
