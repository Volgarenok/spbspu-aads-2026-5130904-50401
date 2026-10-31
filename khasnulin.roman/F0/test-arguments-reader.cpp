#include "ArgumentReader.hpp"
#include <boost/test/unit_test.hpp>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

using namespace khasnulin;

namespace
{
  Token plain(const std::string &text)
  {
    Token token;
    token.text = text;
    token.isQuoted = false;
    return token;
  }

  Token quoted(const std::string &text)
  {
    Token token;
    token.text = text;
    token.isQuoted = true;
    return token;
  }

  Vector< Token > makeTokens(const std::vector< Token > &source)
  {
    Vector< Token > result;
    for (const Token &token : source)
    {
      result.pushBack(token);
    }
    return result;
  }

  Vector< Token > withArgument(const Token &argument)
  {
    return makeTokens({plain("command"), argument});
  }

  std::vector< std::string > collectTags(const TagSet &tags)
  {
    std::vector< std::string > result;
    for (auto it = tags.begin(); it != tags.end(); ++it)
    {
      result.push_back(it->first.value());
    }
    return result;
  }

  std::string maxSizeText()
  {
    return std::to_string(std::numeric_limits< std::size_t >::max());
  }

  UserName readUserName(const Token &argument)
  {
    Vector< Token > tokens = withArgument(argument);
    ArgumentReader reader(tokens);
    return reader.userName();
  }

  ArticleName readArticleName(const Token &argument)
  {
    Vector< Token > tokens = withArgument(argument);
    ArgumentReader reader(tokens);
    return reader.articleName();
  }

  Category readCategory(const Token &argument)
  {
    Vector< Token > tokens = withArgument(argument);
    ArgumentReader reader(tokens);
    return reader.category();
  }

  Difficulty readDifficulty(const Token &argument)
  {
    Vector< Token > tokens = withArgument(argument);
    ArgumentReader reader(tokens);
    return reader.difficulty();
  }

  Rating readRating(const Token &argument)
  {
    Vector< Token > tokens = withArgument(argument);
    ArgumentReader reader(tokens);
    return reader.rating();
  }

  std::size_t readLimit(const Token &argument)
  {
    Vector< Token > tokens = withArgument(argument);
    ArgumentReader reader(tokens);
    return reader.limit();
  }

  bool checkNextIsDifficulty(const Token &argument)
  {
    Vector< Token > tokens = withArgument(argument);
    ArgumentReader reader(tokens);
    return reader.nextIsDifficulty();
  }
}

static_assert(std::is_constructible< ArgumentReader, const Vector< Token > & >::value,
              "reader must be constructible from lvalue tokens");
static_assert(!std::is_constructible< ArgumentReader, Vector< Token > && >::value,
              "reader must not be constructible from temporary tokens");
static_assert(!std::is_convertible< const Vector< Token > &, ArgumentReader >::value,
              "reader constructor must be explicit");

BOOST_AUTO_TEST_SUITE(parse_unsigned_number_tests)

BOOST_AUTO_TEST_CASE(parses_single_digits)
{
  BOOST_CHECK(parseUnsignedNumber("0") == 0);
  BOOST_CHECK(parseUnsignedNumber("7") == 7);
}

BOOST_AUTO_TEST_CASE(parses_multi_digit_numbers)
{
  BOOST_CHECK(parseUnsignedNumber("10") == 10);
  BOOST_CHECK(parseUnsignedNumber("1234567890") == 1234567890);
}

BOOST_AUTO_TEST_CASE(accepts_leading_zeros)
{
  BOOST_CHECK(parseUnsignedNumber("007") == 7);
  BOOST_CHECK(parseUnsignedNumber("000") == 0);
}

BOOST_AUTO_TEST_CASE(parses_max_value)
{
  BOOST_CHECK(parseUnsignedNumber(maxSizeText()) == std::numeric_limits< std::size_t >::max());
}

