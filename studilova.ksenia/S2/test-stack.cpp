#include <string>
#include <utility>

#include <boost/test/unit_test.hpp>

#include "stack.hpp"

BOOST_AUTO_TEST_SUITE(stack_tests)

BOOST_AUTO_TEST_CASE(push_and_pop)
{
  studilova::Stack< int > s;

  s.push(1);
  s.push(2);
  s.push(3);

  BOOST_CHECK_EQUAL(s.top(), 3);
  s.pop();

  BOOST_CHECK_EQUAL(s.top(), 2);
  s.pop();

  BOOST_CHECK_EQUAL(s.top(), 1);
  s.pop();

  BOOST_CHECK(s.empty());
}

BOOST_AUTO_TEST_CASE(top_does_not_remove)
{
  studilova::Stack< int > s;

  s.push(10);
  s.push(20);

  BOOST_CHECK_EQUAL(s.top(), 20);
  BOOST_CHECK_EQUAL(s.top(), 20);
  BOOST_CHECK_EQUAL(s.size(), 2);
}

BOOST_AUTO_TEST_CASE(empty_and_size)
{
  studilova::Stack< int > s;

  BOOST_CHECK(s.empty());
  BOOST_CHECK_EQUAL(s.size(), 0);

  s.push(1);
  BOOST_CHECK(!s.empty());
  BOOST_CHECK_EQUAL(s.size(), 1);
}

BOOST_AUTO_TEST_CASE(pop_empty_throws)
{
  studilova::Stack< int > s;

  BOOST_CHECK_THROW(s.pop(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(top_empty_throws)
{
  studilova::Stack< int > s;

  BOOST_CHECK_THROW(s.top(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(string_stack)
{
  studilova::Stack< std::string > s;

  s.push("hello");
  s.push("world");

  BOOST_CHECK_EQUAL(s.top(), "world");
  s.pop();

  BOOST_CHECK_EQUAL(s.top(), "hello");
}

BOOST_AUTO_TEST_CASE(clear_stack)
{
  studilova::Stack< int > s;

  s.push(1);
  s.push(2);
  s.clear();

  BOOST_CHECK(s.empty());
  BOOST_CHECK_EQUAL(s.size(), 0);
}

BOOST_AUTO_TEST_CASE(const_top)
{
  studilova::Stack< int > s;

  s.push(10);

  const studilova::Stack< int >& constS = s;

  BOOST_CHECK_EQUAL(constS.top(), 10);
}

BOOST_AUTO_TEST_CASE(emplace_stack)
{
  studilova::Stack< std::pair< int, std::string > > s;

  s.emplace(10, "hello");

  BOOST_CHECK_EQUAL(s.top().first, 10);
  BOOST_CHECK_EQUAL(s.top().second, "hello");
  BOOST_CHECK_EQUAL(s.size(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
