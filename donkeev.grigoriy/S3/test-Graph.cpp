#include "Hash-table.hpp"
#include "hash-structs.hpp"
#include "Graph.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(Graph_tests)

BOOST_AUTO_TEST_CASE(graphSizeConstructor)
{
  donkeev::Graph graph(16, 4);
  BOOST_CHECK(graph.table_.size() == 0);

  graph.addEdge("first", "second", 1);
  BOOST_CHECK(graph.table_.size() == 1);
}

BOOST_AUTO_TEST_CASE(graphCopyConstructor)
{
  donkeev::Graph graph(16, 4);
  graph.addEdge("first", "second", 1);

  donkeev::Graph copyGraph(graph);
  BOOST_CHECK(copyGraph.table_.size() == 1);
}

BOOST_AUTO_TEST_CASE(graphMoveConstructor)
{
  donkeev::Graph graph(16, 4);
  graph.addEdge("first", "second", 1);

  donkeev::Graph copyGraph(std::move(graph));
  BOOST_CHECK(copyGraph.table_.size() == 1);
  BOOST_CHECK(graph.table_.size() == 0);
}

BOOST_AUTO_TEST_CASE(graphCopyOperator)
{
  donkeev::Graph graph(16, 4);
  graph.addEdge("first", "second", 1);

  donkeev::Graph copyGraph;
  copyGraph = graph;
  BOOST_CHECK(copyGraph.table_.size() == 1);
}

BOOST_AUTO_TEST_CASE(graphMoveOperator)
{
  donkeev::Graph graph(16, 4);
  graph.addEdge("first", "second", 1);

  donkeev::Graph copyGraph;
  copyGraph = std::move(graph);
  BOOST_CHECK(copyGraph.table_.size() == 1);
  BOOST_CHECK(graph.table_.size() == 0);
}

BOOST_AUTO_TEST_CASE(addVertexMethod)
{
  donkeev::Graph graph(16, 4);
  graph.addVertex("first");
  BOOST_CHECK(graph.table_.size() == 0);
  BOOST_CHECK(graph.uniqueVertexes_.getSize() == 1);
}

BOOST_AUTO_TEST_CASE(deleteEdgeMethod)
{
  donkeev::Graph graph(16, 4);
  graph.addEdge("first", "second", 1);
  BOOST_CHECK(graph.table_.size() == 1);

  graph.deleteEdge("first", "second", 1);
  BOOST_CHECK(graph.table_.size() == 0);
}

BOOST_AUTO_TEST_SUITE_END()
