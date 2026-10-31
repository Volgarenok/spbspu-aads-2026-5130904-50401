#include "ArticleFilter.hpp"
#include <boost/test/unit_test.hpp>
#include <string>

using namespace khasnulin;

namespace
{
  Article makeArticle(const std::string &category, Difficulty difficulty)
  {
    return Article(ArticleName("Sample Article"), Category(category), difficulty, Latent{});
  }

  const Difficulty allDifficulties[3] = {Difficulty::beginner, Difficulty::intermediate,
                                         Difficulty::advanced};
}

BOOST_AUTO_TEST_SUITE(article_filter_construction_tests)

BOOST_AUTO_TEST_CASE(default_filter_has_no_conditions)
{
  ArticleFilter filter;
  BOOST_CHECK(filter.category() == nullptr);
  BOOST_CHECK(filter.difficulty() == nullptr);
}

BOOST_AUTO_TEST_CASE(getters_return_given_conditions)
{
  Category category("cpp");
  Difficulty difficulty = Difficulty::advanced;
  ArticleFilter filter(&category, &difficulty);
  BOOST_CHECK(filter.category() == &category);
  BOOST_CHECK(filter.difficulty() == &difficulty);
  BOOST_CHECK(filter.category()->value() == "cpp");
  BOOST_CHECK(*filter.difficulty() == Difficulty::advanced);
}

BOOST_AUTO_TEST_CASE(only_difficulty_can_be_given)
{
  Difficulty difficulty = Difficulty::beginner;
  ArticleFilter filter(nullptr, &difficulty);
  BOOST_CHECK(filter.category() == nullptr);
  BOOST_CHECK(filter.difficulty() == &difficulty);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(article_filter_matches_tests)

BOOST_AUTO_TEST_CASE(default_filter_accepts_any_article)
{
  ArticleFilter filter;
  for (Difficulty difficulty : allDifficulties)
  {
    BOOST_CHECK(filter.matches(makeArticle("cpp", difficulty)));
    BOOST_CHECK(filter.matches(makeArticle("ai", difficulty)));
  }
}

BOOST_AUTO_TEST_CASE(category_only_ignores_difficulty)
{
  Category category("cpp");
  ArticleFilter filter(&category);
  for (Difficulty difficulty : allDifficulties)
  {
    BOOST_CHECK(filter.matches(makeArticle("cpp", difficulty)));
    BOOST_CHECK(!filter.matches(makeArticle("ai", difficulty)));
  }
}

BOOST_AUTO_TEST_CASE(difficulty_only_ignores_category)
{
  Difficulty difficulty = Difficulty::intermediate;
  ArticleFilter filter(nullptr, &difficulty);
  BOOST_CHECK(filter.matches(makeArticle("cpp", Difficulty::intermediate)));
  BOOST_CHECK(filter.matches(makeArticle("ai", Difficulty::intermediate)));
  BOOST_CHECK(!filter.matches(makeArticle("cpp", Difficulty::beginner)));
  BOOST_CHECK(!filter.matches(makeArticle("ai", Difficulty::advanced)));
}

BOOST_AUTO_TEST_CASE(both_conditions_must_hold)
{
  Category category("cpp");
  Difficulty difficulty = Difficulty::advanced;
  ArticleFilter filter(&category, &difficulty);
  BOOST_CHECK(filter.matches(makeArticle("cpp", Difficulty::advanced)));
  BOOST_CHECK(!filter.matches(makeArticle("cpp", Difficulty::beginner)));
  BOOST_CHECK(!filter.matches(makeArticle("ai", Difficulty::advanced)));
  BOOST_CHECK(!filter.matches(makeArticle("ai", Difficulty::beginner)));
}

BOOST_AUTO_TEST_CASE(category_is_compared_after_normalization)
{
  Category category("  CPP ");
  ArticleFilter filter(&category);
  BOOST_CHECK(filter.matches(makeArticle("cpp", Difficulty::beginner)));
}

BOOST_AUTO_TEST_CASE(filter_reads_conditions_without_copying)
{
  Difficulty difficulty = Difficulty::beginner;
  ArticleFilter filter(nullptr, &difficulty);
  BOOST_CHECK(filter.matches(makeArticle("cpp", Difficulty::beginner)));
  difficulty = Difficulty::advanced;
  BOOST_CHECK(!filter.matches(makeArticle("cpp", Difficulty::beginner)));
  BOOST_CHECK(filter.matches(makeArticle("cpp", Difficulty::advanced)));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(article_filter_value_tests)

BOOST_AUTO_TEST_CASE(copy_keeps_conditions)
{
  Category category("cpp");
  Difficulty difficulty = Difficulty::advanced;
  ArticleFilter original(&category, &difficulty);
  ArticleFilter copy = original;
  BOOST_CHECK(copy.category() == &category);
  BOOST_CHECK(copy.difficulty() == &difficulty);
  BOOST_CHECK(copy.matches(makeArticle("cpp", Difficulty::advanced)));
  BOOST_CHECK(!copy.matches(makeArticle("cpp", Difficulty::beginner)));
}

BOOST_AUTO_TEST_CASE(assignment_replaces_conditions)
{
  Category category("cpp");
  Difficulty difficulty = Difficulty::advanced;
  ArticleFilter filter(&category, &difficulty);
  filter = ArticleFilter();
  BOOST_CHECK(filter.category() == nullptr);
  BOOST_CHECK(filter.difficulty() == nullptr);
  BOOST_CHECK(filter.matches(makeArticle("ai", Difficulty::beginner)));
}

BOOST_AUTO_TEST_SUITE_END()
