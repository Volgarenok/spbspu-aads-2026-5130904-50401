#include "Entities.hpp"
#include "Repo.hpp"
#include <algorithm>
#include <boost/test/unit_test.hpp>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

using namespace khasnulin;

namespace
{
  using CategoryEntry = std::pair< std::string, std::size_t >;

  Latent makeLatent(double base)
  {
    Latent latent{};
    for (std::size_t i = 0; i < latentDimensionCount; ++i)
    {
      latent.v[i] = base + 0.01 * static_cast< double >(i);
    }
    return latent;
  }

  bool isSameLatent(const Latent &lhs, const Latent &rhs)
  {
    for (std::size_t i = 0; i < latentDimensionCount; ++i)
    {
      if (lhs.v[i] != rhs.v[i])
      {
        return false;
      }
    }
    return true;
  }

  std::vector< std::string > collectNames(const UserRepo &repo)
  {
    std::vector< std::string > result;
    repo.traverse([&result](const User &user) { result.push_back(user.name().value()); });
    return result;
  }

  std::vector< std::string > collectNames(const ArticleRepo &repo)
  {
    std::vector< std::string > result;
    repo.traverse([&result](const Article &article) { result.push_back(article.name().value()); });
    return result;
  }

  std::vector< CategoryEntry > collectCategories(const ArticleRepo &repo)
  {
    std::vector< CategoryEntry > result;
    repo.traverseCategories([&result](const Category &category, ArticleRepo::count_type count)
                            { result.push_back({category.value(), count}); });
    return result;
  }

  bool addArticle(ArticleRepo &repo, const std::string &name, const std::string &category)
  {
    return repo.add(ArticleName(name), Category(category), Difficulty::beginner, makeLatent(0.2));
  }

  std::vector< std::string > makeShuffledNames(const std::string &prefix, std::size_t count, std::size_t step)
  {
    std::vector< std::string > result;
    for (std::size_t i = 0; i < count; ++i)
    {
      result.push_back(prefix + std::to_string((i * step) % count));
    }
    return result;
  }
}

BOOST_AUTO_TEST_SUITE(user_repo_tests)

BOOST_AUTO_TEST_CASE(new_repo_is_empty)
{
  UserRepo repo;
  BOOST_CHECK(repo.empty());
  BOOST_CHECK(repo.size() == 0);
  BOOST_CHECK(repo.find(UserName("roman")) == nullptr);
  BOOST_CHECK(collectNames(repo).empty());
}

BOOST_AUTO_TEST_CASE(add_creates_user_without_preferences)
{
  UserRepo repo;
  Latent latent = makeLatent(0.1);
  BOOST_CHECK(repo.add(UserName("roman"), latent));
  BOOST_CHECK(repo.size() == 1);
  BOOST_CHECK(repo.empty() == false);
  const User *user = repo.find(UserName("roman"));
  BOOST_CHECK(user != nullptr);
  BOOST_CHECK(user->name() == UserName("roman"));
  BOOST_CHECK(user->preferences().empty());
  BOOST_CHECK(isSameLatent(user->latent(), latent));
}

BOOST_AUTO_TEST_CASE(add_rejects_duplicate_name)
{
  UserRepo repo;
  repo.add(UserName("roman"), makeLatent(0.1));
  BOOST_CHECK(repo.add(UserName("  roman "), makeLatent(0.5)) == false);
  BOOST_CHECK(repo.size() == 1);
  BOOST_CHECK(isSameLatent(repo.find(UserName("roman"))->latent(), makeLatent(0.1)));
}

BOOST_AUTO_TEST_CASE(names_differing_in_case_are_different_users)
{
  UserRepo repo;
  BOOST_CHECK(repo.add(UserName("Roman"), makeLatent(0.1)));
  BOOST_CHECK(repo.add(UserName("roman"), makeLatent(0.2)));
  BOOST_CHECK(repo.size() == 2);
}

BOOST_AUTO_TEST_CASE(find_works_on_const_repo)
{
  UserRepo repo;
  repo.add(UserName("roman"), makeLatent(0.1));
  const UserRepo &cref = repo;
  BOOST_CHECK(cref.find(UserName("roman")) != nullptr);
  BOOST_CHECK(cref.find(UserName("alex")) == nullptr);
}

