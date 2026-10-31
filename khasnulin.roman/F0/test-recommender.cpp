#include "ArticleFilter.hpp"
#include "Model.hpp"
#include "Recommender.hpp"
#include "Storage.hpp"
#include <algorithm>
#include <boost/test/unit_test.hpp>
#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>

using namespace khasnulin;

namespace
{
  const double tolerance = 1e-12;

  struct KeepVectors
  {
    void operator()(Latent &v1, Latent &v2, const Rating &rate) const noexcept
    {
    }
  };

  struct Ranked
  {
    std::string name;
    double score;
  };

  bool rankedBetter(const Ranked &lhs, const Ranked &rhs)
  {
    if (lhs.score != rhs.score)
    {
      return lhs.score > rhs.score;
    }
    return lhs.name < rhs.name;
  }

  Latent firstComponentLatent(double value)
  {
    Latent latent{};
    latent.v[0] = value;
    return latent;
  }

  Latent twoComponentLatent(double first, double second)
  {
    Latent latent{};
    latent.v[0] = first;
    latent.v[1] = second;
    return latent;
  }

  std::string nameOf(const Article *article)
  {
    return article->name().value();
  }

  void addUser(Storage &storage, const std::string &name, const Latent &latent,
               const std::vector< std::string > &preferences)
  {
    BOOST_REQUIRE(storage.addUser(UserName(name), latent));
    for (const std::string &preference : preferences)
    {
      storage.addPreference(UserName(name), Tag(preference));
    }
  }

  void addArticle(Storage &storage, const std::string &name, const std::string &category,
                  Difficulty difficulty, const Latent &latent, const std::vector< std::string > &tags)
  {
    BOOST_REQUIRE(storage.addArticle(ArticleName(name), Category(category), difficulty, latent));
    for (const std::string &tag : tags)
    {
      storage.addArticleTag(ArticleName(name), Tag(tag));
    }
  }

  void rate(Storage &storage, const std::string &user, const std::string &article, int rating)
  {
    BOOST_REQUIRE(storage.rate(UserName(user), ArticleName(article), Rating(rating), KeepVectors()));
  }

  std::vector< Ranked > toRanked(const Vector< Recommender::Recommendation > &result)
  {
    std::vector< Ranked > ranked;
    for (std::size_t i = 0; i < result.getSize(); ++i)
    {
      ranked.push_back(Ranked{nameOf(result[i].article), result[i].score});
    }
    return ranked;
  }

  std::vector< Ranked > referenceRecommend(const Storage &storage, const std::string &userName,
                                           const std::vector< std::string > &candidates, std::size_t limit)
  {
    Scorer scorer;
    const User *user = storage.findUser(UserName(userName));
    std::size_t userRatingCount = storage.userRatingsCount(UserName(userName));
    std::vector< Ranked > ranked;
    for (const std::string &candidate : candidates)
    {
      const Article *article = storage.findArticle(ArticleName(candidate));
      double score = scorer.userArticleScore(user->latent(), user->preferences(), userRatingCount,
                                             article->latent(), article->tags());
      ranked.push_back(Ranked{candidate, score});
    }
    std::sort(ranked.begin(), ranked.end(), rankedBetter);
    if (ranked.size() > limit)
    {
      ranked.resize(limit);
    }
    return ranked;
  }

  std::vector< Ranked > referenceSimilar(const Storage &storage, const std::string &articleName,
                                         const std::vector< std::string > &candidates, std::size_t limit)
  {
    Scorer scorer;
    const Article *source = storage.findArticle(ArticleName(articleName));
    std::size_t sourceCount = storage.articleRatingsCount(ArticleName(articleName));
    std::vector< Ranked > ranked;
    for (const std::string &candidate : candidates)
    {
      const Article *article = storage.findArticle(ArticleName(candidate));
      std::size_t candidateCount = storage.articleRatingsCount(ArticleName(candidate));
      double score = scorer.articleArticleScore(source->latent(), source->tags(), sourceCount,
                                                article->latent(), article->tags(), candidateCount);
      ranked.push_back(Ranked{candidate, score});
    }
    std::sort(ranked.begin(), ranked.end(), rankedBetter);
    if (ranked.size() > limit)
    {
      ranked.resize(limit);
    }
    return ranked;
  }

  bool sameRanking(const std::vector< Ranked > &actual, const std::vector< Ranked > &expected)
  {
    if (actual.size() != expected.size())
    {
      return false;
    }
    for (std::size_t i = 0; i < actual.size(); ++i)
    {
      if (actual[i].name != expected[i].name || std::fabs(actual[i].score - expected[i].score) >= tolerance)
      {
        return false;
      }
    }
    return true;
  }

