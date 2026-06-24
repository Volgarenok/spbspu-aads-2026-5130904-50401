#include <boost/test/unit_test.hpp>
#include "graph.hpp"

BOOST_AUTO_TEST_SUITE(GraphTests)

BOOST_AUTO_TEST_CASE(basicCoLendCreatesEdge)
{
  burukov::RecommendationGraph graph;
  graph.addCoLend("A", "B");
  graph.calculateScores();
  auto recs = graph.getRecommendations("A", 5);
  BOOST_CHECK(!recs.empty());
  BOOST_CHECK_EQUAL(recs.front().first, "B");
}

BOOST_AUTO_TEST_CASE(undirectedEdgesBothWays)
{
  burukov::RecommendationGraph graph;
  graph.addCoLend("A", "B");
  graph.calculateScores();
  auto recsA = graph.getRecommendations("A", 5);
  auto recsB = graph.getRecommendations("B", 5);
  BOOST_CHECK(!recsA.empty());
  BOOST_CHECK(!recsB.empty());
}

BOOST_AUTO_TEST_CASE(selfLoopIgnored)
{
  burukov::RecommendationGraph graph;
  graph.addCoLend("A", "A");
  graph.calculateScores();
  auto recs = graph.getRecommendations("A", 5);
  BOOST_CHECK(recs.empty());
}

BOOST_AUTO_TEST_CASE(transitiveConnections)
{
  burukov::RecommendationGraph graph;
  graph.addCoLend("A", "B", 2.0);
  graph.addCoLend("B", "C", 2.0);
  graph.calculateScores();
  auto recs = graph.getRecommendations("A", 5);
  bool hasC = false;
  for (auto it = recs.cbegin(); it != recs.cend(); ++it)
  {
    if (it->first == "C")
    {
      hasC = true;
      break;
    }
  }
  BOOST_CHECK(hasC);
}

BOOST_AUTO_TEST_CASE(higherFrequencyGetsHigherScore)
{
  burukov::RecommendationGraph graph;
  graph.addCoLend("A", "B", 5.0);
  graph.addCoLend("A", "C", 1.0);
  graph.calculateScores();
  auto recs = graph.getRecommendations("A", 5);
  BOOST_CHECK_GE(recs.size(), 2);
  BOOST_CHECK_EQUAL(recs.front().first, "B");
}

BOOST_AUTO_TEST_CASE(emptyGraphReturnsEmpty)
{
  burukov::RecommendationGraph graph;
  auto recs = graph.getRecommendations("X", 5);
  BOOST_CHECK(recs.empty());
}

BOOST_AUTO_TEST_CASE(limitKReturnsExactCount)
{
  burukov::RecommendationGraph graph;
  graph.addCoLend("A", "B");
  graph.addCoLend("A", "C");
  graph.addCoLend("A", "D");
  graph.calculateScores();
  auto recs = graph.getRecommendations("A", 2);
  BOOST_CHECK_EQUAL(recs.size(), 2);
}

BOOST_AUTO_TEST_SUITE_END()
