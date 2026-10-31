#include "Graph.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <map>
#include <random>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace khasnulin;

namespace
{
  using StrGraph = Graph< std::string, std::string, int >;
  using IntGraph = Graph< int, int, int >;
  using Reference = std::map< std::pair< int, int >, int >;

  std::vector< std::pair< std::string, int > > collectUser(const StrGraph &g, const std::string &user)
  {
    std::vector< std::pair< std::string, int > > out;
    g.traverseUserRatings(user, [&out](const std::string &a, const int &w) { out.emplace_back(a, w); });
    return out;
  }

  std::vector< std::pair< std::string, int > > collectArticle(const StrGraph &g, const std::string &article)
  {
    std::vector< std::pair< std::string, int > > out;
    g.traverseArticleRatings(article, [&out](const std::string &u, const int &w) { out.emplace_back(u, w); });
    return out;
  }

  void checkInvariants(const IntGraph &g, const Reference &ref)
  {
    BOOST_REQUIRE_EQUAL(g.ratingsCount(), ref.size());
    BOOST_REQUIRE_EQUAL(g.empty(), ref.empty());

    Reference all;
    g.traverseAllRatings([&all](const int &u, const int &a, const int &w)
                         { BOOST_REQUIRE(all.emplace(std::make_pair(u, a), w).second); });
    BOOST_REQUIRE(all == ref);

    std::map< int, std::vector< std::pair< int, int > > > byUser;
    std::map< int, std::vector< std::pair< int, int > > > byArticle;
    for (const auto &item : ref)
    {
      byUser[item.first.first].emplace_back(item.first.second, item.second);
      byArticle[item.first.second].emplace_back(item.first.first, item.second);
    }

    for (auto &item : byUser)
    {
      std::vector< std::pair< int, int > > got;
      g.traverseUserRatings(item.first, [&got](const int &a, const int &w) { got.emplace_back(a, w); });
      BOOST_REQUIRE(got == item.second);
      BOOST_REQUIRE_EQUAL(g.userRatingsCount(item.first), item.second.size());
    }
    for (auto &item : byArticle)
    {
      std::vector< std::pair< int, int > > got;
      g.traverseArticleRatings(item.first, [&got](const int &u, const int &w) { got.emplace_back(u, w); });
      BOOST_REQUIRE(got == item.second);
      BOOST_REQUIRE_EQUAL(g.articleRatingsCount(item.first), item.second.size());
    }
  }

  struct CountingFunctor
  {
    size_t calls = 0;
    void operator()(const std::string &s, const int &c)
    {
      ++calls;
    }
  };
}

BOOST_AUTO_TEST_SUITE(graph_test_suite)

BOOST_AUTO_TEST_CASE(test_empty_creation)
{
  StrGraph g;

  BOOST_REQUIRE(g.empty());
  BOOST_CHECK_EQUAL(g.ratingsCount(), 0);
  BOOST_CHECK_EQUAL(g.userRatingsCount("roman"), 0);
  BOOST_CHECK_EQUAL(g.articleRatingsCount("cpp"), 0);
  BOOST_CHECK(g.hasRating("roman", "cpp") == false);
  BOOST_CHECK(g.findRating("roman", "cpp") == nullptr);
}

BOOST_AUTO_TEST_CASE(test_rate_new_rating_positive)
{
  StrGraph g;

  BOOST_CHECK(g.rate("roman", "cpp", 9) == true);
  BOOST_CHECK(g.rate("roman", "go", 7) == true);
  BOOST_CHECK(g.rate("alex", "cpp", 5) == true);

  BOOST_REQUIRE(g.empty() == false);
  BOOST_CHECK_EQUAL(g.ratingsCount(), 3);
  BOOST_CHECK_EQUAL(g.userRatingsCount("roman"), 2);
  BOOST_CHECK_EQUAL(g.userRatingsCount("alex"), 1);
  BOOST_CHECK_EQUAL(g.articleRatingsCount("cpp"), 2);
  BOOST_CHECK_EQUAL(g.articleRatingsCount("go"), 1);

  BOOST_CHECK(g.hasRating("roman", "cpp"));
  BOOST_CHECK(g.hasRating("alex", "go") == false);

  const int *found = g.findRating("alex", "cpp");
  BOOST_REQUIRE(found != nullptr);
  BOOST_CHECK_EQUAL(*found, 5);
}