  std::vector< std::string > namesOf(const std::vector< Ranked > &ranked)
  {
    std::vector< std::string > names;
    for (const Ranked &item : ranked)
    {
      names.push_back(item.name);
    }
    return names;
  }

  void fillCatalog(Storage &storage)
  {
    addUser(storage, "roman", firstComponentLatent(1.0), {"cpp", "backend"});
    addUser(storage, "ivan", firstComponentLatent(0.5), {"ml"});
    addArticle(storage, "Alpha", "cpp", Difficulty::beginner, firstComponentLatent(0.9), {"cpp", "backend"});
    addArticle(storage, "Beta", "cpp", Difficulty::advanced, firstComponentLatent(0.2), {"cpp"});
    addArticle(storage, "Gamma", "ai", Difficulty::intermediate, firstComponentLatent(-0.5), {"ml"});
    addArticle(storage, "Delta", "ai", Difficulty::beginner, firstComponentLatent(0.6), {"backend", "async"});
    addArticle(storage, "Epsilon", "cpp", Difficulty::intermediate, twoComponentLatent(0.3, 0.8), {});
    addArticle(storage, "Zeta", "web", Difficulty::advanced, firstComponentLatent(0.4), {"cpp", "web"});
  }
}

BOOST_AUTO_TEST_SUITE(recommender_defaults_tests)

