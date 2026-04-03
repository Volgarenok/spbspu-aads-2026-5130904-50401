#include "queue.hpp"
#include <string>
#include <stdexcept>
#include <boost/test/unit_test.hpp>

using namespace levkin;


BOOST_AUTO_TEST_CASE(queue_basic_operations)
{
  Queue< long long > q;
  BOOST_CHECK(q.empty());
  BOOST_CHECK_EQUAL(q.size(), 0);

  q.push(10);
  q.push(20);

  BOOST_CHECK(!q.empty());
  BOOST_CHECK_EQUAL(q.size(), 2);
  BOOST_CHECK_EQUAL(q.front(), 10);

  BOOST_CHECK_EQUAL(q.drop(), 10);
  BOOST_CHECK_EQUAL(q.size(), 1);
  BOOST_CHECK_EQUAL(q.front(), 20);
}

BOOST_AUTO_TEST_CASE(queue_exception_safety)
{
  Queue< long long > q;
  BOOST_CHECK_THROW(q.front(), std::out_of_range);
  BOOST_CHECK_THROW(q.drop(), std::out_of_range);

  BOOST_CHECK_NO_THROW(q.pop());
}

BOOST_AUTO_TEST_CASE(queue_move_semantics)
{
  Queue< std::string > q;
  std::string s = "heavy_object";
  q.push(std::move(s));
  BOOST_CHECK_EQUAL(q.front(), "heavy_object");
  BOOST_CHECK(s.empty());
}