BOOST_AUTO_TEST_CASE(test_rate_replace_existing_rating)
{
  StrGraph g;
  g.rate("roman", "cpp", 9);
  g.rate("alex", "cpp", 5);

  BOOST_CHECK(g.rate("roman", "cpp", 3) == false);

  BOOST_CHECK_EQUAL(g.ratingsCount(), 2);
  BOOST_CHECK_EQUAL(g.userRatingsCount("roman"), 1);
  BOOST_CHECK_EQUAL(g.articleRatingsCount("cpp"), 2);
  BOOST_CHECK_EQUAL(*g.findRating("roman", "cpp"), 3);

  auto byArticle = collectArticle(g, "cpp");
  std::vector< std::pair< std::string, int > > expected{{"alex", 5}, {"roman", 3}};
  BOOST_CHECK(byArticle == expected);
}

BOOST_AUTO_TEST_CASE(test_missing_vertices_negative)
{
  StrGraph g;
  g.rate("roman", "cpp", 9);

  BOOST_CHECK(g.findRating("roman", "go") == nullptr);
  BOOST_CHECK(g.findRating("alex", "cpp") == nullptr);
  BOOST_CHECK(g.findRating("alex", "go") == nullptr);
  BOOST_CHECK(g.hasRating("alex", "cpp") == false);
  BOOST_CHECK_EQUAL(g.userRatingsCount("alex"), 0);
  BOOST_CHECK_EQUAL(g.articleRatingsCount("go"), 0);

  BOOST_CHECK(g.eraseRating("roman", "go") == false);
  BOOST_CHECK(g.eraseRating("alex", "cpp") == false);
  BOOST_CHECK_EQUAL(g.eraseUser("alex"), 0);
  BOOST_CHECK_EQUAL(g.eraseArticle("go"), 0);

  BOOST_CHECK_EQUAL(g.ratingsCount(), 1);
}

BOOST_AUTO_TEST_CASE(test_erase_rating_removes_empty_vertices)
{
  StrGraph g;
  g.rate("roman", "cpp", 9);
  g.rate("roman", "go", 7);

  BOOST_CHECK(g.eraseRating("roman", "cpp") == true);

  BOOST_CHECK_EQUAL(g.ratingsCount(), 1);
  BOOST_CHECK(g.hasRating("roman", "cpp") == false);
  BOOST_CHECK_EQUAL(g.userRatingsCount("roman"), 1);
  BOOST_CHECK_EQUAL(g.articleRatingsCount("cpp"), 0);
  BOOST_CHECK(collectArticle(g, "cpp").empty());

  BOOST_CHECK(g.eraseRating("roman", "go") == true);
  BOOST_CHECK(g.eraseRating("roman", "go") == false);

  BOOST_CHECK(g.empty());
  BOOST_CHECK_EQUAL(g.userRatingsCount("roman"), 0);
  BOOST_CHECK(collectUser(g, "roman").empty());
}

BOOST_AUTO_TEST_CASE(test_erase_user_cascade)
{
  StrGraph g;
  g.rate("roman", "cpp", 9);
  g.rate("roman", "go", 7);
  g.rate("roman", "rust", 8);
  g.rate("alex", "cpp", 5);

  BOOST_CHECK_EQUAL(g.eraseUser("roman"), 3);

  BOOST_CHECK_EQUAL(g.ratingsCount(), 1);
  BOOST_CHECK_EQUAL(g.userRatingsCount("roman"), 0);
  BOOST_CHECK(g.hasRating("roman", "cpp") == false);

  BOOST_CHECK_EQUAL(g.articleRatingsCount("cpp"), 1);
  BOOST_CHECK_EQUAL(g.articleRatingsCount("go"), 0);
  BOOST_CHECK_EQUAL(g.articleRatingsCount("rust"), 0);
  std::vector< std::pair< std::string, int > > expected{{"alex", 5}};
  BOOST_CHECK(collectArticle(g, "cpp") == expected);

  BOOST_CHECK_EQUAL(g.eraseUser("roman"), 0);
}