BOOST_AUTO_TEST_CASE(find_gives_mutable_access)
{
  UserRepo repo;
  repo.add(UserName("roman"), makeLatent(0.1));
  User *user = repo.find(UserName("roman"));
  user->addPreference(Tag("cpp"));
  user->latent().v[0] = 0.9;
  const UserRepo &cref = repo;
  BOOST_CHECK(cref.find(UserName("roman"))->preferences().size() == 1);
  BOOST_CHECK(cref.find(UserName("roman"))->latent().v[0] == 0.9);
}

BOOST_AUTO_TEST_CASE(erase_removes_user)
{
  UserRepo repo;
  repo.add(UserName("roman"), makeLatent(0.1));
  repo.add(UserName("alex"), makeLatent(0.2));
  BOOST_CHECK(repo.erase(UserName("roman")));
  BOOST_CHECK(repo.find(UserName("roman")) == nullptr);
  BOOST_CHECK(repo.size() == 1);
  std::vector< std::string > expected = {"alex"};
  BOOST_CHECK(collectNames(repo) == expected);
}

BOOST_AUTO_TEST_CASE(erase_missing_returns_false)
{
  UserRepo repo;
  repo.add(UserName("roman"), makeLatent(0.1));
  BOOST_CHECK(repo.erase(UserName("alex")) == false);
  BOOST_CHECK(repo.erase(UserName("Roman")) == false);
  BOOST_CHECK(repo.size() == 1);
}

BOOST_AUTO_TEST_CASE(erased_name_can_be_added_again)
{
  UserRepo repo;
  repo.add(UserName("roman"), makeLatent(0.1));
  repo.find(UserName("roman"))->addPreference(Tag("cpp"));
  repo.erase(UserName("roman"));
  BOOST_CHECK(repo.add(UserName("roman"), makeLatent(0.3)));
  const User *user = repo.find(UserName("roman"));
  BOOST_CHECK(user->preferences().empty());
  BOOST_CHECK(isSameLatent(user->latent(), makeLatent(0.3)));
}

BOOST_AUTO_TEST_CASE(traverse_visits_users_in_name_order)
{
  UserRepo repo;
  repo.add(UserName("roman"), makeLatent(0.1));
  repo.add(UserName("alex"), makeLatent(0.1));
  repo.add(UserName("Zed"), makeLatent(0.1));
  repo.add(UserName("boris"), makeLatent(0.1));
  std::vector< std::string > expected = {"Zed", "alex", "boris", "roman"};
  BOOST_CHECK(collectNames(repo) == expected);
}

BOOST_AUTO_TEST_CASE(table_key_matches_user_name)
{
  UserRepo repo;
  repo.add(UserName("roman"), makeLatent(0.1));
  repo.add(UserName("alex"), makeLatent(0.1));
  repo.add(UserName("Boris K"), makeLatent(0.1));
  const UserRepo &cref = repo;
  bool consistent = true;
  cref.traverse(
      [&cref, &consistent](const User &user)
      {
        if (cref.find(user.name()) != &user)
        {
          consistent = false;
        }
      });
  BOOST_CHECK(consistent);
}

BOOST_AUTO_TEST_CASE(survives_rehash)
{
  UserRepo repo;
  std::vector< std::string > names = makeShuffledNames("user", 200, 73);
  for (const std::string &name : names)
  {
    BOOST_CHECK(repo.add(UserName(name), makeLatent(0.1)));
  }
  BOOST_CHECK(repo.size() == 200);
  for (const std::string &name : names)
  {
    BOOST_CHECK(repo.find(UserName(name)) != nullptr);
  }
  std::vector< std::string > traversed = collectNames(repo);
  BOOST_CHECK(traversed.size() == 200);
  BOOST_CHECK(std::is_sorted(traversed.begin(), traversed.end()));
}

BOOST_AUTO_TEST_CASE(clear_removes_all_users)
{
  UserRepo repo;
  repo.add(UserName("roman"), makeLatent(0.1));
  repo.add(UserName("alex"), makeLatent(0.1));
  repo.clear();
  BOOST_CHECK(repo.empty());
  BOOST_CHECK(repo.find(UserName("roman")) == nullptr);
  BOOST_CHECK(collectNames(repo).empty());
  BOOST_CHECK(repo.add(UserName("roman"), makeLatent(0.1)));
}

