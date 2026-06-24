#include <boost/test/unit_test.hpp>
#include "graph.hpp"

using namespace tarasenko;

BOOST_AUTO_TEST_CASE(add_vertex)
{
  Graph graph;
  graph.addVertex("a");
  BOOST_TEST(graph.hasVertex("a"));
  BOOST_TEST(!graph.hasVertex("b"));
}

BOOST_AUTO_TEST_CASE(bind)
{
  Graph graph;
  graph.addVertex("a");
  graph.addVertex("b");
  graph.bind("a", "b", 1);
  Vector< EdgeInfo > outbound_a{{"b", Vector< unsigned int >{1}}};
  Vector< EdgeInfo > inbound_b{{"a", Vector< unsigned int >{1}}};
  BOOST_CHECK(graph.getOutbound("a") == outbound_a);
  BOOST_CHECK(graph.getInbound("b") == inbound_b);
  BOOST_TEST(graph.getOutbound("a").getSize() == 1);
  BOOST_TEST(graph.getInbound("b").getSize() == 1);
}

BOOST_AUTO_TEST_CASE(bind_multiple_edges)
{
  Graph graph;
  graph.addVertex("a");
  graph.addVertex("b");
  graph.bind("a", "b", 2);
  graph.bind("a", "b", 1);
  Vector< EdgeInfo > outbound_a{{"b", Vector< unsigned int >{1, 2}}};
  Vector< EdgeInfo > inbound_b{{"a", Vector< unsigned int >{1, 2}}};
  BOOST_CHECK(graph.getOutbound("a") == outbound_a);
  BOOST_CHECK(graph.getInbound("b") == inbound_b);
  BOOST_TEST(graph.getOutbound("a").getSize() == 1);
  BOOST_TEST(graph.getInbound("b").getSize() == 1);
  BOOST_TEST(graph.getOutbound("a")[0].weights.getSize() == 2);
  BOOST_TEST(graph.getInbound("b")[0].weights.getSize() == 2);
}

BOOST_AUTO_TEST_CASE(cut)
{
  Graph graph;
  graph.addVertex("a");
  graph.addVertex("b");
  graph.bind("a", "b", 1);
  BOOST_CHECK_THROW(graph.cut("a", "c", 1), std::runtime_error);
  BOOST_CHECK_THROW(graph.cut("a", "b", 2), std::runtime_error);
  BOOST_CHECK_NO_THROW(graph.cut("a", "b", 1));
  BOOST_CHECK_THROW(graph.cut("a", "b", 1), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(get_edges_without_vertex)
{
  Graph graph;
  graph.addVertex("a");
  BOOST_CHECK_THROW(graph.getOutbound("b"), std::runtime_error);
  BOOST_CHECK_THROW(graph.getInbound("b"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(get_vertexes)
{
  Graph graph;
  graph.addVertex("a");
  graph.addVertex("b");
  Vector< std::string > vertexes{"a", "b"};
  BOOST_CHECK(graph.getVertexes() == vertexes);
}

BOOST_AUTO_TEST_CASE(extract)
{
  Graph graph;
  graph.addVertex("a");
  graph.addVertex("b");
  graph.addVertex("c");
  graph.bind("a", "b", 1);
  graph.bind("b", "c", 1);
  Graph extracted = graph.extract(Vector< std::string >{"a", "b"});
  Vector< EdgeInfo > outbound_a{{"b", Vector< unsigned int >{1}}};
  Vector< EdgeInfo > inbound_b{{"a", Vector< unsigned int >{1}}};
  BOOST_CHECK(extracted.getOutbound("a") == outbound_a);
  BOOST_CHECK(extracted.getInbound("b") == inbound_b);
  BOOST_CHECK(extracted.getOutbound("b").getSize() == 0);
  BOOST_CHECK_THROW(graph.extract(Vector< std::string >{"a", "d"}), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(merge_graphs)
{
  Graph graph1;
  graph1.addVertex("a");
  graph1.addVertex("b");
  graph1.bind("a", "b", 1);
  Graph graph2;
  graph2.addVertex("a");
  graph2.addVertex("b");
  graph2.bind("a", "b", 1);
  Graph merged = merge(graph1, graph2);
  Vector< EdgeInfo > outbound_a{{"b", Vector< unsigned int >{1, 1}}};
  Vector< EdgeInfo > inbound_b{{"a", Vector< unsigned int >{1, 1}}};
  BOOST_CHECK(merged.getOutbound("a") == outbound_a);
  BOOST_CHECK(merged.getInbound("b") == inbound_b);
  BOOST_TEST(merged.getOutbound("a").getSize() == 1);
  BOOST_TEST(merged.getInbound("b").getSize() == 1);
  BOOST_TEST(merged.getOutbound("a")[0].weights.getSize() == 2);
  BOOST_TEST(merged.getInbound("b")[0].weights.getSize() == 2);
}