BOOST_AUTO_TEST_CASE(test_erase_article_cascade)
{
  StrGraph g;
  g.rate("roman", "cpp", 9);
  g.rate("alex", "cpp", 5);
  g.rate("ivan", "cpp", 6);
  g.rate("roman", "go", 7);

  BOOST_CHECK_EQUAL(g.eraseArticle("cpp"), 3);

  BOOST_CHECK_EQUAL(g.ratingsCount(), 1);
  BOOST_CHECK_EQUAL(g.articleRatingsCount("cpp"), 0);
  BOOST_CHECK_EQUAL(g.userRatingsCount("roman"), 1);
  BOOST_CHECK_EQUAL(g.userRatingsCount("alex"), 0);
  BOOST_CHECK_EQUAL(g.userRatingsCount("ivan"), 0);
  BOOST_CHECK(g.hasRating("roman", "go"));

  BOOST_CHECK_EQUAL(g.eraseArticle("cpp"), 0);
}

BOOST_AUTO_TEST_CASE(test_traverse_user_ratings_sorted)
{
  StrGraph g;
  g.rate("roman", "rust", 8);
  g.rate("roman", "cpp", 9);
  g.rate("roman", "go", 7);
  g.rate("roman", "assembler", 4);
  g.rate("alex", "java", 1);

  std::vector< std::pair< std::string, int > > expected{{"assembler", 4}, {"cpp", 9}, {"go", 7}, {"rust", 8}};
  BOOST_CHECK(collectUser(g, "roman") == expected);
}

BOOST_AUTO_TEST_CASE(test_traverse_article_ratings_sorted)
{
  StrGraph g;
  g.rate("roman", "cpp", 9);
  g.rate("alex", "cpp", 5);
  g.rate("ivan", "cpp", 6);
  g.rate("ivan", "go", 2);

  std::vector< std::pair< std::string, int > > expected{{"alex", 5}, {"ivan", 6}, {"roman", 9}};
  BOOST_CHECK(collectArticle(g, "cpp") == expected);
}

BOOST_AUTO_TEST_CASE(test_traverse_returns_functor)
{
  StrGraph g;
  g.rate("roman", "cpp", 9);
  g.rate("roman", "go", 7);
  g.rate("alex", "cpp", 5);

  CountingFunctor user_counter = g.traverseUserRatings("roman", CountingFunctor{});
  CountingFunctor article_counter = g.traverseArticleRatings("cpp", CountingFunctor{});
  CountingFunctor missing_counter = g.traverseUserRatings("unknown", CountingFunctor{});

  BOOST_CHECK_EQUAL(user_counter.calls, 2);
  BOOST_CHECK_EQUAL(article_counter.calls, 2);
  BOOST_CHECK_EQUAL(missing_counter.calls, 0);
}

BOOST_AUTO_TEST_CASE(test_traverse_all_ratings)
{
  StrGraph g;
  g.rate("roman", "cpp", 9);
  g.rate("roman", "go", 7);
  g.rate("alex", "cpp", 5);

  std::set< std::tuple< std::string, std::string, int > > visited;
  size_t calls = 0;
  g.traverseAllRatings(
      [&](const std::string &u, const std::string &a, const int &w)
      {
        ++calls;
        visited.emplace(u, a, w);
      });

  std::set< std::tuple< std::string, std::string, int > > expected{
      {"roman", "cpp", 9}, {"roman", "go", 7}, {"alex", "cpp", 5}};
  BOOST_CHECK_EQUAL(calls, 3);
  BOOST_CHECK(visited == expected);
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  StrGraph g;
  g.rate("roman", "cpp", 9);
  g.rate("alex", "go", 5);

  g.clear();

  BOOST_CHECK(g.empty());
  BOOST_CHECK_EQUAL(g.ratingsCount(), 0);
  BOOST_CHECK_EQUAL(g.userRatingsCount("roman"), 0);
  BOOST_CHECK_EQUAL(g.articleRatingsCount("go"), 0);

  BOOST_CHECK(g.rate("roman", "cpp", 1) == true);
  BOOST_CHECK_EQUAL(g.ratingsCount(), 1);
}

