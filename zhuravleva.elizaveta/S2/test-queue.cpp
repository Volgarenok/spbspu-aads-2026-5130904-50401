#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include "queue.hpp"

BOOST_AUTO_TEST_CASE(queue_push_drop)
{
  zhuravleva::Queue< int > q;
  q.push(1);
  q.push(2);
  q.push(3);
  BOOST_CHECK_EQUAL(q.front(), 1);
  q.pop();
  BOOST_CHECK_EQUAL(q.front(), 2);
  q.pop();
  BOOST_CHECK_EQUAL(q.front(), 3);
  q.pop();
}

BOOST_AUTO_TEST_CASE(queue_empty)
{
  zhuravleva::Queue< int > q;
  BOOST_CHECK(q.empty());
  q.push(10);
  BOOST_CHECK(!q.empty());
  q.pop();
  BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_CASE(queue_clear)
{
  zhuravleva::Queue< int > q;
  q.push(1);
  q.push(2);
  q.clear();
  BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_CASE(queue_exception_pop)
{
  zhuravleva::Queue< int > q;
  BOOST_CHECK_THROW(q.pop(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(queue_exception_front)
{
  zhuravleva::Queue< int > q;
  BOOST_CHECK_THROW(q.front(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(queue_front)
{
  zhuravleva::Queue< int > q;
  q.push(5);
  q.push(10);
  BOOST_CHECK_EQUAL(q.front(), 5);
  q.pop();
  BOOST_CHECK_EQUAL(q.front(), 10);
}
