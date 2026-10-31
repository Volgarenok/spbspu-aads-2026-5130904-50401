#include "Entities.hpp"
#include "Storage.hpp"
#include <boost/test/unit_test.hpp>
#include <cstddef>
#include <map>
#include <string>
#include <utility>
#include <vector>

using namespace khasnulin;

namespace
{
  using CountEntry = std::pair< std::string, std::size_t >;
  using RatingEntry = std::pair< std::string, int >;

  Latent makeLatent(double base)
  {
    Latent latent{};
    for (std::size_t i = 0; i < latentDimensionCount; ++i)
    {
      latent.v[i] = base + 0.01 * static_cast< double >(i);
    }
    return latent;
  }

  void noLearn(Latent &, Latent &, const Rating &) noexcept
  {
  }

  bool addArticle(Storage &storage, const std::string &name, const std::string &category)
  {
    return storage.addArticle(ArticleName(name), Category(category), Difficulty::beginner, makeLatent(0.2));
  }

  bool rate(Storage &storage, const std::string &user, const std::string &article, int value)
  {
    return storage.rate(UserName(user), ArticleName(article), Rating(value), noLearn);
  }

  std::vector< std::string > collectUserNames(const Storage &storage)
  {
    std::vector< std::string > result;
    storage.traverseUsers([&result](const User &user) { result.push_back(user.name().value()); });
    return result;
  }

  std::vector< std::string > collectArticleNames(const Storage &storage)
  {
    std::vector< std::string > result;
    storage.traverseArticles([&result](const Article &article) { result.push_back(article.name().value()); });
    return result;
  }

  std::vector< CountEntry > collectTags(const Storage &storage)
  {
    std::vector< CountEntry > result;
    storage.traverseTags([&result](const Tag &tag, Storage::count_type count)
                         { result.push_back({tag.value(), count}); });
    return result;
  }

  std::vector< CountEntry > collectCategories(const Storage &storage)
  {
    std::vector< CountEntry > result;
    storage.traverseCategories([&result](const Category &category, Storage::count_type count)
                               { result.push_back({category.value(), count}); });
    return result;
  }

  std::vector< RatingEntry > collectUserRatings(const Storage &storage, const std::string &user)
  {
    std::vector< RatingEntry > result;
    storage.traverseUserRatings(UserName(user), [&result](const Article &article, const Rating &rating)
                                { result.push_back({article.name().value(), rating.value()}); });
    return result;
  }

  std::vector< RatingEntry > collectArticleRatings(const Storage &storage, const std::string &article)
  {
    std::vector< RatingEntry > result;
    storage.traverseArticleRatings(ArticleName(article), [&result](const User &user, const Rating &rating)
                                   { result.push_back({user.name().value(), rating.value()}); });
    return result;
  }

  // Пересчитывает счётчики тегов по самим сущностям: инвариант индекса тегов.
  std::vector< CountEntry > recountTags(const Storage &storage)
  {
    std::map< std::string, std::size_t > counts;
    storage.traverseUsers(
        [&counts](const User &user)
        {
          for (auto it = user.preferences().begin(); it != user.preferences().end(); ++it)
          {
            ++counts[it->first.value()];
          }
        });
    storage.traverseArticles(
        [&counts](const Article &article)
        {
          for (auto it = article.tags().begin(); it != article.tags().end(); ++it)
          {
            ++counts[it->first.value()];
          }
        });
    return std::vector< CountEntry >(counts.begin(), counts.end());
  }

  // Сумма оценок по пользователям должна совпадать с суммой по статьям.
  bool ratingCountsConsistent(const Storage &storage)
  {
    std::size_t byUsers = 0;
    std::size_t byArticles = 0;
    storage.traverseUsers([&storage, &byUsers](const User &user)
                          { byUsers += storage.userRatingsCount(user.name()); });
    storage.traverseArticles([&storage, &byArticles](const Article &article)
                             { byArticles += storage.articleRatingsCount(article.name()); });
    return byUsers == byArticles;
  }

