#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <stdexcept>

#include "Queue.hpp"

BOOST_AUTO_TEST_SUITE(queue_test_suite)

using namespace khasnulin;

BOOST_AUTO_TEST_CASE(test_empty_creation)
{
  Queue< int > q;

  BOOST_REQUIRE(q.empty());
  BOOST_CHECK_EQUAL(q.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_push)
{
  Queue< int > q;

  q.push(1);
  q.push(2);

  BOOST_REQUIRE(q.empty() == false);
  BOOST_CHECK_EQUAL(q.size(), 2);
  BOOST_CHECK_EQUAL(q.front(), 2);
  BOOST_CHECK_EQUAL(q.back(), 1);
}

BOOST_AUTO_TEST_CASE(test_pop)
{
  Queue< int > q;

  q.push(1);
  q.push(2);

  q.pop();

  BOOST_REQUIRE(q.empty() == false);
  BOOST_CHECK_EQUAL(q.front(), 1);
  BOOST_CHECK_EQUAL(q.front(), q.back());

  q.pop();

  BOOST_CHECK(q.empty());
  BOOST_CHECK_EQUAL(q.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_empty_pop)
{
  Queue< int > q;

  BOOST_CHECK_THROW(q.pop(), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()
