#include <boost/test/unit_test.hpp>
#include "Graph.hpp"

using namespace petrov;

BOOST_AUTO_TEST_CASE(test_add_and_check_vertex)
{
  Graph graph;
  BOOST_TEST(graph.getVertexCount() == 0);

  graph.addVertex(10);
  BOOST_TEST(graph.hasVertex(10));
  BOOST_TEST(!graph.hasVertex(20));
  BOOST_TEST(graph.getVertexCount() == 1);
}

BOOST_AUTO_TEST_CASE(test_add_edge_and_getters)
{
  Graph graph;
  graph.addWeightedEdge(1, 2, 5);

  BOOST_TEST(graph.hasVertex(1));
  BOOST_TEST(graph.hasVertex(2));
  BOOST_TEST(graph.getVertexCount() == 2);

  List<int> outbound_1 = graph.getOutbound(1);
  List<int> inbound_2 = graph.getInbound(2);

  BOOST_TEST(outbound_1.getSize() == 1);
  BOOST_TEST(inbound_2.getSize() == 1);
  BOOST_TEST(*outbound_1.begin() == 2);
  BOOST_TEST(*inbound_2.begin() == 1);
}

BOOST_AUTO_TEST_CASE(test_add_unweighted_edge)
{
  Graph graph;
  graph.addEdge(5, 6);

  List<int> outbound = graph.getOutbound(5);
  BOOST_TEST(outbound.getSize() == 1);
  BOOST_TEST(*outbound.begin() == 6);
}

BOOST_AUTO_TEST_CASE(test_cut_weighted_edge)
{
  Graph graph;
  graph.addWeightedEdge(1, 2, 100);

  graph.cut(1, 2, 50);
  BOOST_TEST(graph.getOutbound(1).getSize() == 1);

  graph.cut(1, 2, 100);
  BOOST_TEST(graph.getOutbound(1).getSize() == 0);
  BOOST_TEST(graph.getInbound(2).getSize() == 0);
}

BOOST_AUTO_TEST_CASE(test_remove_vertex)
{
  Graph graph;
  graph.addWeightedEdge(1, 2, 10);
  graph.addWeightedEdge(2, 3, 20);

  graph.removeVertex(2);

  BOOST_TEST(!graph.hasVertex(2));
  BOOST_TEST(graph.hasVertex(1));
  BOOST_TEST(graph.hasVertex(3));
  BOOST_TEST(graph.getVertexCount() == 2);

  BOOST_TEST(graph.getOutbound(1).getSize() == 0);
  BOOST_TEST(graph.getInbound(3).getSize() == 0);
}

BOOST_AUTO_TEST_CASE(test_get_all_vertices)
{
  Graph graph;
  graph.addVertex(7);
  graph.addVertex(8);

  List<int> all = graph.getAllVertices();
  BOOST_TEST(all.getSize() == 2);
}

BOOST_AUTO_TEST_CASE(test_graph_copy_and_assignment)
{
  Graph graph1;
  graph1.addVertex(42);

  Graph graph2(graph1);
  BOOST_TEST(graph2.hasVertex(42));

  Graph graph3;
  graph3 = graph2;
  BOOST_TEST(graph3.hasVertex(42));
}

BOOST_AUTO_TEST_CASE(test_swap_graphs)
{
  Graph graph1;
  graph1.addVertex(1);

  Graph graph2;
  graph2.addVertex(2);
  graph2.addVertex(3);

  graph1.swap(graph2);

  BOOST_TEST(graph1.getVertexCount() == 2);
  BOOST_TEST(graph1.hasVertex(2));
  BOOST_TEST(graph2.getVertexCount() == 1);
  BOOST_TEST(graph2.hasVertex(1));
}

BOOST_AUTO_TEST_CASE(test_merge_graphs)
{
  Graph graph1;
  graph1.addWeightedEdge(1, 2, 10);

  Graph graph2;
  graph2.addWeightedEdge(2, 3, 20);
  graph2.addWeightedEdge(1, 2, 10);

  Graph merged = graph1.merge(graph2);

  BOOST_TEST(merged.hasVertex(1));
  BOOST_TEST(merged.hasVertex(2));
  BOOST_TEST(merged.hasVertex(3));
  BOOST_TEST(merged.getVertexCount() == 3);
}
