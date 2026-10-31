#include "Tokenizer.hpp"
#include <boost/test/unit_test.hpp>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>

using namespace khasnulin;

namespace
{
  std::vector< std::string > collectTexts(const Vector< Token > &tokens)
  {
    std::vector< std::string > result;
    for (std::size_t i = 0; i < tokens.getSize(); ++i)
    {
      result.push_back(tokens[i].text);
    }
    return result;
  }

  std::vector< bool > collectQuotedFlags(const Vector< Token > &tokens)
  {
    std::vector< bool > result;
    for (std::size_t i = 0; i < tokens.getSize(); ++i)
    {
      result.push_back(tokens[i].isQuoted);
    }
    return result;
  }
}

BOOST_AUTO_TEST_SUITE(tokenize_blank_lines_tests)

BOOST_AUTO_TEST_CASE(empty_line_gives_no_tokens)
{
  BOOST_CHECK(tokenize("").isEmpty());
}

BOOST_AUTO_TEST_CASE(spaces_only_line_gives_no_tokens)
{
  BOOST_CHECK(tokenize("     ").isEmpty());
}

BOOST_AUTO_TEST_CASE(carriage_return_only_line_gives_no_tokens)
{
  BOOST_CHECK(tokenize("\r").isEmpty());
  BOOST_CHECK(tokenize("   \r").isEmpty());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(tokenize_plain_words_tests)

BOOST_AUTO_TEST_CASE(splits_words_by_single_space)
{
  Vector< Token > tokens = tokenize("add-user roman cpp");
  std::vector< std::string > expectedTexts = {"add-user", "roman", "cpp"};
  std::vector< bool > expectedFlags = {false, false, false};
  BOOST_CHECK(collectTexts(tokens) == expectedTexts);
  BOOST_CHECK(collectQuotedFlags(tokens) == expectedFlags);
}

BOOST_AUTO_TEST_CASE(single_word_line)
{
  Vector< Token > tokens = tokenize("show-tags");
  std::vector< std::string > expected = {"show-tags"};
  BOOST_CHECK(collectTexts(tokens) == expected);
  BOOST_CHECK(tokens[0].isQuoted == false);
}

BOOST_AUTO_TEST_CASE(skips_repeated_and_edge_spaces)
{
  Vector< Token > tokens = tokenize("   show-users    10   ");
  std::vector< std::string > expected = {"show-users", "10"};
  BOOST_CHECK(collectTexts(tokens) == expected);
}

BOOST_AUTO_TEST_CASE(keeps_special_chars_in_plain_words)
{
  Vector< Token > tokens = tokenize("add-user olga-ivanova c++17 c# a.b");
  std::vector< std::string > expected = {"add-user", "olga-ivanova", "c++17", "c#", "a.b"};
  BOOST_CHECK(collectTexts(tokens) == expected);
}

BOOST_AUTO_TEST_CASE(tab_is_not_separator)
{
  Vector< Token > tokens = tokenize("add-user\troman");
  std::vector< std::string > expected = {"add-user\troman"};
  BOOST_CHECK(collectTexts(tokens) == expected);
}

BOOST_AUTO_TEST_CASE(many_tokens_are_kept_in_order)
{
  std::string line = "add-user roman";
  std::vector< std::string > expected = {"add-user", "roman"};
  for (std::size_t i = 0; i < 100; ++i)
  {
    std::string tag = "tag" + std::to_string(i);
    line += " " + tag;
    expected.push_back(tag);
  }
  BOOST_CHECK(collectTexts(tokenize(line)) == expected);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(tokenize_quoted_tests)

BOOST_AUTO_TEST_CASE(quoted_token_keeps_inner_spaces)
{
  Vector< Token > tokens = tokenize("similar \"Advanced C++\"");
  std::vector< std::string > expectedTexts = {"similar", "Advanced C++"};
  std::vector< bool > expectedFlags = {false, true};
  BOOST_CHECK(collectTexts(tokens) == expectedTexts);
  BOOST_CHECK(collectQuotedFlags(tokens) == expectedFlags);
}

BOOST_AUTO_TEST_CASE(quoted_token_is_not_normalized)
{
  Vector< Token > tokens = tokenize("similar \"  Advanced   C++ \"");
  std::vector< std::string > expected = {"similar", "  Advanced   C++ "};
  BOOST_CHECK(collectTexts(tokens) == expected);
}

BOOST_AUTO_TEST_CASE(quoted_token_keeps_special_chars)
{
  Vector< Token > tokens = tokenize("similar \"Who's next: $5, a&b? 100%\"");
  std::vector< std::string > expected = {"similar", "Who's next: $5, a&b? 100%"};
  BOOST_CHECK(collectTexts(tokens) == expected);
}

BOOST_AUTO_TEST_CASE(single_word_in_quotes_is_quoted)
{
  Vector< Token > tokens = tokenize("similar \"Python\"");
  std::vector< bool > expectedFlags = {false, true};
  BOOST_CHECK(collectQuotedFlags(tokens) == expectedFlags);
  BOOST_CHECK(tokens[1].text == "Python");
}

BOOST_AUTO_TEST_CASE(backslash_is_ordinary_char)
{
  Vector< Token > tokens = tokenize("similar \"C:\\\" next");
  std::vector< std::string > expectedTexts = {"similar", "C:\\", "next"};
  std::vector< bool > expectedFlags = {false, true, false};
  BOOST_CHECK(collectTexts(tokens) == expectedTexts);
  BOOST_CHECK(collectQuotedFlags(tokens) == expectedFlags);
}

BOOST_AUTO_TEST_CASE(empty_quotes_give_empty_quoted_token)
{
  Vector< Token > tokens = tokenize("rate-article roman \"\" 5");
  std::vector< std::string > expectedTexts = {"rate-article", "roman", "", "5"};
  std::vector< bool > expectedFlags = {false, false, true, false};
  BOOST_CHECK(collectTexts(tokens) == expectedTexts);
  BOOST_CHECK(collectQuotedFlags(tokens) == expectedFlags);
}

BOOST_AUTO_TEST_CASE(adjacent_quoted_tokens_separated_by_space)
{
  Vector< Token > tokens = tokenize("\"a b\" \"c d\"");
  std::vector< std::string > expectedTexts = {"a b", "c d"};
  std::vector< bool > expectedFlags = {true, true};
  BOOST_CHECK(collectTexts(tokens) == expectedTexts);
  BOOST_CHECK(collectQuotedFlags(tokens) == expectedFlags);
}

BOOST_AUTO_TEST_CASE(quoted_token_at_line_start)
{
  Vector< Token > tokens = tokenize("\"add-user\" roman");
  std::vector< std::string > expectedTexts = {"add-user", "roman"};
  std::vector< bool > expectedFlags = {true, false};
  BOOST_CHECK(collectTexts(tokens) == expectedTexts);
  BOOST_CHECK(collectQuotedFlags(tokens) == expectedFlags);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(tokenize_seed_lines_tests)

BOOST_AUTO_TEST_CASE(add_article_seed_line)
{
  Vector< Token > tokens =
      tokenize("add-article \"Stocks fall, bonds rise\" finance intermediate markets newsus");
  std::vector< std::string > expectedTexts = {
      "add-article", "Stocks fall, bonds rise", "finance", "intermediate", "markets", "newsus"};
  std::vector< bool > expectedFlags = {false, true, false, false, false, false};
  BOOST_CHECK(collectTexts(tokens) == expectedTexts);
  BOOST_CHECK(collectQuotedFlags(tokens) == expectedFlags);
}

BOOST_AUTO_TEST_CASE(rate_article_seed_line)
{
  Vector< Token > tokens = tokenize("rate-article olga-ivanova \"Stocks fall, bonds rise\" 10");
  std::vector< std::string > expectedTexts = {"rate-article", "olga-ivanova", "Stocks fall, bonds rise",
                                              "10"};
  std::vector< bool > expectedFlags = {false, false, true, false};
  BOOST_CHECK(collectTexts(tokens) == expectedTexts);
  BOOST_CHECK(collectQuotedFlags(tokens) == expectedFlags);
}

BOOST_AUTO_TEST_CASE(strips_trailing_carriage_return)
{
  std::vector< std::string > plain = {"show-users"};
  BOOST_CHECK(collectTexts(tokenize("show-users\r")) == plain);

  Vector< Token > quoted = tokenize("similar \"A B\"\r");
  std::vector< std::string > expectedTexts = {"similar", "A B"};
  std::vector< bool > expectedFlags = {false, true};
  BOOST_CHECK(collectTexts(quoted) == expectedTexts);
  BOOST_CHECK(collectQuotedFlags(quoted) == expectedFlags);
}

BOOST_AUTO_TEST_CASE(keeps_carriage_return_inside_line)
{
  std::vector< std::string > plain = {"add\ruser", "roman"};
  BOOST_CHECK(collectTexts(tokenize("add\ruser roman")) == plain);

  std::vector< std::string > quoted = {"similar", "A\rB"};
  BOOST_CHECK(collectTexts(tokenize("similar \"A\rB\"")) == quoted);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(tokenize_errors_tests)

BOOST_AUTO_TEST_CASE(throws_on_unclosed_quote)
{
  BOOST_CHECK_THROW(tokenize("similar \"Advanced C++"), std::invalid_argument);
  BOOST_CHECK_THROW(tokenize("similar \"Advanced C++\r"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(throws_on_lone_quote)
{
  BOOST_CHECK_THROW(tokenize("\""), std::invalid_argument);
  BOOST_CHECK_THROW(tokenize("similar \""), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(throws_on_text_right_after_closing_quote)
{
  BOOST_CHECK_THROW(tokenize("similar \"abc\"def"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(throws_on_quote_right_after_closing_quote)
{
  BOOST_CHECK_THROW(tokenize("similar \"a\"\"b\""), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(throws_on_quote_inside_word)
{
  BOOST_CHECK_THROW(tokenize("ab\"cd\""), std::invalid_argument);
  BOOST_CHECK_THROW(tokenize("similar ab\""), std::invalid_argument);
  BOOST_CHECK_THROW(tokenize("add-user ro\"man"), std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()