BOOST_AUTO_TEST_CASE(default_limits_match_design)
{
  BOOST_CHECK(Recommender::defaultRecommendLimit == 10);
  BOOST_CHECK(Recommender::defaultSimilarLimit == 5);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(recommend_tests)

BOOST_AUTO_TEST_CASE(unknown_user_throws)
{
  Storage storage;
  fillCatalog(storage);
  BOOST_CHECK_THROW(Recommender().recommend(storage, UserName("nobody"), ArticleFilter()),
                    std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(unknown_filter_category_throws)
{
  Storage storage;
  fillCatalog(storage);
  Category category("rust");
  BOOST_CHECK_THROW(Recommender().recommend(storage, UserName("roman"), ArticleFilter(&category)),
                    std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(empty_catalog_gives_empty_result)
{
  Storage storage;
  addUser(storage, "roman", firstComponentLatent(1.0), {"cpp"});
  BOOST_CHECK(Recommender().recommend(storage, UserName("roman"), ArticleFilter()).isEmpty());
}

BOOST_AUTO_TEST_CASE(cold_start_ranks_by_tags)
{
  Storage storage;
  fillCatalog(storage);
  std::vector< Ranked > actual =
      toRanked(Recommender().recommend(storage, UserName("roman"), ArticleFilter()));
  std::vector< Ranked > expected =
      referenceRecommend(storage, "roman", {"Alpha", "Beta", "Gamma", "Delta", "Epsilon", "Zeta"}, 10);
  BOOST_CHECK(sameRanking(actual, expected));
  BOOST_CHECK(actual.front().name == "Alpha");
}

BOOST_AUTO_TEST_CASE(rated_articles_are_excluded)
{
  Storage storage;
  fillCatalog(storage);
  rate(storage, "roman", "Alpha", 9);
  rate(storage, "roman", "Gamma", 2);
  std::vector< Ranked > actual =
      toRanked(Recommender().recommend(storage, UserName("roman"), ArticleFilter()));
  std::vector< Ranked > expected =
      referenceRecommend(storage, "roman", {"Beta", "Delta", "Epsilon", "Zeta"}, 10);
  BOOST_CHECK(sameRanking(actual, expected));
  std::vector< std::string > names = namesOf(actual);
  BOOST_CHECK(std::find(names.begin(), names.end(), "Alpha") == names.end());
  BOOST_CHECK(std::find(names.begin(), names.end(), "Gamma") == names.end());
}

BOOST_AUTO_TEST_CASE(ratings_shift_weight_to_vectors)
{
  Storage storage;
  fillCatalog(storage);
  for (const char *article : {"Alpha", "Beta", "Gamma", "Delta", "Epsilon"})
  {
    rate(storage, "ivan", article, 5);
  }
  rate(storage, "roman", "Gamma", 1);
  rate(storage, "roman", "Epsilon", 6);
  std::vector< Ranked > actual =
      toRanked(Recommender().recommend(storage, UserName("roman"), ArticleFilter()));
  std::vector< Ranked > expected =
      referenceRecommend(storage, "roman", {"Alpha", "Beta", "Delta", "Zeta"}, 10);
  BOOST_CHECK(sameRanking(actual, expected));
}

BOOST_AUTO_TEST_CASE(all_rated_gives_empty_result)
{
  Storage storage;
  addUser(storage, "roman", firstComponentLatent(1.0), {"cpp"});
  addArticle(storage, "Alpha", "cpp", Difficulty::beginner, firstComponentLatent(0.9), {"cpp"});
  addArticle(storage, "Beta", "cpp", Difficulty::advanced, firstComponentLatent(0.2), {"cpp"});
  rate(storage, "roman", "Alpha", 8);
  rate(storage, "roman", "Beta", 3);
  BOOST_CHECK(Recommender().recommend(storage, UserName("roman"), ArticleFilter()).isEmpty());
}

BOOST_AUTO_TEST_CASE(limit_keeps_best_in_order)
{
  Storage storage;
  fillCatalog(storage);
  Recommender recommender(Scorer(), 2, 2);
  std::vector< Ranked > actual = toRanked(recommender.recommend(storage, UserName("roman"), ArticleFilter()));
  std::vector< Ranked > expected =
      referenceRecommend(storage, "roman", {"Alpha", "Beta", "Gamma", "Delta", "Epsilon", "Zeta"}, 2);
  BOOST_CHECK(actual.size() == 2);
  BOOST_CHECK(sameRanking(actual, expected));
}

BOOST_AUTO_TEST_CASE(equal_scores_are_ordered_by_name)
{
  Storage storage;
  addUser(storage, "roman", firstComponentLatent(1.0), {"cpp"});
  addArticle(storage, "Charlie", "cpp", Difficulty::beginner, firstComponentLatent(0.5), {"cpp"});
  addArticle(storage, "Alpha", "cpp", Difficulty::beginner, firstComponentLatent(0.5), {"cpp"});
  addArticle(storage, "Bravo", "cpp", Difficulty::beginner, firstComponentLatent(0.5), {"cpp"});
  std::vector< Ranked > actual =
      toRanked(Recommender().recommend(storage, UserName("roman"), ArticleFilter()));
  BOOST_CHECK(namesOf(actual) == std::vector< std::string >({"Alpha", "Bravo", "Charlie"}));
}

BOOST_AUTO_TEST_CASE(tie_break_decides_who_fits_in_limit)
{
  Storage storage;
  addUser(storage, "roman", firstComponentLatent(1.0), {"cpp"});
  addArticle(storage, "Charlie", "cpp", Difficulty::beginner, firstComponentLatent(0.5), {"cpp"});
  addArticle(storage, "Alpha", "cpp", Difficulty::beginner, firstComponentLatent(0.5), {"cpp"});
  addArticle(storage, "Bravo", "cpp", Difficulty::beginner, firstComponentLatent(0.5), {"cpp"});
  Recommender recommender(Scorer(), 2, 2);
  std::vector< Ranked > actual = toRanked(recommender.recommend(storage, UserName("roman"), ArticleFilter()));
  BOOST_CHECK(namesOf(actual) == std::vector< std::string >({"Alpha", "Bravo"}));
}

BOOST_AUTO_TEST_CASE(category_filter_is_applied)
{
  Storage storage;
  fillCatalog(storage);
  Category category("cpp");
  std::vector< Ranked > actual =
      toRanked(Recommender().recommend(storage, UserName("roman"), ArticleFilter(&category)));
  std::vector< Ranked > expected = referenceRecommend(storage, "roman", {"Alpha", "Beta", "Epsilon"}, 10);
  BOOST_CHECK(sameRanking(actual, expected));
}

BOOST_AUTO_TEST_CASE(difficulty_filter_is_applied)
{
  Storage storage;
  fillCatalog(storage);
  Difficulty difficulty = Difficulty::beginner;
  std::vector< Ranked > actual =
      toRanked(Recommender().recommend(storage, UserName("roman"), ArticleFilter(nullptr, &difficulty)));
  std::vector< Ranked > expected = referenceRecommend(storage, "roman", {"Alpha", "Delta"}, 10);
  BOOST_CHECK(sameRanking(actual, expected));
}

BOOST_AUTO_TEST_CASE(both_filters_and_exclusion_work_together)
{
  Storage storage;
  fillCatalog(storage);
  rate(storage, "roman", "Beta", 7);
  Category category("cpp");
  Difficulty difficulty = Difficulty::intermediate;
  std::vector< Ranked > actual =
      toRanked(Recommender().recommend(storage, UserName("roman"), ArticleFilter(&category, &difficulty)));
  BOOST_CHECK(namesOf(actual) == std::vector< std::string >({"Epsilon"}));
}

BOOST_AUTO_TEST_CASE(filter_matching_nothing_gives_empty_result)
{
  Storage storage;
  fillCatalog(storage);
  Category category("web");
  Difficulty difficulty = Difficulty::beginner;
  BOOST_CHECK(
      Recommender().recommend(storage, UserName("roman"), ArticleFilter(&category, &difficulty)).isEmpty());
}

BOOST_AUTO_TEST_CASE(does_not_change_storage)
{
  Storage storage;
  fillCatalog(storage);
  rate(storage, "roman", "Gamma", 4);
  std::vector< Ranked > first =
      toRanked(Recommender().recommend(storage, UserName("roman"), ArticleFilter()));
  std::vector< Ranked > second =
      toRanked(Recommender().recommend(storage, UserName("roman"), ArticleFilter()));
  BOOST_CHECK(sameRanking(first, second));
  BOOST_CHECK(storage.userRatingsCount(UserName("roman")) == 1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(similar_tests)

BOOST_AUTO_TEST_CASE(unknown_article_throws)
{
  Storage storage;
  fillCatalog(storage);
  BOOST_CHECK_THROW(Recommender().similar(storage, ArticleName("Nothing")), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(single_article_gives_empty_result)
{
  Storage storage;
  addArticle(storage, "Alpha", "cpp", Difficulty::beginner, firstComponentLatent(0.9), {"cpp"});
  BOOST_CHECK(Recommender().similar(storage, ArticleName("Alpha")).isEmpty());
}

BOOST_AUTO_TEST_CASE(excludes_source_article_and_ranks_rest)
{
  Storage storage;
  fillCatalog(storage);
  std::vector< Ranked > actual = toRanked(Recommender().similar(storage, ArticleName("Alpha")));
  std::vector< Ranked > expected =
      referenceSimilar(storage, "Alpha", {"Beta", "Gamma", "Delta", "Epsilon", "Zeta"}, 5);
  BOOST_CHECK(sameRanking(actual, expected));
  std::vector< std::string > names = namesOf(actual);
  BOOST_CHECK(std::find(names.begin(), names.end(), "Alpha") == names.end());
}

BOOST_AUTO_TEST_CASE(default_limit_is_five)
{
  Storage storage;
  fillCatalog(storage);
  addArticle(storage, "Eta", "cpp", Difficulty::beginner, firstComponentLatent(0.7), {"cpp"});
  BOOST_CHECK(Recommender().similar(storage, ArticleName("Alpha")).getSize() == 5);
}

BOOST_AUTO_TEST_CASE(rating_counts_of_both_articles_are_used)
{
  Storage storage;
  fillCatalog(storage);
  for (const char *article : {"Alpha", "Beta", "Delta"})
  {
    rate(storage, "ivan", article, 6);
    rate(storage, "roman", article, 8);
  }
  rate(storage, "ivan", "Zeta", 3);
  std::vector< Ranked > actual = toRanked(Recommender().similar(storage, ArticleName("Alpha")));
  std::vector< Ranked > expected =
      referenceSimilar(storage, "Alpha", {"Beta", "Gamma", "Delta", "Epsilon", "Zeta"}, 5);
  BOOST_CHECK(sameRanking(actual, expected));
}

BOOST_AUTO_TEST_CASE(limit_keeps_best_in_order)
{
  Storage storage;
  fillCatalog(storage);
  Recommender recommender(Scorer(), 2, 2);
  std::vector< Ranked > actual = toRanked(recommender.similar(storage, ArticleName("Alpha")));
  std::vector< Ranked > expected =
      referenceSimilar(storage, "Alpha", {"Beta", "Gamma", "Delta", "Epsilon", "Zeta"}, 2);
  BOOST_CHECK(actual.size() == 2);
  BOOST_CHECK(sameRanking(actual, expected));
}

BOOST_AUTO_TEST_CASE(equal_scores_are_ordered_by_name)
{
  Storage storage;
  addArticle(storage, "Source", "cpp", Difficulty::beginner, firstComponentLatent(1.0), {"cpp"});
  addArticle(storage, "Charlie", "cpp", Difficulty::beginner, firstComponentLatent(0.5), {"cpp"});
  addArticle(storage, "Alpha", "cpp", Difficulty::beginner, firstComponentLatent(0.5), {"cpp"});
  addArticle(storage, "Bravo", "cpp", Difficulty::beginner, firstComponentLatent(0.5), {"cpp"});
  std::vector< Ranked > actual = toRanked(Recommender().similar(storage, ArticleName("Source")));
  BOOST_CHECK(namesOf(actual) == std::vector< std::string >({"Alpha", "Bravo", "Charlie"}));
}

BOOST_AUTO_TEST_CASE(rated_articles_are_not_excluded)
{
  Storage storage;
  fillCatalog(storage);
  rate(storage, "roman", "Beta", 9);
  std::vector< std::string > names = namesOf(toRanked(Recommender().similar(storage, ArticleName("Alpha"))));
  BOOST_CHECK(std::find(names.begin(), names.end(), "Beta") != names.end());
}

BOOST_AUTO_TEST_SUITE_END()