  void fillSample(Storage &storage)
  {
    storage.addUser(UserName("roman"), makeLatent(0.1));
    storage.addUser(UserName("alex"), makeLatent(0.15));
    addArticle(storage, "Advanced C++", "cpp");
    addArticle(storage, "Move Semantics", "cpp");
    addArticle(storage, "Go Basics", "go");
    storage.addPreference(UserName("roman"), Tag("cpp"));
    storage.addPreference(UserName("roman"), Tag("backend"));
    storage.addPreference(UserName("alex"), Tag("go"));
    storage.addArticleTag(ArticleName("Advanced C++"), Tag("cpp"));
    storage.addArticleTag(ArticleName("Advanced C++"), Tag("templates"));
    storage.addArticleTag(ArticleName("Go Basics"), Tag("go"));
    rate(storage, "roman", "Advanced C++", 10);
    rate(storage, "roman", "Go Basics", 4);
    rate(storage, "alex", "Advanced C++", 7);
    rate(storage, "alex", "Move Semantics", 8);
  }
}

BOOST_AUTO_TEST_SUITE(storage_entities_tests)

BOOST_AUTO_TEST_CASE(new_storage_is_empty)
{
  Storage storage;
  BOOST_CHECK(storage.findUser(UserName("roman")) == nullptr);
  BOOST_CHECK(storage.findArticle(ArticleName("Advanced C++")) == nullptr);
  BOOST_CHECK(collectUserNames(storage).empty());
  BOOST_CHECK(collectArticleNames(storage).empty());
  BOOST_CHECK(collectTags(storage).empty());
  BOOST_CHECK(collectCategories(storage).empty());
}

BOOST_AUTO_TEST_CASE(add_user_creates_user)
{
  Storage storage;
  BOOST_CHECK(storage.addUser(UserName("roman"), makeLatent(0.1)));
  const User *user = storage.findUser(UserName("roman"));
  BOOST_CHECK(user != nullptr);
  BOOST_CHECK(user->preferences().empty());
  BOOST_CHECK(storage.userRatingsCount(UserName("roman")) == 0);
}

BOOST_AUTO_TEST_CASE(add_user_rejects_duplicate)
{
  Storage storage;
  storage.addUser(UserName("roman"), makeLatent(0.1));
  BOOST_CHECK(storage.addUser(UserName(" roman "), makeLatent(0.5)) == false);
  std::vector< std::string > expected = {"roman"};
  BOOST_CHECK(collectUserNames(storage) == expected);
}

BOOST_AUTO_TEST_CASE(add_article_creates_article_and_counts_category)
{
  Storage storage;
  BOOST_CHECK(storage.addArticle(ArticleName("Advanced C++"), Category("cpp"), Difficulty::advanced,
                                 makeLatent(0.2)));
  const Article *article = storage.findArticle(ArticleName("Advanced C++"));
  BOOST_CHECK(article != nullptr);
  BOOST_CHECK(article->category() == Category("cpp"));
  BOOST_CHECK(article->difficulty() == Difficulty::advanced);
  std::vector< CountEntry > expected = {{"cpp", 1}};
  BOOST_CHECK(collectCategories(storage) == expected);
}

BOOST_AUTO_TEST_CASE(add_article_rejects_duplicate)
{
  Storage storage;
  addArticle(storage, "Advanced C++", "cpp");
  BOOST_CHECK(addArticle(storage, "Advanced C++", "go") == false);
  std::vector< CountEntry > expected = {{"cpp", 1}};
  BOOST_CHECK(collectCategories(storage) == expected);
}

