#define BOOST_TEST_MODULE QueueTests
#include <boost/test/included/unit_test.hpp>

#include "queue.hpp"
#include <string>

BOOST_AUTO_TEST_SUITE(queue_tests)

BOOST_AUTO_TEST_CASE(push_and_pop)
{
  studilova::Queue< int > q;

  q.push(1);
  q.push(2);
  q.push(3);

  BOOST_CHECK_EQUAL(q.front(), 1);
  q.pop();

  BOOST_CHECK_EQUAL(q.front(), 2);
  q.pop();

  BOOST_CHECK_EQUAL(q.front(), 3);
  q.pop();

  BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_CASE(front_does_not_remove)
{
  studilova::Queue< int > q;

  q.push(10);
  q.push(20);

  BOOST_CHECK_EQUAL(q.front(), 10);
  BOOST_CHECK_EQUAL(q.front(), 10);
  BOOST_CHECK_EQUAL(q.size(), 2);
}

BOOST_AUTO_TEST_CASE(empty_and_size)
{
  studilova::Queue< int > q;

  BOOST_CHECK(q.empty());
  BOOST_CHECK_EQUAL(q.size(), 0);

  q.push(5);
  BOOST_CHECK(!q.empty());
  BOOST_CHECK_EQUAL(q.size(), 1);
}

BOOST_AUTO_TEST_CASE(pop_empty_throws)
{
  studilova::Queue< int > q;

  BOOST_CHECK_THROW(q.pop(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(front_empty_throws)
{
  studilova::Queue< int > q;

  BOOST_CHECK_THROW(q.front(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(string_queue)
{
  studilova::Queue< std::string > q;

  q.push("first");
  q.push("second");

  BOOST_CHECK_EQUAL(q.front(), "first");
  q.pop();

  BOOST_CHECK_EQUAL(q.front(), "second");
}

BOOST_AUTO_TEST_SUITE_END()
