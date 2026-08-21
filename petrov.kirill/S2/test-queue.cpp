#include <boost/test/unit_test.hpp>
#include <string>
#include "Queue.hpp"

BOOST_AUTO_TEST_SUITE(QueTest)

BOOST_AUTO_TEST_CASE(queue_push)
{
  petrov::Queue<int> q;
  BOOST_CHECK(q.empty());

  q.push(10);

  BOOST_CHECK_EQUAL(q.front(), 10);
}

BOOST_AUTO_TEST_CASE(queue_move_and_exceptions)
{
  petrov::Queue<size_t> q1;
  q1.push(2);

  petrov::Queue<size_t> q2(std::move(q1));
  BOOST_CHECK(q1.empty());
  BOOST_CHECK_EQUAL(q2.front(), 2);

  q2.pop();
  BOOST_CHECK(q2.empty());
}

BOOST_AUTO_TEST_SUITE_END()
