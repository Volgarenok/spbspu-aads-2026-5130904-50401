#include "boost/test/unit_test.hpp"
#include "functions.hpp"

using namespace tarasenko;

BOOST_AUTO_TEST_CASE(get_number)
{
  std::string s = "12345";
  size_t pos = 0;
  BOOST_TEST(getNumber(s, pos) == 12345);
  BOOST_TEST(pos == 5);
  s = "12345a";
  pos = 1;
  BOOST_TEST(getNumber(s, pos) == 2345);
  BOOST_TEST(pos == 5);
}

void checkToken(const Token& token, TokenType type, long long value, char operation, int priority)
{
  BOOST_CHECK(token.type == type);
  BOOST_CHECK(token.value == value);
  BOOST_CHECK(token.operation == operation);
  BOOST_CHECK(token.priority == priority);
}

BOOST_AUTO_TEST_CASE(string_to_queue)
{
  std::string s = "( 1 + 2 ) * ( 3 - 4 )";
  Queue< Token > queue = stringToQueue(s);

  BOOST_TEST(queue.size() == 11);

  checkToken(queue.pop(), TokenType::left_parenthe, 0, 0, 0);
  checkToken(queue.pop(), TokenType::num, 1, 0, 0);
  checkToken(queue.pop(), TokenType::operation, 0, '+', 0);
  checkToken(queue.pop(), TokenType::num, 2, 0, 0);
  checkToken(queue.pop(), TokenType::right_parenthe, 0, 0, 0);
  checkToken(queue.pop(), TokenType::operation, 0, '*', 1);
  checkToken(queue.pop(), TokenType::left_parenthe, 0, 0, 0);
  checkToken(queue.pop(), TokenType::num, 3, 0, 0);
  checkToken(queue.pop(), TokenType::operation, 0, '-', 0);
  checkToken(queue.pop(), TokenType::num, 4, 0, 0);
  checkToken(queue.pop(), TokenType::right_parenthe, 0, 0, 0);

  BOOST_TEST(queue.empty());
}