BOOST_AUTO_TEST_CASE(traverse_users_and_articles_in_name_order)
{
  Storage storage;
  fillSample(storage);
  std::vector< std::string > expectedUsers = {"alex", "roman"};
  std::vector< std::string > expectedArticles = {"Advanced C++", "Go Basics", "Move Semantics"};
  BOOST_CHECK(collectUserNames(storage) == expectedUsers);
  BOOST_CHECK(collectArticleNames(storage) == expectedArticles);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(storage_tags_tests)

BOOST_AUTO_TEST_CASE(add_preference_to_missing_user_returns_false)
{
  Storage storage;
  BOOST_CHECK(storage.addPreference(UserName("roman"), Tag("cpp")) == false);
  BOOST_CHECK(collectTags(storage).empty());
}

BOOST_AUTO_TEST_CASE(add_preference_acquires_tag)
{
  Storage storage;
  storage.addUser(UserName("roman"), makeLatent(0.1));
  BOOST_CHECK(storage.addPreference(UserName("roman"), Tag("cpp")));
  BOOST_CHECK(storage.findUser(UserName("roman"))->preferences().size() == 1);
  std::vector< CountEntry > expected = {{"cpp", 1}};
  BOOST_CHECK(collectTags(storage) == expected);
}

BOOST_AUTO_TEST_CASE(duplicate_preference_is_accepted_but_not_counted)
{
  Storage storage;
  storage.addUser(UserName("roman"), makeLatent(0.1));
  storage.addPreference(UserName("roman"), Tag("cpp"));
  BOOST_CHECK(storage.addPreference(UserName("roman"), Tag(" CPP ")));
  std::vector< CountEntry > expected = {{"cpp", 1}};
  BOOST_CHECK(collectTags(storage) == expected);
}

BOOST_AUTO_TEST_CASE(add_article_tag_to_missing_article_returns_false)
{
  Storage storage;
  BOOST_CHECK(storage.addArticleTag(ArticleName("Advanced C++"), Tag("cpp")) == false);
  BOOST_CHECK(collectTags(storage).empty());
}

BOOST_AUTO_TEST_CASE(duplicate_article_tag_is_accepted_but_not_counted)
{
  Storage storage;
  addArticle(storage, "Advanced C++", "cpp");
  storage.addArticleTag(ArticleName("Advanced C++"), Tag("templates"));
  BOOST_CHECK(storage.addArticleTag(ArticleName("Advanced C++"), Tag("Templates")));
  std::vector< CountEntry > expected = {{"templates", 1}};
  BOOST_CHECK(collectTags(storage) == expected);
}

BOOST_AUTO_TEST_CASE(tags_are_shared_between_users_and_articles)
{
  Storage storage;
  storage.addUser(UserName("roman"), makeLatent(0.1));
  addArticle(storage, "Advanced C++", "cpp");
  storage.addPreference(UserName("roman"), Tag("cpp"));
  storage.addArticleTag(ArticleName("Advanced C++"), Tag("cpp"));
  std::vector< CountEntry > expected = {{"cpp", 2}};
  BOOST_CHECK(collectTags(storage) == expected);
}

BOOST_AUTO_TEST_CASE(tag_counts_match_entities)
{
  Storage storage;
  fillSample(storage);
  std::vector< CountEntry > expected = {{"backend", 1}, {"cpp", 2}, {"go", 2}, {"templates", 1}};
  BOOST_CHECK(collectTags(storage) == expected);
  BOOST_CHECK(collectTags(storage) == recountTags(storage));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(storage_rating_tests)

BOOST_AUTO_TEST_CASE(rate_missing_entities_returns_false_without_learning)
{
  Storage storage;
  storage.addUser(UserName("roman"), makeLatent(0.1));
  addArticle(storage, "Advanced C++", "cpp");
  int calls = 0;
  auto learn = [&calls](Latent &, Latent &, const Rating &) noexcept { ++calls; };
  BOOST_CHECK(storage.rate(UserName("alex"), ArticleName("Advanced C++"), Rating(5), learn) == false);
  BOOST_CHECK(storage.rate(UserName("roman"), ArticleName("Go Basics"), Rating(5), learn) == false);
  BOOST_CHECK(calls == 0);
  BOOST_CHECK(storage.userRatingsCount(UserName("roman")) == 0);
  BOOST_CHECK(storage.articleRatingsCount(ArticleName("Advanced C++")) == 0);
}

BOOST_AUTO_TEST_CASE(rate_stores_rating_and_learns_once)
{
  Storage storage;
  storage.addUser(UserName("roman"), makeLatent(0.1));
  addArticle(storage, "Advanced C++", "cpp");
  int calls = 0;
  bool argumentsCorrect = false;
  auto learn = [&](Latent &user, Latent &article, const Rating &rating) noexcept
  {
    ++calls;
    argumentsCorrect = user.v[0] == 0.1 && article.v[0] == 0.2 && rating.value() == 7;
    user.v[0] = 0.5;
    article.v[0] = 0.6;
  };
  BOOST_CHECK(storage.rate(UserName("roman"), ArticleName("Advanced C++"), Rating(7), learn));
  BOOST_CHECK(calls == 1);
  BOOST_CHECK(argumentsCorrect);
  BOOST_CHECK(storage.hasRating(UserName("roman"), ArticleName("Advanced C++")));
  BOOST_CHECK(storage.findUser(UserName("roman"))->latent().v[0] == 0.5);
  BOOST_CHECK(storage.findArticle(ArticleName("Advanced C++"))->latent().v[0] == 0.6);
}

BOOST_AUTO_TEST_CASE(rerate_replaces_rating_and_learns_again)
{
  Storage storage;
  storage.addUser(UserName("roman"), makeLatent(0.1));
  addArticle(storage, "Advanced C++", "cpp");
  int calls = 0;
  auto learn = [&calls](Latent &, Latent &, const Rating &) noexcept { ++calls; };
  storage.rate(UserName("roman"), ArticleName("Advanced C++"), Rating(3), learn);
  BOOST_CHECK(storage.rate(UserName("roman"), ArticleName("Advanced C++"), Rating(9), learn));
  BOOST_CHECK(calls == 2);
  BOOST_CHECK(storage.userRatingsCount(UserName("roman")) == 1);
  std::vector< RatingEntry > expected = {{"Advanced C++", 9}};
  BOOST_CHECK(collectUserRatings(storage, "roman") == expected);
}

BOOST_AUTO_TEST_CASE(has_rating_distinguishes_pairs)
{
  Storage storage;
  fillSample(storage);
  BOOST_CHECK(storage.hasRating(UserName("roman"), ArticleName("Advanced C++")));
  BOOST_CHECK(storage.hasRating(UserName("roman"), ArticleName("Move Semantics")) == false);
  BOOST_CHECK(storage.hasRating(UserName("ivan"), ArticleName("Advanced C++")) == false);
}

BOOST_AUTO_TEST_CASE(traverse_user_ratings_in_article_name_order)
{
  Storage storage;
  fillSample(storage);
  std::vector< RatingEntry > expected = {{"Advanced C++", 10}, {"Go Basics", 4}};
  BOOST_CHECK(collectUserRatings(storage, "roman") == expected);
}

BOOST_AUTO_TEST_CASE(traverse_article_ratings_in_user_name_order)
{
  Storage storage;
  fillSample(storage);
  std::vector< RatingEntry > expected = {{"alex", 7}, {"roman", 10}};
  BOOST_CHECK(collectArticleRatings(storage, "Advanced C++") == expected);
}

BOOST_AUTO_TEST_CASE(ratings_counts)
{
  Storage storage;
  fillSample(storage);
  BOOST_CHECK(storage.userRatingsCount(UserName("roman")) == 2);
  BOOST_CHECK(storage.userRatingsCount(UserName("alex")) == 2);
  BOOST_CHECK(storage.articleRatingsCount(ArticleName("Advanced C++")) == 2);
  BOOST_CHECK(storage.articleRatingsCount(ArticleName("Move Semantics")) == 1);
  BOOST_CHECK(ratingCountsConsistent(storage));
}

BOOST_AUTO_TEST_CASE(missing_entities_have_no_ratings)
{
  Storage storage;
  fillSample(storage);
  BOOST_CHECK(storage.userRatingsCount(UserName("ivan")) == 0);
  BOOST_CHECK(storage.articleRatingsCount(ArticleName("Unknown")) == 0);
  BOOST_CHECK(collectUserRatings(storage, "ivan").empty());
  BOOST_CHECK(collectArticleRatings(storage, "Unknown").empty());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(storage_erase_tests)

BOOST_AUTO_TEST_CASE(erase_missing_returns_false)
{
  Storage storage;
  fillSample(storage);
  BOOST_CHECK(storage.eraseUser(UserName("ivan")) == false);
  BOOST_CHECK(storage.eraseArticle(ArticleName("Unknown")) == false);
  BOOST_CHECK(collectUserNames(storage).size() == 2);
  BOOST_CHECK(collectArticleNames(storage).size() == 3);
}

BOOST_AUTO_TEST_CASE(erase_user_cascades)
{
  Storage storage;
  fillSample(storage);
  BOOST_CHECK(storage.eraseUser(UserName("roman")));
  BOOST_CHECK(storage.findUser(UserName("roman")) == nullptr);
  BOOST_CHECK(storage.hasRating(UserName("roman"), ArticleName("Advanced C++")) == false);
  BOOST_CHECK(storage.articleRatingsCount(ArticleName("Advanced C++")) == 1);
  BOOST_CHECK(storage.articleRatingsCount(ArticleName("Go Basics")) == 0);
  std::vector< RatingEntry > expectedRatings = {{"alex", 7}};
  BOOST_CHECK(collectArticleRatings(storage, "Advanced C++") == expectedRatings);
  std::vector< CountEntry > expectedTags = {{"cpp", 1}, {"go", 2}, {"templates", 1}};
  BOOST_CHECK(collectTags(storage) == expectedTags);
  BOOST_CHECK(collectTags(storage) == recountTags(storage));
  BOOST_CHECK(ratingCountsConsistent(storage));
}

BOOST_AUTO_TEST_CASE(erase_article_cascades)
{
  Storage storage;
  fillSample(storage);
  BOOST_CHECK(storage.eraseArticle(ArticleName("Advanced C++")));
  BOOST_CHECK(storage.findArticle(ArticleName("Advanced C++")) == nullptr);
  BOOST_CHECK(storage.userRatingsCount(UserName("roman")) == 1);
  BOOST_CHECK(storage.userRatingsCount(UserName("alex")) == 1);
  std::vector< RatingEntry > expectedRatings = {{"Go Basics", 4}};
  BOOST_CHECK(collectUserRatings(storage, "roman") == expectedRatings);
  std::vector< CountEntry > expectedTags = {{"backend", 1}, {"cpp", 1}, {"go", 2}};
  BOOST_CHECK(collectTags(storage) == expectedTags);
  BOOST_CHECK(collectTags(storage) == recountTags(storage));
  std::vector< CountEntry > expectedCategories = {{"cpp", 1}, {"go", 1}};
  BOOST_CHECK(collectCategories(storage) == expectedCategories);
  BOOST_CHECK(ratingCountsConsistent(storage));
}

BOOST_AUTO_TEST_CASE(erase_last_article_of_category_removes_category)
{
  Storage storage;
  fillSample(storage);
  storage.eraseArticle(ArticleName("Go Basics"));
  std::vector< CountEntry > expected = {{"cpp", 2}};
  BOOST_CHECK(collectCategories(storage) == expected);
}

BOOST_AUTO_TEST_CASE(readded_user_starts_clean)
{
  Storage storage;
  fillSample(storage);
  storage.eraseUser(UserName("roman"));
  BOOST_CHECK(storage.addUser(UserName("roman"), makeLatent(0.3)));
  BOOST_CHECK(storage.findUser(UserName("roman"))->preferences().empty());
  BOOST_CHECK(storage.userRatingsCount(UserName("roman")) == 0);
  BOOST_CHECK(collectUserRatings(storage, "roman").empty());
}

BOOST_AUTO_TEST_CASE(readded_article_starts_clean)
{
  Storage storage;
  fillSample(storage);
  storage.eraseArticle(ArticleName("Advanced C++"));
  BOOST_CHECK(addArticle(storage, "Advanced C++", "systems"));
  BOOST_CHECK(storage.findArticle(ArticleName("Advanced C++"))->tags().empty());
  BOOST_CHECK(storage.articleRatingsCount(ArticleName("Advanced C++")) == 0);
  BOOST_CHECK(storage.hasRating(UserName("alex"), ArticleName("Advanced C++")) == false);
}

BOOST_AUTO_TEST_CASE(erase_everything_leaves_empty_indexes)
{
  Storage storage;
  fillSample(storage);
  storage.eraseUser(UserName("roman"));
  storage.eraseUser(UserName("alex"));
  storage.eraseArticle(ArticleName("Advanced C++"));
  storage.eraseArticle(ArticleName("Move Semantics"));
  storage.eraseArticle(ArticleName("Go Basics"));
  BOOST_CHECK(collectUserNames(storage).empty());
  BOOST_CHECK(collectArticleNames(storage).empty());
  BOOST_CHECK(collectTags(storage).empty());
  BOOST_CHECK(collectCategories(storage).empty());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(storage_state_tests)

BOOST_AUTO_TEST_CASE(clear_resets_everything)
{
  Storage storage;
  fillSample(storage);
  storage.clear();
  BOOST_CHECK(collectUserNames(storage).empty());
  BOOST_CHECK(collectArticleNames(storage).empty());
  BOOST_CHECK(collectTags(storage).empty());
  BOOST_CHECK(collectCategories(storage).empty());
  BOOST_CHECK(storage.hasRating(UserName("roman"), ArticleName("Advanced C++")) == false);
  BOOST_CHECK(storage.addUser(UserName("roman"), makeLatent(0.1)));
  BOOST_CHECK(storage.userRatingsCount(UserName("roman")) == 0);
}

BOOST_AUTO_TEST_CASE(swap_exchanges_everything)
{
  Storage filled;
  fillSample(filled);
  Storage other;
  other.addUser(UserName("ivan"), makeLatent(0.4));
  filled.swap(other);
  std::vector< std::string > expectedFilled = {"ivan"};
  BOOST_CHECK(collectUserNames(filled) == expectedFilled);
  BOOST_CHECK(collectArticleNames(filled).empty());
  BOOST_CHECK(collectTags(filled).empty());
  BOOST_CHECK(collectUserNames(other).size() == 2);
  BOOST_CHECK(other.userRatingsCount(UserName("roman")) == 2);
  BOOST_CHECK(collectTags(other) == recountTags(other));
}

BOOST_AUTO_TEST_CASE(copy_is_independent)
{
  Storage original;
  fillSample(original);
  Storage copy = original;
  copy.eraseUser(UserName("roman"));
  copy.addPreference(UserName("alex"), Tag("rust"));
  rate(copy, "alex", "Go Basics", 2);
  BOOST_CHECK(original.userRatingsCount(UserName("roman")) == 2);
  BOOST_CHECK(original.hasRating(UserName("alex"), ArticleName("Go Basics")) == false);
  BOOST_CHECK(collectTags(original) == recountTags(original));
  std::vector< CountEntry > expectedTags = {{"backend", 1}, {"cpp", 2}, {"go", 2}, {"templates", 1}};
  BOOST_CHECK(collectTags(original) == expectedTags);
}

BOOST_AUTO_TEST_SUITE_END()
