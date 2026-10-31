#include "Entities.hpp"
#include <boost/test/unit_test.hpp>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

using namespace khasnulin;

namespace
{
  // Гарантии типов имён проверяются на этапе компиляции.
  static_assert(std::is_constructible< UserName, std::string >::value,
                "UserName must be constructible from string");
  static_assert(!std::is_convertible< std::string, UserName >::value,
                "UserName constructor must be explicit");
  static_assert(!std::is_convertible< const char *, ArticleName >::value,
                "ArticleName constructor must be explicit");
  static_assert(!std::is_convertible< UserName, std::string >::value,
                "UserName must not convert to string implicitly");
  static_assert(!std::is_default_constructible< UserName >::value,
                "UserName must not be default constructible outside entities");
  static_assert(!std::is_default_constructible< ArticleName >::value,
                "ArticleName must not be default constructible outside entities");
  static_assert(!std::is_constructible< ArticleName, UserName >::value,
                "ArticleName must not be constructible from UserName");
  static_assert(!std::is_constructible< UserName, ArticleName >::value,
                "UserName must not be constructible from ArticleName");
  static_assert(!std::is_assignable< UserName &, ArticleName >::value,
                "ArticleName must not be assignable to UserName");
  static_assert(!std::is_default_constructible< Category >::value,
                "Category must not be default constructible outside Article");

  std::vector< std::string > collectTags(const TagSet &tags)
  {
    std::vector< std::string > result;
    for (auto it = tags.begin(); it != tags.end(); ++it)
    {
      result.push_back(it->first.value());
    }
    return result;
  }

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

  bool isZeroLatent(const Latent &latent)
  {
    return isSameLatent(latent, Latent{});
  }
}

BOOST_AUTO_TEST_SUITE(normalize_label_tests)

BOOST_AUTO_TEST_CASE(lowercases_letters)
{
  BOOST_CHECK(normalizeLabel("CPP") == "cpp");
}

BOOST_AUTO_TEST_CASE(trims_edge_spaces)
{
  BOOST_CHECK(normalizeLabel("  cpp  ") == "cpp");
}

BOOST_AUTO_TEST_CASE(collapses_inner_spaces)
{
  BOOST_CHECK(normalizeLabel("machine   learning") == "machine learning");
}

BOOST_AUTO_TEST_CASE(applies_all_rules_together)
{
  BOOST_CHECK(normalizeLabel("  Machine   Learning ") == "machine learning");
}

BOOST_AUTO_TEST_CASE(keeps_digits_and_special_chars)
{
  BOOST_CHECK(normalizeLabel("C++17") == "c++17");
  BOOST_CHECK(normalizeLabel("move-semantics") == "move-semantics");
  BOOST_CHECK(normalizeLabel("c#") == "c#");
  BOOST_CHECK(normalizeLabel("web 3") == "web 3");
}

BOOST_AUTO_TEST_CASE(accepts_single_letter)
{
  BOOST_CHECK(normalizeLabel("a") == "a");
}

BOOST_AUTO_TEST_CASE(is_idempotent)
{
  std::string once = normalizeLabel("  Machine   Learning ");
  BOOST_CHECK(normalizeLabel(once) == once);
}