BOOST_AUTO_TEST_CASE(swap_exchanges_contents)
{
  UserRepo lhs;
  lhs.add(UserName("roman"), makeLatent(0.1));
  UserRepo rhs;
  rhs.add(UserName("alex"), makeLatent(0.2));
  rhs.add(UserName("boris"), makeLatent(0.3));
  lhs.swap(rhs);
  std::vector< std::string > expectedLhs = {"alex", "boris"};
  std::vector< std::string > expectedRhs = {"roman"};
  BOOST_CHECK(collectNames(lhs) == expectedLhs);
  BOOST_CHECK(collectNames(rhs) == expectedRhs);
  BOOST_CHECK(lhs.find(UserName("alex")) != nullptr);
  BOOST_CHECK(rhs.find(UserName("alex")) == nullptr);
}

BOOST_AUTO_TEST_CASE(copy_is_independent)
{
  UserRepo original;
  original.add(UserName("roman"), makeLatent(0.1));
  UserRepo copy = original;
  copy.add(UserName("alex"), makeLatent(0.2));
  copy.erase(UserName("roman"));
  copy.find(UserName("alex"))->addPreference(Tag("cpp"));
  std::vector< std::string > expectedOriginal = {"roman"};
  BOOST_CHECK(collectNames(original) == expectedOriginal);
  BOOST_CHECK(original.find(UserName("roman"))->preferences().empty());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(article_repo_tests)

BOOST_AUTO_TEST_CASE(new_repo_is_empty)
{
  ArticleRepo repo;
  BOOST_CHECK(repo.empty());
  BOOST_CHECK(repo.size() == 0);
  BOOST_CHECK(repo.find(ArticleName("Advanced C++")) == nullptr);
  BOOST_CHECK(collectNames(repo).empty());
  BOOST_CHECK(collectCategories(repo).empty());
  BOOST_CHECK(repo.categoryCount(Category("cpp")) == 0);
}

BOOST_AUTO_TEST_CASE(add_creates_article_with_all_fields)
{
  ArticleRepo repo;
  Latent latent = makeLatent(0.2);
  BOOST_CHECK(repo.add(ArticleName("Advanced C++"), Category("cpp"), Difficulty::advanced, latent));
  BOOST_CHECK(repo.size() == 1);
  const Article *article = repo.find(ArticleName("Advanced C++"));
  BOOST_CHECK(article != nullptr);
  BOOST_CHECK(article->name() == ArticleName("Advanced C++"));
  BOOST_CHECK(article->category() == Category("cpp"));
  BOOST_CHECK(article->difficulty() == Difficulty::advanced);
  BOOST_CHECK(article->tags().empty());
  BOOST_CHECK(isSameLatent(article->latent(), latent));
}

BOOST_AUTO_TEST_CASE(add_counts_category)
{
  ArticleRepo repo;
  addArticle(repo, "Advanced C++", "cpp");
  BOOST_CHECK(repo.categoryCount(Category("cpp")) == 1);
  addArticle(repo, "Move Semantics", " CPP ");
  BOOST_CHECK(repo.categoryCount(Category("cpp")) == 2);
}

BOOST_AUTO_TEST_CASE(add_rejects_duplicate_name_without_counting)
{
  ArticleRepo repo;
  addArticle(repo, "Advanced C++", "cpp");
  BOOST_CHECK(repo.add(ArticleName(" Advanced   C++ "), Category("go"), Difficulty::beginner,
                       makeLatent(0.5)) == false);
  BOOST_CHECK(repo.size() == 1);
  BOOST_CHECK(repo.categoryCount(Category("cpp")) == 1);
  BOOST_CHECK(repo.categoryCount(Category("go")) == 0);
  BOOST_CHECK(repo.find(ArticleName("Advanced C++"))->category() == Category("cpp"));
}

BOOST_AUTO_TEST_CASE(names_differing_in_case_are_different_articles)
{
  ArticleRepo repo;
  BOOST_CHECK(addArticle(repo, "Advanced C++", "cpp"));
  BOOST_CHECK(addArticle(repo, "advanced c++", "cpp"));
  BOOST_CHECK(repo.size() == 2);
  BOOST_CHECK(repo.categoryCount(Category("cpp")) == 2);
}

BOOST_AUTO_TEST_CASE(find_works_on_const_repo)
{
  ArticleRepo repo;
  addArticle(repo, "Advanced C++", "cpp");
  const ArticleRepo &cref = repo;
  BOOST_CHECK(cref.find(ArticleName("Advanced C++")) != nullptr);
  BOOST_CHECK(cref.find(ArticleName("Go Basics")) == nullptr);
}

BOOST_AUTO_TEST_CASE(find_gives_mutable_access)
{
  ArticleRepo repo;
  addArticle(repo, "Advanced C++", "cpp");
  repo.find(ArticleName("Advanced C++"))->addTag(Tag("templates"));
  const ArticleRepo &cref = repo;
  BOOST_CHECK(cref.find(ArticleName("Advanced C++"))->tags().size() == 1);
}

BOOST_AUTO_TEST_CASE(erase_releases_category)
{
  ArticleRepo repo;
  addArticle(repo, "Advanced C++", "cpp");
  addArticle(repo, "Move Semantics", "cpp");
  BOOST_CHECK(repo.erase(ArticleName("Advanced C++")));
  BOOST_CHECK(repo.find(ArticleName("Advanced C++")) == nullptr);
  BOOST_CHECK(repo.size() == 1);
  BOOST_CHECK(repo.categoryCount(Category("cpp")) == 1);
}

BOOST_AUTO_TEST_CASE(erase_of_last_article_removes_category)
{
  ArticleRepo repo;
  addArticle(repo, "Advanced C++", "cpp");
  addArticle(repo, "Go Basics", "go");
  repo.erase(ArticleName("Advanced C++"));
  BOOST_CHECK(repo.categoryCount(Category("cpp")) == 0);
  std::vector< CategoryEntry > expected = {{"go", 1}};
  BOOST_CHECK(collectCategories(repo) == expected);
}

BOOST_AUTO_TEST_CASE(erase_missing_returns_false)
{
  ArticleRepo repo;
  addArticle(repo, "Advanced C++", "cpp");
  BOOST_CHECK(repo.erase(ArticleName("Go Basics")) == false);
  BOOST_CHECK(repo.erase(ArticleName("advanced c++")) == false);
  BOOST_CHECK(repo.size() == 1);
  BOOST_CHECK(repo.categoryCount(Category("cpp")) == 1);
}

BOOST_AUTO_TEST_CASE(erased_name_can_be_added_again_with_other_category)
{
  ArticleRepo repo;
  addArticle(repo, "Advanced C++", "cpp");
  repo.erase(ArticleName("Advanced C++"));
  BOOST_CHECK(addArticle(repo, "Advanced C++", "systems"));
  BOOST_CHECK(repo.categoryCount(Category("cpp")) == 0);
  BOOST_CHECK(repo.categoryCount(Category("systems")) == 1);
  BOOST_CHECK(repo.find(ArticleName("Advanced C++"))->tags().empty());
}

BOOST_AUTO_TEST_CASE(traverse_visits_articles_in_name_order)
{
  ArticleRepo repo;
  addArticle(repo, "Move Semantics", "cpp");
  addArticle(repo, "Go Basics", "go");
  addArticle(repo, "algorithms 101", "cs");
  addArticle(repo, "Advanced C++", "cpp");
  std::vector< std::string > expected = {"Advanced C++", "Go Basics", "Move Semantics", "algorithms 101"};
  BOOST_CHECK(collectNames(repo) == expected);
}

BOOST_AUTO_TEST_CASE(traverse_categories_in_ascending_order_with_counts)
{
  ArticleRepo repo;
  addArticle(repo, "Move Semantics", "cpp");
  addArticle(repo, "Go Basics", "go");
  addArticle(repo, "Advanced C++", "cpp");
  addArticle(repo, "Graphs", "algorithms");
  std::vector< CategoryEntry > expected = {{"algorithms", 1}, {"cpp", 2}, {"go", 1}};
  BOOST_CHECK(collectCategories(repo) == expected);
}

BOOST_AUTO_TEST_CASE(table_key_matches_article_name)
{
  ArticleRepo repo;
  addArticle(repo, "Advanced C++", "cpp");
  addArticle(repo, "Go Basics", "go");
  const ArticleRepo &cref = repo;
  bool consistent = true;
  cref.traverse(
      [&cref, &consistent](const Article &article)
      {
        if (cref.find(article.name()) != &article)
        {
          consistent = false;
        }
      });
  BOOST_CHECK(consistent);
}

BOOST_AUTO_TEST_CASE(category_counts_match_articles)
{
  ArticleRepo repo;
  std::vector< std::string > names = makeShuffledNames("article", 60, 7);
  for (std::size_t i = 0; i < names.size(); ++i)
  {
    std::string category = (i % 3 == 0) ? "cpp" : ((i % 3 == 1) ? "go" : "rust");
    addArticle(repo, names[i], category);
  }
  for (std::size_t i = 0; i < names.size(); i += 2)
  {
    repo.erase(ArticleName(names[i]));
  }
  std::size_t expectedCpp = 0;
  std::size_t expectedGo = 0;
  std::size_t expectedRust = 0;
  repo.traverse(
      [&](const Article &article)
      {
        const std::string &value = article.category().value();
        if (value == "cpp")
        {
          ++expectedCpp;
        }
        else if (value == "go")
        {
          ++expectedGo;
        }
        else
        {
          ++expectedRust;
        }
      });
  BOOST_CHECK(repo.size() == 30);
  BOOST_CHECK(repo.categoryCount(Category("cpp")) == expectedCpp);
  BOOST_CHECK(repo.categoryCount(Category("go")) == expectedGo);
  BOOST_CHECK(repo.categoryCount(Category("rust")) == expectedRust);
}

BOOST_AUTO_TEST_CASE(survives_rehash)
{
  ArticleRepo repo;
  std::vector< std::string > names = makeShuffledNames("article", 200, 73);
  for (const std::string &name : names)
  {
    BOOST_CHECK(addArticle(repo, name, "cs"));
  }
  BOOST_CHECK(repo.size() == 200);
  BOOST_CHECK(repo.categoryCount(Category("cs")) == 200);
  for (const std::string &name : names)
  {
    BOOST_CHECK(repo.find(ArticleName(name)) != nullptr);
  }
  std::vector< std::string > traversed = collectNames(repo);
  BOOST_CHECK(traversed.size() == 200);
  BOOST_CHECK(std::is_sorted(traversed.begin(), traversed.end()));
}

BOOST_AUTO_TEST_CASE(clear_removes_articles_and_categories)
{
  ArticleRepo repo;
  addArticle(repo, "Advanced C++", "cpp");
  addArticle(repo, "Go Basics", "go");
  repo.clear();
  BOOST_CHECK(repo.empty());
  BOOST_CHECK(repo.find(ArticleName("Advanced C++")) == nullptr);
  BOOST_CHECK(collectCategories(repo).empty());
  BOOST_CHECK(repo.categoryCount(Category("cpp")) == 0);
  BOOST_CHECK(addArticle(repo, "Advanced C++", "cpp"));
}

BOOST_AUTO_TEST_CASE(swap_exchanges_articles_and_categories)
{
  ArticleRepo lhs;
  addArticle(lhs, "Advanced C++", "cpp");
  ArticleRepo rhs;
  addArticle(rhs, "Go Basics", "go");
  addArticle(rhs, "Graphs", "algorithms");
  lhs.swap(rhs);
  std::vector< std::string > expectedLhs = {"Go Basics", "Graphs"};
  std::vector< CategoryEntry > expectedRhsCategories = {{"cpp", 1}};
  BOOST_CHECK(collectNames(lhs) == expectedLhs);
  BOOST_CHECK(collectCategories(rhs) == expectedRhsCategories);
  BOOST_CHECK(lhs.categoryCount(Category("cpp")) == 0);
  BOOST_CHECK(lhs.categoryCount(Category("go")) == 1);
}

BOOST_AUTO_TEST_CASE(copy_is_independent)
{
  ArticleRepo original;
  addArticle(original, "Advanced C++", "cpp");
  ArticleRepo copy = original;
  addArticle(copy, "Move Semantics", "cpp");
  copy.find(ArticleName("Advanced C++"))->addTag(Tag("templates"));
  copy.erase(ArticleName("Advanced C++"));
  BOOST_CHECK(original.size() == 1);
  BOOST_CHECK(original.categoryCount(Category("cpp")) == 1);
  BOOST_CHECK(original.find(ArticleName("Advanced C++"))->tags().empty());
  BOOST_CHECK(copy.categoryCount(Category("cpp")) == 1);
}

BOOST_AUTO_TEST_SUITE_END()