BOOST_AUTO_TEST_CASE(throws_on_overflow)
{
  BOOST_CHECK_THROW(parseUnsignedNumber(maxSizeText() + "0"), std::invalid_argument);
  BOOST_CHECK_THROW(parseUnsignedNumber("999999999999999999999999999999"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(throws_on_empty)
{
  BOOST_CHECK_THROW(parseUnsignedNumber(""), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(throws_on_sign)
{
  BOOST_CHECK_THROW(parseUnsignedNumber("+5"), std::invalid_argument);
  BOOST_CHECK_THROW(parseUnsignedNumber("-5"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(throws_on_non_digits)
{
  BOOST_CHECK_THROW(parseUnsignedNumber("5a"), std::invalid_argument);
  BOOST_CHECK_THROW(parseUnsignedNumber("a5"), std::invalid_argument);
  BOOST_CHECK_THROW(parseUnsignedNumber("1.5"), std::invalid_argument);
  BOOST_CHECK_THROW(parseUnsignedNumber(" 5"), std::invalid_argument);
  BOOST_CHECK_THROW(parseUnsignedNumber("5 "), std::invalid_argument);
  BOOST_CHECK_THROW(parseUnsignedNumber("ten"), std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(argument_reader_position_tests)

BOOST_AUTO_TEST_CASE(skips_command_name)
{
  Vector< Token > tokens = makeTokens({plain("delete-user"), plain("roman")});
  ArgumentReader reader(tokens);
  BOOST_CHECK(reader.remaining() == 1);
  BOOST_CHECK(reader.userName().value() == "roman");
}

BOOST_AUTO_TEST_CASE(no_arguments_after_command_name)
{
  Vector< Token > tokens = makeTokens({plain("show-tags")});
  ArgumentReader reader(tokens);
  BOOST_CHECK(reader.remaining() == 0);
  BOOST_CHECK_NO_THROW(reader.finish());
}

BOOST_AUTO_TEST_CASE(remaining_decreases_after_each_read)
{
  Vector< Token > tokens =
      makeTokens({plain("rate-article"), plain("roman"), quoted("Advanced C++"), plain("7")});
  ArgumentReader reader(tokens);
  BOOST_CHECK(reader.remaining() == 3);
  reader.userName();
  BOOST_CHECK(reader.remaining() == 2);
  reader.articleName();
  BOOST_CHECK(reader.remaining() == 1);
  reader.rating();
  BOOST_CHECK(reader.remaining() == 0);
}

BOOST_AUTO_TEST_CASE(throws_when_first_argument_missing)
{
  Vector< Token > tokens = makeTokens({plain("delete-user")});
  ArgumentReader reader(tokens);
  BOOST_CHECK_THROW(reader.userName(), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(throws_when_later_argument_missing)
{
  Vector< Token > tokens = makeTokens({plain("rate-article"), plain("roman"), quoted("Advanced C++")});
  ArgumentReader reader(tokens);
  reader.userName();
  reader.articleName();
  BOOST_CHECK_THROW(reader.rating(), std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(argument_reader_commands_tests)

BOOST_AUTO_TEST_CASE(reads_rate_article_arguments)
{
  Vector< Token > tokens = makeTokens(
      {plain("rate-article"), plain("olga-ivanova"), quoted("Stocks fall, bonds rise"), plain("10")});
  ArgumentReader reader(tokens);
  BOOST_CHECK(reader.userName().value() == "olga-ivanova");
  BOOST_CHECK(reader.articleName().value() == "Stocks fall, bonds rise");
  BOOST_CHECK(reader.rating().value() == 10);
  BOOST_CHECK_NO_THROW(reader.finish());
}

BOOST_AUTO_TEST_CASE(reads_add_article_arguments)
{
  Vector< Token > tokens =
      makeTokens({plain("add-article"), quoted("Stocks fall, bonds rise"), plain("finance"),
                  plain("intermediate"), plain("newsus"), plain("markets")});
  ArgumentReader reader(tokens);
  BOOST_CHECK(reader.articleName().value() == "Stocks fall, bonds rise");
  BOOST_CHECK(reader.category().value() == "finance");
  BOOST_CHECK(reader.difficulty() == Difficulty::intermediate);
  std::vector< std::string > expectedTags = {"markets", "newsus"};
  BOOST_CHECK(collectTags(reader.restTags()) == expectedTags);
  BOOST_CHECK_NO_THROW(reader.finish());
}

BOOST_AUTO_TEST_CASE(reads_recommend_with_two_filters)
{
  Vector< Token > tokens =
      makeTokens({plain("recommend"), plain("roman"), plain("finance"), plain("advanced")});
  ArgumentReader reader(tokens);
  reader.userName();
  BOOST_CHECK(reader.remaining() == 2);
  BOOST_CHECK(reader.category().value() == "finance");
  BOOST_CHECK(reader.difficulty() == Difficulty::advanced);
  BOOST_CHECK_NO_THROW(reader.finish());
}

BOOST_AUTO_TEST_CASE(reads_optional_limit)
{
  Vector< Token > withLimit = makeTokens({plain("show-users"), plain("5")});
  ArgumentReader limited(withLimit);
  BOOST_CHECK(limited.remaining() == 1);
  BOOST_CHECK(limited.limit() == 5);
  BOOST_CHECK_NO_THROW(limited.finish());

  Vector< Token > withoutLimit = makeTokens({plain("show-users")});
  ArgumentReader unlimited(withoutLimit);
  BOOST_CHECK(unlimited.remaining() == 0);
  BOOST_CHECK_NO_THROW(unlimited.finish());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(argument_reader_user_name_tests)

BOOST_AUTO_TEST_CASE(accepts_plain_token)
{
  BOOST_CHECK(readUserName(plain("olga-ivanova")).value() == "olga-ivanova");
}

BOOST_AUTO_TEST_CASE(preserves_case)
{
  BOOST_CHECK(readUserName(plain("Roman")).value() == "Roman");
}

BOOST_AUTO_TEST_CASE(throws_on_quoted_token)
{
  BOOST_CHECK_THROW(readUserName(quoted("roman")), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(throws_on_invalid_name)
{
  BOOST_CHECK_THROW(readUserName(plain("")), std::invalid_argument);
  BOOST_CHECK_THROW(readUserName(plain("ro\tman")), std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(argument_reader_article_name_tests)

BOOST_AUTO_TEST_CASE(accepts_quoted_token)
{
  BOOST_CHECK(readArticleName(quoted("Advanced C++")).value() == "Advanced C++");
}

BOOST_AUTO_TEST_CASE(accepts_single_word_in_quotes)
{
  BOOST_CHECK(readArticleName(quoted("Python")).value() == "Python");
}

BOOST_AUTO_TEST_CASE(normalizes_quoted_text)
{
  BOOST_CHECK(readArticleName(quoted("  Advanced   C++ ")).value() == "Advanced C++");
}

BOOST_AUTO_TEST_CASE(keeps_special_chars)
{
  BOOST_CHECK(readArticleName(quoted("Who's next: $5, a&b? C:\\")).value() == "Who's next: $5, a&b? C:\\");
}

BOOST_AUTO_TEST_CASE(throws_on_plain_token)
{
  BOOST_CHECK_THROW(readArticleName(plain("Python")), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(throws_on_empty_quoted_token)
{
  BOOST_CHECK_THROW(readArticleName(quoted("")), std::invalid_argument);
  BOOST_CHECK_THROW(readArticleName(quoted("   ")), std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(argument_reader_category_tests)

BOOST_AUTO_TEST_CASE(accepts_plain_token)
{
  BOOST_CHECK(readCategory(plain("finance")).value() == "finance");
}

BOOST_AUTO_TEST_CASE(throws_on_quoted_token)
{
  BOOST_CHECK_THROW(readCategory(quoted("finance")), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(throws_on_difficulty_name)
{
  BOOST_CHECK_THROW(readCategory(plain("advanced")), std::invalid_argument);
  BOOST_CHECK_THROW(readCategory(plain("Advanced")), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(throws_on_invalid_label)
{
  BOOST_CHECK_THROW(readCategory(plain("42")), std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(argument_reader_difficulty_tests)

BOOST_AUTO_TEST_CASE(accepts_canonical_names)
{
  BOOST_CHECK(readDifficulty(plain("beginner")) == Difficulty::beginner);
  BOOST_CHECK(readDifficulty(plain("intermediate")) == Difficulty::intermediate);
  BOOST_CHECK(readDifficulty(plain("advanced")) == Difficulty::advanced);
}

BOOST_AUTO_TEST_CASE(accepts_unnormalized_name)
{
  BOOST_CHECK(readDifficulty(plain("Advanced")) == Difficulty::advanced);
}

BOOST_AUTO_TEST_CASE(throws_on_quoted_token)
{
  BOOST_CHECK_THROW(readDifficulty(quoted("advanced")), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(throws_on_unknown_name)
{
  BOOST_CHECK_THROW(readDifficulty(plain("expert")), std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(argument_reader_rating_tests)

BOOST_AUTO_TEST_CASE(accepts_bounds_and_middle)
{
  BOOST_CHECK(readRating(plain("1")).value() == 1);
  BOOST_CHECK(readRating(plain("5")).value() == 5);
  BOOST_CHECK(readRating(plain("10")).value() == 10);
}

BOOST_AUTO_TEST_CASE(accepts_leading_zeros)
{
  BOOST_CHECK(readRating(plain("08")).value() == 8);
}

BOOST_AUTO_TEST_CASE(throws_out_of_range)
{
  BOOST_CHECK_THROW(readRating(plain("0")), std::invalid_argument);
  BOOST_CHECK_THROW(readRating(plain("11")), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(throws_on_value_that_wraps_in_int)
{
  BOOST_CHECK_THROW(readRating(plain("4294967301")), std::invalid_argument);
  BOOST_CHECK_THROW(readRating(plain(maxSizeText())), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(throws_on_quoted_token)
{
  BOOST_CHECK_THROW(readRating(quoted("5")), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(throws_on_non_number)
{
  BOOST_CHECK_THROW(readRating(plain("ten")), std::invalid_argument);
  BOOST_CHECK_THROW(readRating(plain("-1")), std::invalid_argument);
  BOOST_CHECK_THROW(readRating(plain("9.5")), std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(argument_reader_limit_tests)

BOOST_AUTO_TEST_CASE(accepts_positive_numbers)
{
  BOOST_CHECK(readLimit(plain("1")) == 1);
  BOOST_CHECK(readLimit(plain("1000000")) == 1000000);
}

BOOST_AUTO_TEST_CASE(throws_on_zero)
{
  BOOST_CHECK_THROW(readLimit(plain("0")), std::invalid_argument);
  BOOST_CHECK_THROW(readLimit(plain("000")), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(throws_on_quoted_token)
{
  BOOST_CHECK_THROW(readLimit(quoted("10")), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(throws_on_non_number)
{
  BOOST_CHECK_THROW(readLimit(plain("-3")), std::invalid_argument);
  BOOST_CHECK_THROW(readLimit(plain("all")), std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(argument_reader_rest_tags_tests)

BOOST_AUTO_TEST_CASE(empty_when_no_tokens_left)
{
  Vector< Token > tokens = makeTokens({plain("add-user"), plain("roman")});
  ArgumentReader reader(tokens);
  reader.userName();
  BOOST_CHECK(reader.restTags().empty());
  BOOST_CHECK_NO_THROW(reader.finish());
}

BOOST_AUTO_TEST_CASE(collects_tags_sorted)
{
  Vector< Token > tokens =
      makeTokens({plain("add-user"), plain("roman"), plain("rust"), plain("cpp"), plain("go")});
  ArgumentReader reader(tokens);
  reader.userName();
  std::vector< std::string > expected = {"cpp", "go", "rust"};
  BOOST_CHECK(collectTags(reader.restTags()) == expected);
}

BOOST_AUTO_TEST_CASE(consumes_all_remaining_tokens)
{
  Vector< Token > tokens = makeTokens({plain("add-user"), plain("roman"), plain("cpp"), plain("go")});
  ArgumentReader reader(tokens);
  reader.userName();
  reader.restTags();
  BOOST_CHECK(reader.remaining() == 0);
  BOOST_CHECK_NO_THROW(reader.finish());
}

BOOST_AUTO_TEST_CASE(removes_duplicates_after_normalization)
{
  Vector< Token > tokens =
      makeTokens({plain("add-user"), plain("roman"), plain("cpp"), plain("CPP"), plain("cpp")});
  ArgumentReader reader(tokens);
  reader.userName();
  std::vector< std::string > expected = {"cpp"};
  BOOST_CHECK(collectTags(reader.restTags()) == expected);
}

BOOST_AUTO_TEST_CASE(throws_on_any_invalid_tag)
{
  Vector< Token > tokens = makeTokens({plain("add-user"), plain("roman"), plain("cpp"), plain("123")});
  ArgumentReader reader(tokens);
  reader.userName();
  BOOST_CHECK_THROW(reader.restTags(), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(throws_on_quoted_tag)
{
  Vector< Token > tokens = makeTokens({plain("add-user"), plain("roman"), quoted("cpp")});
  ArgumentReader reader(tokens);
  reader.userName();
  BOOST_CHECK_THROW(reader.restTags(), std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(argument_reader_finish_tests)

BOOST_AUTO_TEST_CASE(passes_when_all_arguments_read)
{
  Vector< Token > tokens = makeTokens({plain("delete-user"), plain("roman")});
  ArgumentReader reader(tokens);
  reader.userName();
  BOOST_CHECK_NO_THROW(reader.finish());
}

BOOST_AUTO_TEST_CASE(throws_on_extra_argument)
{
  Vector< Token > tokens = makeTokens({plain("delete-user"), plain("roman"), plain("extra")});
  ArgumentReader reader(tokens);
  reader.userName();
  BOOST_CHECK_THROW(reader.finish(), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(throws_on_argument_for_command_without_arguments)
{
  Vector< Token > tokens = makeTokens({plain("show-tags"), plain("extra")});
  ArgumentReader reader(tokens);
  BOOST_CHECK_THROW(reader.finish(), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(throws_on_extra_optional_argument)
{
  Vector< Token > tokens = makeTokens({plain("show-users"), plain("5"), plain("7")});
  ArgumentReader reader(tokens);
  reader.limit();
  BOOST_CHECK_THROW(reader.finish(), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(does_not_move_position)
{
  Vector< Token > tokens = makeTokens({plain("delete-user"), plain("roman"), plain("extra")});
  ArgumentReader reader(tokens);
  reader.userName();
  BOOST_CHECK_THROW(reader.finish(), std::invalid_argument);
  BOOST_CHECK(reader.remaining() == 1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(argument_reader_next_is_difficulty_tests)

BOOST_AUTO_TEST_CASE(true_for_canonical_names)
{
  BOOST_CHECK(checkNextIsDifficulty(plain("beginner")));
  BOOST_CHECK(checkNextIsDifficulty(plain("intermediate")));
  BOOST_CHECK(checkNextIsDifficulty(plain("advanced")));
}

BOOST_AUTO_TEST_CASE(true_for_unnormalized_name)
{
  BOOST_CHECK(checkNextIsDifficulty(plain("Advanced")));
}

BOOST_AUTO_TEST_CASE(false_for_category)
{
  BOOST_CHECK(checkNextIsDifficulty(plain("finance")) == false);
}

BOOST_AUTO_TEST_CASE(false_for_quoted_token)
{
  BOOST_CHECK(checkNextIsDifficulty(quoted("advanced")) == false);
}

BOOST_AUTO_TEST_CASE(false_for_invalid_label)
{
  BOOST_CHECK(checkNextIsDifficulty(plain("123")) == false);
  BOOST_CHECK(checkNextIsDifficulty(plain("")) == false);
}

BOOST_AUTO_TEST_CASE(false_when_no_tokens_left)
{
  Vector< Token > tokens = makeTokens({plain("recommend"), plain("roman")});
  ArgumentReader reader(tokens);
  reader.userName();
  BOOST_CHECK(reader.nextIsDifficulty() == false);
}

BOOST_AUTO_TEST_CASE(does_not_move_position)
{
  Vector< Token > tokens = makeTokens({plain("recommend"), plain("roman"), plain("advanced")});
  ArgumentReader reader(tokens);
  reader.userName();
  BOOST_CHECK(reader.nextIsDifficulty());
  BOOST_CHECK(reader.nextIsDifficulty());
  BOOST_CHECK(reader.remaining() == 1);
  BOOST_CHECK(reader.difficulty() == Difficulty::advanced);
}

BOOST_AUTO_TEST_CASE(guides_single_filter_choice)
{
  Vector< Token > byDifficulty = makeTokens({plain("recommend"), plain("roman"), plain("Advanced")});
  ArgumentReader difficultyReader(byDifficulty);
  difficultyReader.userName();
  BOOST_REQUIRE(difficultyReader.nextIsDifficulty());
  BOOST_CHECK(difficultyReader.difficulty() == Difficulty::advanced);
  BOOST_CHECK_NO_THROW(difficultyReader.finish());

  Vector< Token > byCategory = makeTokens({plain("recommend"), plain("roman"), plain("finance")});
  ArgumentReader categoryReader(byCategory);
  categoryReader.userName();
  BOOST_REQUIRE(categoryReader.nextIsDifficulty() == false);
  BOOST_CHECK(categoryReader.category().value() == "finance");
  BOOST_CHECK_NO_THROW(categoryReader.finish());
}

BOOST_AUTO_TEST_SUITE_END()