BOOST_AUTO_TEST_CASE(throws_on_empty_and_spaces_only)
{
  BOOST_CHECK_THROW(normalizeLabel(""), std::invalid_argument);
  BOOST_CHECK_THROW(normalizeLabel("    "), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(throws_without_letters)
{
  BOOST_CHECK_THROW(normalizeLabel("123"), std::invalid_argument);
  BOOST_CHECK_THROW(normalizeLabel("+-~"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(throws_on_non_ascii)
{
  BOOST_CHECK_THROW(normalizeLabel("тег"), std::invalid_argument);
  BOOST_CHECK_THROW(normalizeLabel("cppтег"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(throws_on_quote)
{
  BOOST_CHECK_THROW(normalizeLabel("c\"pp"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(throws_on_control_chars)
{
  BOOST_CHECK_THROW(normalizeLabel("c\tpp"), std::invalid_argument);
  BOOST_CHECK_THROW(normalizeLabel("c\npp"), std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(normalize_text_tests)

BOOST_AUTO_TEST_CASE(preserves_case)
{
  BOOST_CHECK(normalizeText("  Advanced   C++ ") == "Advanced C++");
}

BOOST_AUTO_TEST_CASE(is_idempotent)
{
  std::string once = normalizeText("  Advanced   C++ ");
  BOOST_CHECK(normalizeText(once) == once);
}

BOOST_AUTO_TEST_CASE(throws_on_invalid_input)
{
  BOOST_CHECK_THROW(normalizeText("   "), std::invalid_argument);
  BOOST_CHECK_THROW(normalizeText("c\"pp"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(throws_without_letters)
{
  BOOST_CHECK_THROW(normalizeText("2024"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(throws_on_non_ascii)
{
  BOOST_CHECK_THROW(normalizeText("Роман"), std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(name_tests)

BOOST_AUTO_TEST_CASE(stores_normalized_value_preserving_case)
{
  UserName name("  Roman   Khasnulin ");
  BOOST_CHECK(name.value() == "Roman Khasnulin");
}

BOOST_AUTO_TEST_CASE(article_name_normalizes_too)
{
  ArticleName name("  Advanced   C++ ");
  BOOST_CHECK(name.value() == "Advanced C++");
}

BOOST_AUTO_TEST_CASE(throws_on_invalid_value)
{
  BOOST_CHECK_THROW(UserName(""), std::invalid_argument);
  BOOST_CHECK_THROW(UserName("    "), std::invalid_argument);
  BOOST_CHECK_THROW(UserName("123"), std::invalid_argument);
  BOOST_CHECK_THROW(UserName("Ro\"man"), std::invalid_argument);
  BOOST_CHECK_THROW(UserName("Роман"), std::invalid_argument);
  BOOST_CHECK_THROW(UserName("Ro\tman"), std::invalid_argument);
  BOOST_CHECK_THROW(ArticleName("\"quoted\""), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(equal_after_normalization)
{
  BOOST_CHECK(UserName(" Roman   K ") == UserName("Roman K"));
  BOOST_CHECK((UserName("Roman") == UserName("Alex")) == false);
}

BOOST_AUTO_TEST_CASE(equality_is_case_sensitive)
{
  BOOST_CHECK((UserName("Roman") == UserName("roman")) == false);
  BOOST_CHECK((ArticleName("Advanced C++") == ArticleName("advanced c++")) == false);
}

BOOST_AUTO_TEST_CASE(is_idempotent)
{
  UserName once("  Roman   Khasnulin ");
  UserName twice(once.value());
  BOOST_CHECK(twice == once);
}

BOOST_AUTO_TEST_CASE(copy_keeps_value)
{
  ArticleName original("Advanced C++");
  ArticleName copy = original;
  BOOST_CHECK(copy == original);
  BOOST_CHECK(copy.value() == "Advanced C++");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(tag_tests)

BOOST_AUTO_TEST_CASE(stores_normalized_value)
{
  Tag tag("  Move   Semantics ");
  BOOST_CHECK(tag.value() == "move semantics");
}

BOOST_AUTO_TEST_CASE(throws_on_invalid_value)
{
  BOOST_CHECK_THROW(Tag("   "), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(equal_after_normalization)
{
  BOOST_CHECK(Tag("CPP") == Tag(" cpp "));
  BOOST_CHECK((Tag("cpp") == Tag("go")) == false);
}

BOOST_AUTO_TEST_CASE(less_compares_values)
{
  BOOST_CHECK(Tag("a") < Tag("b"));
  BOOST_CHECK((Tag("b") < Tag("a")) == false);
  BOOST_CHECK((Tag("a") < Tag("A")) == false);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(category_tests)

BOOST_AUTO_TEST_CASE(stores_normalized_value)
{
  Category category("  AI  ");
  BOOST_CHECK(category.value() == "ai");
}

BOOST_AUTO_TEST_CASE(rejects_difficulty_names)
{
  BOOST_CHECK_THROW(Category("advanced"), std::invalid_argument);
  BOOST_CHECK_THROW(Category("Beginner"), std::invalid_argument);
  BOOST_CHECK_THROW(Category("  intermediate "), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(accepts_difficulty_word_inside)
{
  Category category("Advanced CPP");
  BOOST_CHECK(category.value() == "advanced cpp");
}

BOOST_AUTO_TEST_CASE(throws_on_invalid_label)
{
  BOOST_CHECK_THROW(Category("   "), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(equal_after_normalization)
{
  BOOST_CHECK(Category("AI") == Category(" ai "));
  BOOST_CHECK((Category("ai") == Category("ml")) == false);
}

BOOST_AUTO_TEST_CASE(less_compares_values)
{
  BOOST_CHECK(Category("ai") < Category("ml"));
  BOOST_CHECK((Category("ml") < Category("ai")) == false);
  BOOST_CHECK((Category("ai") < Category("AI")) == false);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(difficulty_tests)

BOOST_AUTO_TEST_CASE(parse_accepts_unnormalized_input)
{
  BOOST_CHECK(parseDifficulty("Advanced") == Difficulty::advanced);
  BOOST_CHECK(parseDifficulty("  beginner ") == Difficulty::beginner);
  BOOST_CHECK(parseDifficulty("INTERMEDIATE") == Difficulty::intermediate);
}

BOOST_AUTO_TEST_CASE(parse_throws_on_unknown)
{
  BOOST_CHECK_THROW(parseDifficulty("expert"), std::invalid_argument);
  BOOST_CHECK_THROW(parseDifficulty(""), std::invalid_argument);
  BOOST_CHECK_THROW(parseDifficulty("adv anced"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(to_string_returns_canonical_names)
{
  BOOST_CHECK(std::string(toString(Difficulty::beginner)) == "beginner");
  BOOST_CHECK(std::string(toString(Difficulty::intermediate)) == "intermediate");
  BOOST_CHECK(std::string(toString(Difficulty::advanced)) == "advanced");
}

BOOST_AUTO_TEST_CASE(parse_and_to_string_round_trip)
{
  Difficulty all[] = {Difficulty::beginner, Difficulty::intermediate, Difficulty::advanced};
  for (Difficulty d : all)
  {
    BOOST_CHECK(parseDifficulty(toString(d)) == d);
  }
}

BOOST_AUTO_TEST_CASE(is_difficulty_name_matches_only_canonical_names)
{
  BOOST_CHECK(isDifficultyName("beginner"));
  BOOST_CHECK(isDifficultyName("intermediate"));
  BOOST_CHECK(isDifficultyName("advanced"));
  BOOST_CHECK(isDifficultyName("expert") == false);
  BOOST_CHECK(isDifficultyName("advanced cpp") == false);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(user_tests)

BOOST_AUTO_TEST_CASE(constructs_with_name_and_latent)
{
  Latent latent = makeLatent(0.1);
  User user(UserName("roman"), latent);
  BOOST_CHECK(user.name() == UserName("roman"));
  BOOST_CHECK(user.preferences().empty());
  BOOST_CHECK(isSameLatent(user.latent(), latent));
}

BOOST_AUTO_TEST_CASE(default_constructed_is_empty_placeholder)
{
  User user;
  BOOST_CHECK(user.name().value().empty());
  BOOST_CHECK(user.preferences().empty());
  BOOST_CHECK(isZeroLatent(user.latent()));
}

BOOST_AUTO_TEST_CASE(add_preference_reports_new_tag)
{
  User user(UserName("roman"), makeLatent(0.1));
  BOOST_CHECK(user.addPreference(Tag("cpp")));
  BOOST_CHECK(user.preferences().size() == 1);
}

BOOST_AUTO_TEST_CASE(add_preference_ignores_duplicate)
{
  User user(UserName("roman"), makeLatent(0.1));
  user.addPreference(Tag("cpp"));
  BOOST_CHECK(user.addPreference(Tag(" CPP ")) == false);
  BOOST_CHECK(user.preferences().size() == 1);
}

BOOST_AUTO_TEST_CASE(preferences_are_sorted)
{
  User user(UserName("roman"), makeLatent(0.1));
  user.addPreference(Tag("rust"));
  user.addPreference(Tag("cpp"));
  user.addPreference(Tag("go"));
  std::vector< std::string > expected = {"cpp", "go", "rust"};
  BOOST_CHECK(collectTags(user.preferences()) == expected);
}
BOOST_AUTO_TEST_CASE(less_compares_values_case_sensitively)
{
  BOOST_CHECK(UserName("alex") < UserName("roman"));
  BOOST_CHECK((UserName("roman") < UserName("alex")) == false);
  BOOST_CHECK(UserName("Roman") < UserName("roman"));
  BOOST_CHECK((UserName("roman") < UserName("Roman")) == false);
  BOOST_CHECK((UserName("roman") < UserName("roman")) == false);
}

BOOST_AUTO_TEST_CASE(latent_is_mutable)
{
  User user(UserName("roman"), makeLatent(0.1));
  user.latent().v[0] = 0.5;
  const User &cref = user;
  BOOST_CHECK(cref.latent().v[0] == 0.5);
}

BOOST_AUTO_TEST_CASE(copy_is_independent)
{
  User original(UserName("roman"), makeLatent(0.1));
  original.addPreference(Tag("cpp"));
  User copy = original;
  copy.addPreference(Tag("go"));
  copy.latent().v[0] = 0.9;
  BOOST_CHECK(copy.name() == original.name());
  BOOST_CHECK(original.preferences().size() == 1);
  BOOST_CHECK(copy.preferences().size() == 2);
  BOOST_CHECK(isSameLatent(original.latent(), makeLatent(0.1)));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(article_tests)

BOOST_AUTO_TEST_CASE(constructs_with_all_fields)
{
  Latent latent = makeLatent(0.2);
  Article article(ArticleName("Advanced C++"), Category("cpp"), Difficulty::advanced, latent);
  BOOST_CHECK(article.name() == ArticleName("Advanced C++"));
  BOOST_CHECK(article.category() == Category("cpp"));
  BOOST_CHECK(article.difficulty() == Difficulty::advanced);
  BOOST_CHECK(article.tags().empty());
  BOOST_CHECK(isSameLatent(article.latent(), latent));
}

BOOST_AUTO_TEST_CASE(default_constructed_is_empty_placeholder)
{
  Article article;
  BOOST_CHECK(article.name().value().empty());
  BOOST_CHECK(article.category().value().empty());
  BOOST_CHECK(article.difficulty() == Difficulty::beginner);
  BOOST_CHECK(article.tags().empty());
  BOOST_CHECK(isZeroLatent(article.latent()));
}

BOOST_AUTO_TEST_CASE(add_tag_reports_new_and_duplicate)
{
  Article article(ArticleName("Advanced C++"), Category("cpp"), Difficulty::advanced, makeLatent(0.2));
  BOOST_CHECK(article.addTag(Tag("templates")));
  BOOST_CHECK(article.addTag(Tag("Templates")) == false);
  BOOST_CHECK(article.tags().size() == 1);
}

BOOST_AUTO_TEST_CASE(tags_are_sorted)
{
  Article article(ArticleName("Advanced C++"), Category("cpp"), Difficulty::advanced, makeLatent(0.2));
  article.addTag(Tag("templates"));
  article.addTag(Tag("cpp"));
  article.addTag(Tag("move-semantics"));
  std::vector< std::string > expected = {"cpp", "move-semantics", "templates"};
  BOOST_CHECK(collectTags(article.tags()) == expected);
}

BOOST_AUTO_TEST_CASE(copy_is_independent)
{
  Article original(ArticleName("Advanced C++"), Category("cpp"), Difficulty::advanced, makeLatent(0.2));
  original.addTag(Tag("cpp"));
  Article copy = original;
  copy.addTag(Tag("templates"));
  BOOST_CHECK(copy.name() == original.name());
  BOOST_CHECK(copy.category() == original.category());
  BOOST_CHECK(original.tags().size() == 1);
  BOOST_CHECK(copy.tags().size() == 2);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(rating_tests)

BOOST_AUTO_TEST_CASE(accepts_bounds)
{
  BOOST_CHECK(Rating(Rating::minRating).value() == 1);
  BOOST_CHECK(Rating(Rating::maxRating).value() == 10);
}

BOOST_AUTO_TEST_CASE(accepts_value_inside_range)
{
  BOOST_CHECK(Rating(7).value() == 7);
}

BOOST_AUTO_TEST_CASE(throws_outside_range)
{
  BOOST_CHECK_THROW(Rating(0), std::invalid_argument);
  BOOST_CHECK_THROW(Rating(11), std::invalid_argument);
  BOOST_CHECK_THROW(Rating(-5), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(equality_compares_values)
{
  BOOST_CHECK(Rating(5) == Rating(5));
  BOOST_CHECK((Rating(5) == Rating(6)) == false);
}

BOOST_AUTO_TEST_CASE(copy_keeps_value)
{
  Rating original(9);
  Rating copy = original;
  BOOST_CHECK(copy == original);
  copy = Rating(3);
  BOOST_CHECK(original.value() == 9);
}

BOOST_AUTO_TEST_SUITE_END()