BOOST_AUTO_TEST_CASE(test_swap)
{
  StrGraph first;
  first.rate("roman", "cpp", 9);
  first.rate("roman", "go", 7);

  StrGraph second;
  second.rate("alex", "rust", 3);

  first.swap(second);

  BOOST_CHECK_EQUAL(first.ratingsCount(), 1);
  BOOST_CHECK(first.hasRating("alex", "rust"));
  BOOST_CHECK(first.hasRating("roman", "cpp") == false);

  BOOST_CHECK_EQUAL(second.ratingsCount(), 2);
  BOOST_CHECK(second.hasRating("roman", "cpp"));
  BOOST_CHECK_EQUAL(second.articleRatingsCount("rust"), 0);
}

BOOST_AUTO_TEST_CASE(test_copy_independent)
{
  StrGraph g;
  g.rate("roman", "cpp", 9);

  StrGraph copy(g);
  g.rate("roman", "cpp", 1);
  g.rate("alex", "go", 5);
  g.eraseUser("roman");

  BOOST_CHECK_EQUAL(copy.ratingsCount(), 1);
  BOOST_REQUIRE(copy.findRating("roman", "cpp") != nullptr);
  BOOST_CHECK_EQUAL(*copy.findRating("roman", "cpp"), 9);
  BOOST_CHECK(copy.hasRating("alex", "go") == false);
}

BOOST_AUTO_TEST_CASE(test_different_key_types)
{
  Graph< int, std::string, double > g;

  BOOST_CHECK(g.rate(1, "cpp", 9.5) == true);
  BOOST_CHECK(g.rate(2, "cpp", 4.0) == true);

  BOOST_REQUIRE(g.findRating(1, "cpp") != nullptr);
  BOOST_CHECK_CLOSE(*g.findRating(1, "cpp"), 9.5, 1e-9);
  BOOST_CHECK_EQUAL(g.articleRatingsCount("cpp"), 2);

  double sum = 0.0;
  g.traverseArticleRatings("cpp", [&sum](const int &, const double &w) { sum += w; });
  BOOST_CHECK_CLOSE(sum, 13.5, 1e-9);

  BOOST_CHECK_EQUAL(g.eraseArticle("cpp"), 2);
  BOOST_CHECK(g.empty());
}

BOOST_AUTO_TEST_CASE(test_many_vertices_growth)
{
  IntGraph g;
  Reference ref;
  for (int u = 0; u < 300; ++u)
  {
    g.rate(u, u % 7, u);
    g.rate(u, 1000 + u, -u);
    ref[{u, u % 7}] = u;
    ref[{u, 1000 + u}] = -u;
  }

  checkInvariants(g, ref);
}

BOOST_AUTO_TEST_CASE(test_stress_against_reference)
{
  IntGraph g;
  Reference ref;

  std::mt19937 gen(42);
  std::uniform_int_distribution< int > op_dist(0, 9);
  std::uniform_int_distribution< int > key_dist(0, 60);
  std::uniform_int_distribution< int > weight_dist(1, 10);

  for (int step = 0; step < 5000; ++step)
  {
    int user = key_dist(gen);
    int article = key_dist(gen);
    int op = op_dist(gen);

    if (op < 6)
    {
      int w = weight_dist(gen);
      bool expected_new = ref.find(std::make_pair(user, article)) == ref.end();
      BOOST_REQUIRE_EQUAL(g.rate(user, article, w), expected_new);
      ref[{user, article}] = w;
    }
    else if (op < 8)
    {
      bool expected = ref.erase(std::make_pair(user, article)) > 0;
      BOOST_REQUIRE_EQUAL(g.eraseRating(user, article), expected);
    }
    else if (op == 8)
    {
      size_t expected = 0;
      for (auto it = ref.begin(); it != ref.end();)
      {
        if (it->first.first == user)
        {
          it = ref.erase(it);
          ++expected;
        }
        else
        {
          ++it;
        }
      }
      BOOST_REQUIRE_EQUAL(g.eraseUser(user), expected);
    }
    else
    {
      size_t expected = 0;
      for (auto it = ref.begin(); it != ref.end();)
      {
        if (it->first.second == article)
        {
          it = ref.erase(it);
          ++expected;
        }
        else
        {
          ++it;
        }
      }
      BOOST_REQUIRE_EQUAL(g.eraseArticle(article), expected);
    }

    if (step % 250 == 0)
    {
      checkInvariants(g, ref);
    }
  }

  checkInvariants(g, ref);
}

BOOST_AUTO_TEST_SUITE_END()
